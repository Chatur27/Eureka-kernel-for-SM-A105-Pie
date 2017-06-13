/*
 *  linux/fs/fcntl.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#include <linux/syscalls.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/capability.h>
#include <linux/dnotify.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/pipe_fs_i.h>
#include <linux/security.h>
#include <linux/ptrace.h>
#include <linux/signal.h>
#include <linux/rcupdate.h>
#include <linux/pid_namespace.h>
#include <linux/user_namespace.h>
#include <linux/shmem_fs.h>
#include <linux/task_integrity.h>
#include <linux/proca.h>

#include <asm/poll.h>
#include <asm/siginfo.h>
#include <asm/uaccess.h>

#define SETFL_MASK (O_APPEND | O_NONBLOCK | O_NDELAY | O_DIRECT | O_NOATIME)

static int setfl(int fd, struct file * filp, unsigned long arg)
{
	struct inode * inode = file_inode(filp);
	int error = 0;

	/*
	 * O_APPEND cannot be cleared if the file is marked as append-only
	 * and the file is open for write.
	 */
	if (((arg ^ filp->f_flags) & O_APPEND) && IS_APPEND(inode))
		return -EPERM;

	/* O_NOATIME can only be set by the owner or superuser */
	if ((arg & O_NOATIME) && !(filp->f_flags & O_NOATIME))
		if (!inode_owner_or_capable(inode))
			return -EPERM;

	/* required for strict SunOS emulation */
	if (O_NONBLOCK != O_NDELAY)
	       if (arg & O_NDELAY)
		   arg |= O_NONBLOCK;

	if (arg & O_DIRECT) {
		if (!filp->f_mapping || !filp->f_mapping->a_ops ||
			!filp->f_mapping->a_ops->direct_IO)
				return -EINVAL;
	}

	if (filp->f_op->check_flags)
		error = filp->f_op->check_flags(arg);
	if (error)
		return error;

	/*
	 * ->fasync() is responsible for setting the FASYNC bit.
	 */
	if (((arg ^ filp->f_flags) & FASYNC) && filp->f_op->fasync) {
		error = filp->f_op->fasync(fd, filp, (arg & FASYNC) != 0);
		if (error < 0)
			goto out;
		if (error > 0)
			error = 0;
	}
	spin_lock(&filp->f_lock);
	filp->f_flags = (arg & SETFL_MASK) | (filp->f_flags & ~SETFL_MASK);
	spin_unlock(&filp->f_lock);

 out:
	return error;
}

static void f_modown(struct file *filp, struct pid *pid, enum pid_type type,
                     int force)
{
	write_lock_irq(&filp->f_owner.lock);
	if (force || !filp->f_owner.pid) {
		put_pid(filp->f_owner.pid);
		filp->f_owner.pid = get_pid(pid);
		filp->f_owner.pid_type = type;

		if (pid) {
			const struct cred *cred = current_cred();
			filp->f_owner.uid = cred->uid;
			filp->f_owner.euid = cred->euid;
		}
	}
	write_unlock_irq(&filp->f_owner.lock);
}

void __f_setown(struct file *filp, struct pid *pid, enum pid_type type,
		int force)
{
	security_file_set_fowner(filp);
	f_modown(filp, pid, type, force);
}
EXPORT_SYMBOL(__f_setown);

void f_setown(struct file *filp, unsigned long arg, int force)
{
	enum pid_type type;
	struct pid *pid;
	int who = arg;
	type = PIDTYPE_PID;
	if (who < 0) {
		/* avoid overflow below */
		if (who == INT_MIN)
			return;

		type = PIDTYPE_PGID;
		who = -who;
	}
	rcu_read_lock();
	pid = find_vpid(who);
	__f_setown(filp, pid, type, force);
	rcu_read_unlock();
}
EXPORT_SYMBOL(f_setown);

void f_delown(struct file *filp)
{
	f_modown(filp, NULL, PIDTYPE_PID, 1);
}

pid_t f_getown(struct file *filp)
{
	pid_t pid;
	read_lock(&filp->f_owner.lock);
	pid = pid_vnr(filp->f_owner.pid);
	if (filp->f_owner.pid_type == PIDTYPE_PGID)
		pid = -pid;
	read_unlock(&filp->f_owner.lock);
	return pid;
}

