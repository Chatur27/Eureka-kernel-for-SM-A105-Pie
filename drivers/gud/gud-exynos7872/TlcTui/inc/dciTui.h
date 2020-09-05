/*
 * Copyright (c) 2013-2016 TRUSTONIC LIMITED
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __DCITUI_H__
#define __DCITUI_H__

/**< Responses have bit 31 set */

/* Linux checkpatch suggests to use the BIT macro */
#ifndef BIT
#define BIT(n) (1U << (n))
#endif

#define RSP_ID_MASK BIT(31)
#define RSP_ID(cmd_id) (((uint32_t)(cmd_id)) | RSP_ID_MASK)
#define IS_CMD(cmd_id) ((((uint32_t)(cmd_id)) & RSP_ID_MASK) == 0)
#define IS_RSP(cmd_id) ((((uint32_t)(cmd_id)) & RSP_ID_MASK) == RSP_ID_MASK)
#define CMD_ID_FROM_RSP(rsp_id) (rsp_id & (~RSP_ID_MASK))

/**
 * Return codes of driver commands.
 */
#define TUI_DCI_OK                      0x00030000
#define TUI_DCI_ERR_UNKNOWN_CMD         0x00030001
#define TUI_DCI_ERR_NOT_SUPPORTED       0x00030002
#define TUI_DCI_ERR_INTERNAL_ERROR      0x00030003
#define TUI_DCI_ERR_NO_RESPONSE         0x00030004
#define TUI_DCI_ERR_BAD_PARAMETERS      0x00030005
#define TUI_DCI_ERR_NO_EVENT            0x00030006
#define TUI_DCI_ERR_OUT_OF_DISPLAY      0x00030007
/* ... add more error codes when needed */

/**
 * Notification ID's for communication Trustlet Connector -> Driver.
 */
#define NOT_TUI_NONE                0
/* NWd system event that closes the current TUI session*/
#define NOT_TUI_CANCEL_EVENT        1
/* TODO put this in HAL specific code */
#define NOT_TUI_HAL_TOUCH_EVENT     0x80000001

/**
 * Command ID's for communication Driver -> Trustlet Connector.
 */
#define CMD_TUI_SW_NONE             0
/* SWd request to NWd to start the TUI session */
#define CMD_TUI_SW_OPEN_SESSION     1
/* SWd request to NWd to close the TUI session */
#define CMD_TUI_SW_CLOSE_SESSION    2
/* SWd request to NWd stop accessing display controller */
#define CMD_TUI_SW_STOP_DISPLAY     3
/* SWd request to get TlcTui DCI version */
#define CMD_TUI_SW_GET_VERSION      4
/* SWd request to NWd to execute a HAL command */
#define CMD_TUI_SW_HAL              5

#define CMD_TUI_HAL_NONE                    0
#define CMD_TUI_HAL_QUEUE_BUFFER            1
#define CMD_TUI_HAL_QUEUE_DEQUEUE_BUFFER    2
#define CMD_TUI_HAL_CLEAR_TOUCH_INTERRUPT   3
#define CMD_TUI_HAL_HIDE_SURFACE            4
#define CMD_TUI_HAL_GET_RESOLUTION          5

/**
 * Maximum data length.
 */
#define MAX_DCI_DATA_LEN (1024 * 100)

/*
 * TUI DCI VERSION
 */
#define TUI_DCI_VERSION_MAJOR   (1u)
#define TUI_DCI_VERSION_MINOR   (1u)

#define TUI_DCI_VERSION(major, minor) \
	(((major & 0x0000ffff) << 16) | (minor & 0x0000ffff))
#define TUI_DCI_VERSION_GET_MAJOR(version) ((version >> 16) & 0x0000ffff)
#define TUI_DCI_VERSION_GET_MINOR(version) (version & 0x0000ffff)

/* Command payload */

struct tui_disp_data_t {
	uint32_t buff_id;
};

struct tui_hal_cmd_t {
	uint32_t id;    /* Id of the HAL command */
	uint32_t size;  /* Size of the data associated to the HAL command */
	uint64_t data[2];   /* Data associated to the HAL command */
};

struct tui_hal_rsp_t {
	uint32_t id;    /* Id of the HAL response */
	uint32_t return_code;   /* Return code of the HAL response */
	uint32_t size;  /* Size of the data associated to the HAL response */
	uint32_t data[3];   /* Data associated to the HAL response */
};

struct tui_alloc_data_t {
	uint32_t alloc_size;
	uint32_t num_of_buff;
};

union dci_cmd_payload_t {
	struct tui_alloc_data_t alloc_data;
	struct tui_disp_data_t  disp_data;
	struct tui_hal_cmd_t    hal;
};

/* Command */
struct dci_command_t {
	uint32_t id;
	union dci_cmd_payload_t payload;
};

/* TUI frame buffer (output from NWd) */
struct tui_alloc_buffer_t {
	uint64_t    pa;
};

#define MAX_DCI_BUFFER_NUMBER 4

/* Response */
struct dci_response_t {
	uint32_t	id; /* must be command ID | RSP_ID_MASK */
	uint32_t		return_code;
	union {
		struct tui_alloc_buffer_t alloc_buffer[MAX_DCI_BUFFER_NUMBER];
		struct tui_hal_rsp_t hal_rsp;
	};
};

/* DCI buffer */
struct tui_dci_msg_t {
	uint32_t version;
	uint32_t     nwd_notif; /* Notification from TlcTui to DrTui */
	struct dci_command_t  cmd_nwd;   /* Command from DrTui to TlcTui */
	struct dci_response_t nwd_rsp;   /* Response from TlcTui to DrTui */
	uint32_t     hal_cmd;
	uint32_t     hal_rsp;
};

/**
 * Driver UUID. Update accordingly after reserving UUID
 */
#define DR_TUI_UUID { { 7, 0xC, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }

#endif /* __DCITUI_H__ */
