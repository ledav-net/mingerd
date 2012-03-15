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

#ifndef _LOG_H
#define _LOG_H

#include <stdarg.h>

typedef	struct
{
	int	fd,
		options,
		priority;
	char	*ident;
} PLOG;

#define PL_PID			1			/* Add the pid of the actual process on each lines.			*/
#define PL_PERROR		2			/* log also the message to stderr.					*/
#define PL_SYNC			4			/* Return from printlog when message is really saved.			*/

#define PL_EMERG		0			/* Emergecy: System is unusable. Complete restart/checks must be done.	*/
#define PL_ALERT		1			/* Alert:    Process can't continue working. Manual action must be done.*/
#define PL_CRIT			2			/* Crit:     Process was entered in an unknown state.			*/
#define PL_ERR			3			/* Error:    Error is returned from function, etc...			*/
#define PL_WARNING		4			/* Warning:  Message have to be checked further...			*/
#define PL_NOTICE		5			/* Notice:   Message could be important/interresting to know.		*/
#define PL_INFO			6			/* Info:     Message is symply informational...				*/
#define PL_DEBUG		7			/* Debug:    Message is for debugging informations only.		*/

/*
 *	Log format:
 *
 *	D Fri, Dec 20 18:11:04 appname[ 1422]: Actionid value=0xFFFFFFFFFF
 *	^ ^                    ^      ^        ^
 *	| |                    |      |        `- Message Content
 *	| |                    |      `- Optional PID
 *	| |                    `- Identifer Name
 *	| `- Timestamp
 *	`- Level (Sorted list: EACeWNID. Here, it's a Debug line)
 */

extern PLOG			*stdlog;				/* Default PLOG * structure				*/

#define PL_MSG_MAXSIZE		1024			/* Maximum message size printlog can handle. The rest is truncated.     */

#define startlog(file,i,o,p)	_startlog(-1,(file),(i),(o),(p))	/* start/open a new logfile.				*/
#define restartlog(fd,i,o,p)	_startlog((fd),NULL,(i),(o),(p))	/* reuse an already opened file by its file descriptor.	*/

extern PLOG *			_startlog(				/* Open logfile and return the PLOG element.		*/
					  int fd,			/* If >= 0, simply fillin PLOG without open 'file'.	*/
					  char *file,			/* Open this file for output.				*/
					  char *ident,			/* Process identifier.					*/
					  int options,			/* Options. see the above defines.			*/
					  int priority);		/* Default priority to log				*/

extern void			printlog(				/* Log this message to logfile.				*/
					 PLOG *log,			/* PLOG structure to use.				*/
					 int priority,			/* Level of importance of this message.			*/
					 char *format,			/* Format string. 'printf' like.			*/
					 ...);				/* arguments to Format string.				*/

extern void			vprintlog(				/* Same as printlog, but arguments to 'format' string	*/
					PLOG *,				/* are passed via the 'va_list' argument, to let you	*/
					int,				/* the abillity to interface printlog depending on your	*/
					char *,				/* needs ...						*/
					va_list);

extern void			stoplog(PLOG *log);			/* Stop/Free corresponding logfile.			*/

#endif
