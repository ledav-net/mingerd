/*
 * The Minger Email Address Verification Daemon
 * (C)opyright 2012 David De Grave <david@ledav.net>
 *
 * http://sourceforge.net/projects/mingerd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "common.h"

#include <stdio.h>

#define CF_D_PREFIX	"/home/indians/src/mingerd/tmp"
#define CF_D_PREFIX_VAR	CF_D_PREFIX"/var"
#define CF_D_PREFIX_ETC CF_D_PREFIX

#define CF_D_RUN	CF_D_PREFIX_VAR"/run"
#define CF_D_LOG	CF_D_PREFIX_VAR"/log"
#define CF_D_ETC	CF_D_PREFIX_ETC"/etc"

#define CF_DEFAULT_IP	"0.0.0.0"
#define CF_DEFAULT_PORT	4069

typedef struct _config {
		int		listenport,				/* 0..65535						*/
				logpriority;				/* Default priority for logging				*/
		bool		anonymous;				/* Allow anonymous requests				*/
		char		*listenip,				/* xxx.xxx.xxx.xxx					*/
				*secret;				/* secret password					*/
} CONFIG;

typedef struct _confitem {
		char		*name;					/* Name of config item					*/
		int		(*handler)(char *, void *);		/* Handler to process it				*/
		void		*value;					/* Place to write the value (2nd arg to the handler)	*/
} CONFITEM;

extern CONFIG			config;					/* Global configuration					*/

extern int			read_config(				/* Read configuration file				*/
					CONFIG *,			/* Configuration structure to fill in			*/
					const char *);			/* Configuration file to read from			*/

#endif