static int f_setown_ex(struct file *filp, unsigned long arg)
{
	struct f_owner_ex __user *owner_p = (void __user *)arg;
	struct f_owner_ex owner;
	struct pid *pid;
	int type;
	int ret;

	ret = copy_from_user(&owner, owner_p, sizeof(owner));
	if (ret)
		return -EFAULT;

	switch (owner.type) {
	case F_OWNER_TID:
		type = PIDTYPE_MAX;
		break;

	case F_OWNER_PID:
		type = PIDTYPE_PID;
		break;

	case F_OWNER_PGRP:
		type = PIDTYPE_PGID;
		break;

	default:
		return -EINVAL;
	}

	rcu_read_lock();
	pid = find_vpid(owner.pid);
	if (owner.pid && !pid)
		ret = -ESRCH;
	else
		 __f_setown(filp, pid, type, 1);
	rcu_read_unlock();

	return ret;
}

static int f_getown_ex(struct file *filp, unsigned long arg)
{
	struct f_owner_ex __user *owner_p = (void __user *)arg;
	struct f_owner_ex owner;
	int ret = 0;

	read_lock(&filp->f_owner.lock);
	owner.pid = pid_vnr(filp->f_owner.pid);
	switch (filp->f_owner.pid_type) {
	case PIDTYPE_MAX:
		owner.type = F_OWNER_TID;
		break;

	case PIDTYPE_PID:
		owner.type = F_OWNER_PID;
		break;

	case PIDTYPE_PGID:
		owner.type = F_OWNER_PGRP;
		break;

	default:
		WARN_ON(1);
		ret = -EINVAL;
		break;
	}
	read_unlock(&filp->f_owner.lock);

	if (!ret) {
		ret = copy_to_user(owner_p, &owner, sizeof(owner));
		if (ret)
			ret = -EFAULT;
	}
	return ret;
}

#ifdef CONFIG_CHECKPOINT_RESTORE
static int f_getowner_uids(struct file *filp, unsigned long arg)
{
	struct user_namespace *user_ns = current_user_ns();
	uid_t __user *dst = (void __user *)arg;
	uid_t src[2];
	int err;

	read_lock(&filp->f_owner.lock);
	src[0] = from_kuid(user_ns, filp->f_owner.uid);
	src[1] = from_kuid(user_ns, filp->f_owner.euid);
	read_unlock(&filp->f_owner.lock);

	err  = put_user(src[0], &dst[0]);
	err |= put_user(src[1], &dst[1]);

	return err;
}
#else
static int f_getowner_uids(struct file *filp, unsigned long arg)
{
	return -EINVAL;
}
#endif

static long do_fcntl(int fd, unsigned int cmd, unsigned long arg,
		struct file *filp)
{
	long err = -EINVAL;

	switch (cmd) {
	case F_DUPFD:
		err = f_dupfd(arg, filp, 0);
		break;
	case F_DUPFD_CLOEXEC:
		err = f_dupfd(arg, filp, O_CLOEXEC);
		break;
	case F_GETFD:
		err = get_close_on_exec(fd) ? FD_CLOEXEC : 0;
		break;
	case F_SETFD:
		err = 0;
		set_close_on_exec(fd, arg & FD_CLOEXEC);
		break;
	case F_GETFL:
		err = filp->f_flags;
		break;
	case F_SETFL:
		err = setfl(fd, filp, arg);
		break;
#if BITS_PER_LONG != 32
	/* 32-bit arches must use fcntl64() */
	case F_OFD_GETLK:
#endif
	case F_GETLK:
		err = fcntl_getlk(filp, cmd, (struct flock __user *) arg);
		break;
#if BITS_PER_LONG != 32
	/* 32-bit arches must use fcntl64() */
	case F_OFD_SETLK:
	case F_OFD_SETLKW:
#endif
		/* Fallthrough */
	case F_SETLK:
	case F_SETLKW:
		err = fcntl_setlk(fd, filp, cmd, (struct flock __user *) arg);
		break;
	case F_GETOWN:
		/*
		 * XXX If f_owner is a process group, the
		 * negative return value will get converted
		 * into an error.  Oops.  If we keep the
		 * current syscall conventions, the only way
		 * to fix this will be in libc.
		 */
		err = f_getown(filp);
		force_successful_syscall_return();
		break;
	case F_SETOWN:
		f_setown(filp, arg, 1);
		err = 0;
		break;
	case F_GETOWN_EX:
		err = f_getown_ex(filp, arg);
		break;
	case F_SETOWN_EX:
		err = f_setown_ex(filp, arg);
		break;
	case F_GETOWNER_UIDS:
		err = f_getowner_uids(filp, arg);
		break;
	case F_GETSIG:
		err = filp->f_owner.signum;
		break;
	case F_SETSIG:
		/* arg == 0 restores default behaviour. */
		if (!valid_signal(arg)) {
			break;
		}
		err = 0;
		filp->f_owner.signum = arg;
		break;
	case F_GETLEASE:
		err = fcntl_getlease(filp);
		break;
	case F_SETLEASE:
		err = fcntl_setlease(fd, filp, arg);
		break;
	case F_NOTIFY:
		err = fcntl_dirnotify(fd, filp, arg);
		break;
	case F_SETPIPE_SZ:
	case F_GETPIPE_SZ:
		err = pipe_fcntl(filp, cmd, arg);
		break;
#ifdef CONFIG_FIVE
	case F_FIVE_SIGN:
		err = five_fcntl_sign(filp,
				(struct integrity_label __user *)arg);
		break;
	case F_FIVE_VERIFY_ASYNC:
		err = five_fcntl_verify_async(filp);
		break;
	case F_FIVE_VERIFY_SYNC:
		err = five_fcntl_verify_sync(filp);
		break;
#if defined(CONFIG_FIVE_PA_FEATURE) || defined(CONFIG_PROCA)
	case F_FIVE_PA_SETXATTR:
		err = proca_fcntl_setxattr(filp, (void __user *)arg);
		break;
#endif
#endif
	case F_ADD_SEALS:
	case F_GET_SEALS:
		err = shmem_fcntl(filp, cmd, arg);
		break;
	default:
		break;
	}
	return err;
}

