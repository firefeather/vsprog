/**************************************************************************
 *  Copyright (C) 2008 - 2010 by Simon Qian                               *
 *  SimonQian@SimonQian.com                                               *
 *                                                                        *
 *  Project:    Versaloon                                                 *
 *  File:       USB_TO_LPCICP.c                                           *
 *  Author:     SimonQian                                                 *
 *  Versaion:   See changelog                                             *
 *  Purpose:    implementation file for USB_TO_LPCICP                     *
 *  License:    See license                                               *
 *------------------------------------------------------------------------*
 *  Change Log:                                                           *
 *      YYYY-MM-DD:     What(by Who)                                      *
 *      2008-11-07:     created(by SimonQian)                             *
 **************************************************************************/

#include "app_cfg.h"
#if USB_TO_LPCICP_EN

#include "USB_TO_XXX.h"
#include "interfaces.h"

void USB_TO_LPCICP_ProcessCmd(uint8* dat, uint16 len)
{
	uint16 index, length;
	uint8 command, device_idx;
	
	index = 0;
	while(index < len)
	{
		command = dat[index] & USB_TO_XXX_CMDMASK;
		device_idx = dat[index] & USB_TO_XXX_IDXMASK;
		length = GET_LE_U16(&dat[index + 1]);
		index += 3;
		
		switch(command)
		{
		case USB_TO_XXX_INIT:
			if (ERROR_OK == interfaces->lpcicp.init(device_idx))
			{
				buffer_reply[rep_len++] = USB_TO_XXX_OK;
			}
			else
			{
				buffer_reply[rep_len++] = USB_TO_XXX_FAILED;
			}
			break;
		case USB_TO_XXX_FINI:
			if (ERROR_OK == interfaces->lpcicp.fini(device_idx))
			{
				buffer_reply[rep_len++] = USB_TO_XXX_OK;
			}
			else
			{
				buffer_reply[rep_len++] = USB_TO_XXX_FAILED;
			}
			break;
		case USB_TO_LPCICP_EnterProgMode:
			if (ERROR_OK == interfaces->lpcicp.enter_program_mode(device_idx))
			{
				buffer_reply[rep_len++] = USB_TO_XXX_OK;
			}
			else
			{
				buffer_reply[rep_len++] = USB_TO_XXX_FAILED;
			}
			break;
		case USB_TO_LPCICP_In:
			if (ERROR_OK == interfaces->lpcicp.in(device_idx, &buffer_reply[rep_len + 1], length))
			{
				buffer_reply[rep_len++] = USB_TO_XXX_OK;
			}
			else
			{
				buffer_reply[rep_len++] = USB_TO_XXX_FAILED;
			}
			rep_len += length;
			break;
		case USB_TO_LPCICP_Out:
			if (ERROR_OK == interfaces->lpcicp.out(device_idx, &dat[index], length))
			{
				buffer_reply[rep_len++] = USB_TO_XXX_OK;
			}
			else
			{
				buffer_reply[rep_len++] = USB_TO_XXX_FAILED;
			}
			break;
		case USB_TO_LPCICP_PollRdy:
			if (ERROR_OK == interfaces->lpcicp.poll_ready(device_idx, dat[index + 0], 
								&buffer_reply[rep_len + 1], dat[index + 1], 
								dat[index + 2], GET_LE_U16(&dat[index + 3])))
			{
				buffer_reply[rep_len++] = USB_TO_XXX_OK;
			}
			else
			{
				buffer_reply[rep_len++] = USB_TO_XXX_FAILED;
			}
			rep_len++;
			break;
		default:
			buffer_reply[rep_len++] = USB_TO_XXX_CMD_NOT_SUPPORT;
			break;
		}
		index += length;
	}
}

#endif
