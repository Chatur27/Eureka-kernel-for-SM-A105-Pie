#include "pmucal_gnss.h"
#include "pmucal_rae.h"

/**
 *  pmucal_gnss_init - init gnss.
 *		        exposed to PWRCAL interface.

 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_gnss_init(void)
{
	int ret;

	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_gnss_list.init) {
		pr_err("%s there is no sequence element for gnss-init.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_seq(pmucal_gnss_list.init,
				pmucal_gnss_list.num_init);
	if (ret) {
		pr_err("%s %s: error on handling gnss-init sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_gnss_status - get gnss status.
 *		        exposed to PWRCAL interface.
 */
int pmucal_gnss_status(void)
{
	int ret = 0;

	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_gnss_list.status) {
		pr_err("%s there is no sequence element for gnss-status.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}
	
	if (pmucal_gnss_list.num_status != 1) {
		pr_err("%s Something is strange. The number of status seq is not 1.\n",
				PMUCAL_PREFIX);
		return -EFAULT;
	}

	struct pmucal_seq *seq = pmucal_gnss_list.status;
	ret = pmucal_rae_read(pmucal_gnss_list.status);

	return ret;
}

/**
 *  pmucal_gnss_reset_assert - reset assert gnss.
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_gnss_reset_assert(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_gnss_list.reset_assert) {
		pr_err("%s there is no sequence element for gnss-reset_assert.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_seq(pmucal_gnss_list.reset_assert,
				pmucal_gnss_list.num_reset_assert);
	if (ret) {
		pr_err("%s %s: error on handling gnss-reset_assert sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_gnss_reset_release - reset_release gnss.
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_gnss_reset_release(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_gnss_list.reset_release) {
		pr_err("%s there is no sequence element for gnss-reset_release.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_seq(pmucal_gnss_list.reset_release,
				pmucal_gnss_list.num_reset_release);
	if (ret) {
		pr_err("%s %s: error on handling gnss-reset_release sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

int pmucal_is_gnss_regs(int reg) {
	int i;
	int is_gnss_regs = 0;

	for (i = 0; i < pmucal_gnss_list.num_init; i++) {
		if (reg == pmucal_gnss_list.init[i].base_pa + pmucal_gnss_list.init[i].offset) {
			is_gnss_regs = 1;
			goto out;
		}
	}

	for (i = 0; i < pmucal_gnss_list.num_reset_assert; i++) {
		if (reg == pmucal_gnss_list.reset_assert[i].base_pa + pmucal_gnss_list.reset_assert[i].offset) {
			is_gnss_regs = 1;
			goto out;
		}
	}

	for (i = 0; i < pmucal_gnss_list.num_reset_release; i++) {
		if (reg == pmucal_gnss_list.reset_release[i].base_pa + pmucal_gnss_list.reset_release[i].offset) {
			is_gnss_regs = 1;
			goto out;
		}
	}
out:
	return is_gnss_regs;
}

/**
 *  pmucal_gnss_active_clear - gnss_active clear
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_gnss_active_clear(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_gnss_list.gnss_active_clear) {
		pr_err("%s there is no sequence element for gnss_active_clear.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_seq(pmucal_gnss_list.gnss_active_clear,
				pmucal_gnss_list.num_gnss_active_clear);
	if (ret) {
		pr_err("%s %s: error on handling gnss_active_clear sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_gnss_reset_req_clear - gnss_reset_req clear
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_gnss_reset_req_clear(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_gnss_list.gnss_reset_req_clear) {
		pr_err("%s there is no sequence element for gnss_reset_req_clear.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_seq(pmucal_gnss_list.gnss_reset_req_clear,
				pmucal_gnss_list.num_gnss_reset_req_clear);
	if (ret) {
		pr_err("%s %s: error on handling gnss_reset_req_clear sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_gnss_initialize - Initialize function of PMUCAL gnss common logic.
 *		            exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int __init pmucal_gnss_initialize(void)
{
	int ret = 0;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_gnss_list_size) {
		pr_err("%s %s: there is no gnss list. aborting init...\n",
				PMUCAL_PREFIX, __func__);
		return -ENOENT;
	}

	/* convert physical base address to virtual addr */
	ret = pmucal_rae_phy2virt(pmucal_gnss_list.init,
				pmucal_gnss_list.num_init);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting init...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}

	ret = pmucal_rae_phy2virt(pmucal_gnss_list.status,
				pmucal_gnss_list.num_status);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting status...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}

	ret = pmucal_rae_phy2virt(pmucal_gnss_list.reset_assert,
				pmucal_gnss_list.num_reset_assert);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting reset_assert...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}

	ret = pmucal_rae_phy2virt(pmucal_gnss_list.reset_release,
				pmucal_gnss_list.num_reset_release);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting reset_release...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}

out:
	return ret;
}