static int check_fcntl_cmd(unsigned cmd)
{
	switch (cmd) {
	case F_DUPFD:
	case F_DUPFD_CLOEXEC:
	case F_GETFD:
	case F_SETFD:
	case F_GETFL:
		return 1;
	}
	return 0;
}

SYSCALL_DEFINE3(fcntl, unsigned int, fd, unsigned int, cmd, unsigned long, arg)
{	
	struct fd f = fdget_raw(fd);
	long err = -EBADF;

	if (!f.file)
		goto out;

	if (unlikely(f.file->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd))
			goto out1;
	}

	err = security_file_fcntl(f.file, cmd, arg);
	if (!err)
		err = do_fcntl(fd, cmd, arg, f.file);

out1:
 	fdput(f);
out:
	return err;
}

#if BITS_PER_LONG == 32
SYSCALL_DEFINE3(fcntl64, unsigned int, fd, unsigned int, cmd,
		unsigned long, arg)
{	
	struct fd f = fdget_raw(fd);
	long err = -EBADF;

	if (!f.file)
		goto out;

	if (unlikely(f.file->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd))
			goto out1;
	}

	err = security_file_fcntl(f.file, cmd, arg);
	if (err)
		goto out1;
	
	switch (cmd) {
	case F_GETLK64:
	case F_OFD_GETLK:
		err = fcntl_getlk64(f.file, cmd, (struct flock64 __user *) arg);
		break;
	case F_SETLK64:
	case F_SETLKW64:
	case F_OFD_SETLK:
	case F_OFD_SETLKW:
		err = fcntl_setlk64(fd, f.file, cmd,
				(struct flock64 __user *) arg);
		break;
	default:
		err = do_fcntl(fd, cmd, arg, f.file);
		break;
	}
out1:
	fdput(f);
out:
	return err;
}
#endif

/* Table to convert sigio signal codes into poll band bitmaps */

static const long band_table[NSIGPOLL] = {
	POLLIN | POLLRDNORM,			/* POLL_IN */
	POLLOUT | POLLWRNORM | POLLWRBAND,	/* POLL_OUT */
	POLLIN | POLLRDNORM | POLLMSG,		/* POLL_MSG */
	POLLERR,				/* POLL_ERR */
	POLLPRI | POLLRDBAND,			/* POLL_PRI */
	POLLHUP | POLLERR			/* POLL_HUP */
};

static inline int sigio_perm(struct task_struct *p,
                             struct fown_struct *fown, int sig)
{
	const struct cred *cred;
	int ret;

	rcu_read_lock();
	cred = __task_cred(p);
	ret = ((uid_eq(fown->euid, GLOBAL_ROOT_UID) ||
		uid_eq(fown->euid, cred->suid) || uid_eq(fown->euid, cred->uid) ||
		uid_eq(fown->uid,  cred->suid) || uid_eq(fown->uid,  cred->uid)) &&
	       !security_file_send_sigiotask(p, fown, sig));
	rcu_read_unlock();
	return ret;
}

