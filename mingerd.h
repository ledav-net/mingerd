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

#ifndef _MINGERD_H
#define _MINGERD_H

/* status
 * 
 * The following status codes are defined:
 *        
 * 0 - invalid request (for example, malformed query string)
 * 1 - access denied (for example, query from unauthorized IP)
 * 2 - bad or missing credentials (returned when anonymous
 *     mode is disabled and no credentials were provided in the
 *     query string or when the credentials themselves are
 *     invalid)
 * 3 - email address does not exist
 * 4 - email address exists but can not receive mail (for example,
 *     the account associated with the email address has exceeded
 *     local storage constraints or it is otherwise disabled due
 *     to local policy)
 * 5 - email address exists and is active (able to receive mail)
 */

#define ST_BAD_REQUEST		0
#define ST_ACCESS_DENIED	1
#define ST_ACCESS_INVALID	2
#define ST_EMAIL_NOT_FOUND	3
#define ST_EMAIL_NOT_ACTIVE	4
#define ST_SUCCESS		5

#endif
