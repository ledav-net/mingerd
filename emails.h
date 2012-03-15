/*
 * The Minger Email Address Verification Daemon
 * Copyright (C) 2012 David De Grave <david@ledav.net>
 *
 * http://sourceforge.net/projects/mingerd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License  
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef _EMAILS_H
#define _EMAILS_H

#include "common.h"

#define CACHE_EXT	32						/* Cache extend						*/

#define PASSWDF		"/etc/passwd"
#define ALIASESF	"/etc/aliases"

extern int			load_sysuser_cache(			/* Load the system users allowed to receive mails	*/
					const char *);			/* Passwd file to load					*/

extern int			load_aliases_cache(			/* Load all the system aliases				*/
					const char *);			/* Alias file to load					*/

extern bool			check_email(				/* Check the validity of an email address		*/
					const char *);			/* Email string to check				*/

extern int			check_email_active(			/* Check if the email account exist and is active	*/
					const char *);			/* Email string to check				*/

#endif