static void send_sigio_to_task(struct task_struct *p,
			       struct fown_struct *fown,
			       int fd, int reason, int group)
{
	/*
	 * F_SETSIG can change ->signum lockless in parallel, make
	 * sure we read it once and use the same value throughout.
	 */
	int signum = ACCESS_ONCE(fown->signum);

	if (!sigio_perm(p, fown, signum))
		return;

	switch (signum) {
		siginfo_t si;
		default:
			/* Queue a rt signal with the appropriate fd as its
			   value.  We use SI_SIGIO as the source, not 
			   SI_KERNEL, since kernel signals always get 
			   delivered even if we can't queue.  Failure to
			   queue in this case _should_ be reported; we fall
			   back to SIGIO in that case. --sct */
			si.si_signo = signum;
			si.si_errno = 0;
		        si.si_code  = reason;
			/* Make sure we are called with one of the POLL_*
			   reasons, otherwise we could leak kernel stack into
			   userspace.  */
			BUG_ON((reason & __SI_MASK) != __SI_POLL);
			if (reason - POLL_IN >= NSIGPOLL)
				si.si_band  = ~0L;
			else
				si.si_band = band_table[reason - POLL_IN];
			si.si_fd    = fd;
			if (!do_send_sig_info(signum, &si, p, group))
				break;
		/* fall-through: fall back on the old plain SIGIO signal */
		case 0:
			do_send_sig_info(SIGIO, SEND_SIG_PRIV, p, group);
	}
}

void send_sigio(struct fown_struct *fown, int fd, int band)
{
	struct task_struct *p;
	enum pid_type type;
	struct pid *pid;
	int group = 1;
	
	read_lock(&fown->lock);

	type = fown->pid_type;
	if (type == PIDTYPE_MAX) {
		group = 0;
		type = PIDTYPE_PID;
	}

	pid = fown->pid;
	if (!pid)
		goto out_unlock_fown;
	
	read_lock(&tasklist_lock);
	do_each_pid_task(pid, type, p) {
		send_sigio_to_task(p, fown, fd, band, group);
	} while_each_pid_task(pid, type, p);
	read_unlock(&tasklist_lock);
 out_unlock_fown:
	read_unlock(&fown->lock);
}

static void send_sigurg_to_task(struct task_struct *p,
				struct fown_struct *fown, int group)
{
	if (sigio_perm(p, fown, SIGURG))
		do_send_sig_info(SIGURG, SEND_SIG_PRIV, p, group);
}

int send_sigurg(struct fown_struct *fown)
{
	struct task_struct *p;
	enum pid_type type;
	struct pid *pid;
	int group = 1;
	int ret = 0;
	
	read_lock(&fown->lock);

	type = fown->pid_type;
	if (type == PIDTYPE_MAX) {
		group = 0;
		type = PIDTYPE_PID;
	}

	pid = fown->pid;
	if (!pid)
		goto out_unlock_fown;

	ret = 1;
	
	read_lock(&tasklist_lock);
	do_each_pid_task(pid, type, p) {
		send_sigurg_to_task(p, fown, group);
	} while_each_pid_task(pid, type, p);
	read_unlock(&tasklist_lock);
 out_unlock_fown:
	read_unlock(&fown->lock);
	return ret;
}

static DEFINE_SPINLOCK(fasync_lock);
static struct kmem_cache *fasync_cache __read_mostly;

static void fasync_free_rcu(struct rcu_head *head)
{
	kmem_cache_free(fasync_cache,
			container_of(head, struct fasync_struct, fa_rcu));
}

/*
 * Remove a fasync entry. If successfully removed, return
 * positive and clear the FASYNC flag. If no entry exists,
 * do nothing and return 0.
 *
 * NOTE! It is very important that the FASYNC flag always
 * match the state "is the filp on a fasync list".
 *
 */
int fasync_remove_entry(struct file *filp, struct fasync_struct **fapp)
{
	struct fasync_struct *fa, **fp;
	int result = 0;

	spin_lock(&filp->f_lock);
	spin_lock(&fasync_lock);
	for (fp = fapp; (fa = *fp) != NULL; fp = &fa->fa_next) {
		if (fa->fa_file != filp)
			continue;

		spin_lock_irq(&fa->fa_lock);
		fa->fa_file = NULL;
		spin_unlock_irq(&fa->fa_lock);

		*fp = fa->fa_next;
		call_rcu(&fa->fa_rcu, fasync_free_rcu);
		filp->f_flags &= ~FASYNC;
		result = 1;
		break;
	}
	spin_unlock(&fasync_lock);
	spin_unlock(&filp->f_lock);
	return result;
}

struct fasync_struct *fasync_alloc(void)
{
	return kmem_cache_alloc(fasync_cache, GFP_KERNEL);
}

/*
 * NOTE! This can be used only for unused fasync entries:
 * entries that actually got inserted on the fasync list
 * need to be released by rcu - see fasync_remove_entry.
 */
void fasync_free(struct fasync_struct *new)
{
	kmem_cache_free(fasync_cache, new);
}

/*
 * Insert a new entry into the fasync list.  Return the pointer to the
 * old one if we didn't use the new one.
 *
 * NOTE! It is very important that the FASYNC flag always
 * match the state "is the filp on a fasync list".
 */
struct fasync_struct *fasync_insert_entry(int fd, struct file *filp, struct fasync_struct **fapp, struct fasync_struct *new)
{
        struct fasync_struct *fa, **fp;

	spin_lock(&filp->f_lock);
	spin_lock(&fasync_lock);
	for (fp = fapp; (fa = *fp) != NULL; fp = &fa->fa_next) {
		if (fa->fa_file != filp)
			continue;

		spin_lock_irq(&fa->fa_lock);
		fa->fa_fd = fd;
		spin_unlock_irq(&fa->fa_lock);
		goto out;
	}

	spin_lock_init(&new->fa_lock);
	new->magic = FASYNC_MAGIC;
	new->fa_file = filp;
	new->fa_fd = fd;
	new->fa_next = *fapp;
	rcu_assign_pointer(*fapp, new);
	filp->f_flags |= FASYNC;

out:
	spin_unlock(&fasync_lock);
	spin_unlock(&filp->f_lock);
	return fa;
}

/*
 * Add a fasync entry. Return negative on error, positive if
 * added, and zero if did nothing but change an existing one.
 */
static int fasync_add_entry(int fd, struct file *filp, struct fasync_struct **fapp)
{
	struct fasync_struct *new;

	new = fasync_alloc();
	if (!new)
		return -ENOMEM;

	/*
	 * fasync_insert_entry() returns the old (update) entry if
	 * it existed.
	 *
	 * So free the (unused) new entry and return 0 to let the
	 * caller know that we didn't add any new fasync entries.
	 */
	if (fasync_insert_entry(fd, filp, fapp, new)) {
		fasync_free(new);
		return 0;
	}

	return 1;
}

/*
 * fasync_helper() is used by almost all character device drivers
 * to set up the fasync queue, and for regular files by the file
 * lease code. It returns negative on error, 0 if it did no changes
 * and positive if it added/deleted the entry.
 */
int fasync_helper(int fd, struct file * filp, int on, struct fasync_struct **fapp)
{
	if (!on)
		return fasync_remove_entry(filp, fapp);
	return fasync_add_entry(fd, filp, fapp);
}

EXPORT_SYMBOL(fasync_helper);

/*
 * rcu_read_lock() is held
 */
static void kill_fasync_rcu(struct fasync_struct *fa, int sig, int band)
{
	while (fa) {
		struct fown_struct *fown;
		unsigned long flags;

		if (fa->magic != FASYNC_MAGIC) {
			printk(KERN_ERR "kill_fasync: bad magic number in "
			       "fasync_struct!\n");
			return;
		}
		spin_lock_irqsave(&fa->fa_lock, flags);
		if (fa->fa_file) {
			fown = &fa->fa_file->f_owner;
			/* Don't send SIGURG to processes which have not set a
			   queued signum: SIGURG has its own default signalling
			   mechanism. */
			if (!(sig == SIGURG && fown->signum == 0))
				send_sigio(fown, fa->fa_fd, band);
		}
		spin_unlock_irqrestore(&fa->fa_lock, flags);
		fa = rcu_dereference(fa->fa_next);
	}
}

void kill_fasync(struct fasync_struct **fp, int sig, int band)
{
	/* First a quick test without locking: usually
	 * the list is empty.
	 */
	if (*fp) {
		rcu_read_lock();
		kill_fasync_rcu(rcu_dereference(*fp), sig, band);
		rcu_read_unlock();
	}
}
EXPORT_SYMBOL(kill_fasync);

static int __init fcntl_init(void)
{
	/*
	 * Please add new bits here to ensure allocation uniqueness.
	 * Exceptions: O_NONBLOCK is a two bit define on parisc; O_NDELAY
	 * is defined as O_NONBLOCK on some platforms and not on others.
	 */
	BUILD_BUG_ON(21 - 1 /* for O_RDONLY being 0 */ !=
		HWEIGHT32(
			(VALID_OPEN_FLAGS & ~(O_NONBLOCK | O_NDELAY)) |
			__FMODE_EXEC | __FMODE_NONOTIFY));

	fasync_cache = kmem_cache_create("fasync_cache",
		sizeof(struct fasync_struct), 0, SLAB_PANIC, NULL);
	return 0;
}

module_init(fcntl_init)
