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

#include "log.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PL_DATETIME_SIZE	23

PLOG	*stdlog = NULL;

static const char months[12][4] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
static const char days[7][4]    = { "Sun","Mon","Tue","Wed","Thu","Fri","Sat" };
static const char priorities[8] = { 'E','A','C','e','W','N','I','D' };

void	printlog(PLOG *log, int priority, char *format, ...)
{
	va_list		ap;
	if ( priority <= log->priority ) {
		va_start(ap, format);
		vprintlog(log, priority, format, ap);
		va_end(ap);
	}
}

void	vprintlog(PLOG *log, int priority, char *format, va_list ap)
{
	char		msg[PL_MSG_MAXSIZE], *ptr;
	struct tm	*stm;
	time_t		tt;
	int		msg_length;

	if ( log ) {
		tt  = time(NULL);
		stm = localtime(&tt);

		/* We can use the value returned by sprintf to increment a pointer other than calculate lengths. */

		ptr = msg + sprintf(msg	, "%c %s, %s %2d %02d:%02d:%02d %s"
					, priorities[priority], days[stm->tm_wday], months[stm->tm_mon], stm->tm_mday
					, stm->tm_hour, stm->tm_min, stm->tm_sec, log->ident);

		if ( log->options & PL_PID )
			ptr += sprintf(ptr, "[%5d]: ", getpid());
		else {
			strcpy(ptr, ": ");
			ptr += 2;
		}

		vsnprintf(ptr, PL_MSG_MAXSIZE - (ptr-msg) - 1, format, ap);

		for ( ptr=msg ; *ptr ; ptr++ ) if (*ptr == '\n' || *ptr == '\t' || *ptr == '\r') *ptr = ' ';

		*ptr = '\n'; ptr++;
		*ptr = 0;

		msg_length = ptr - msg;

		write(log->fd, msg, msg_length);
		if ( (log->options & PL_PERROR) ) write(2, msg, msg_length);
	}
}

PLOG	*_startlog(int fd, char *file, char *ident, int options, int priority)
{
	int	ident_length, pid_length;
	PLOG	*log;

	if ( fd < 0 && (fd = open(file, O_CREAT | O_APPEND | O_WRONLY | ((options & PL_SYNC) ? O_SYNC : 0), 0640)) < 0 ) {
		return NULL;
	}

	if ( (log = (PLOG *)calloc(1, sizeof(PLOG))) == NULL ) {
		close(fd);
		errno = ENOMEM;
		return NULL;
	}

	log->fd = fd;
	log->options = options;
	log->priority = priority;

	if ( ident == NULL || ident[0] == 0 ) {
		log->ident = "";
		ident_length = 0;
	}
	else	ident_length = strlen(ident);

	if ( (options & PL_PID) ) pid_length = 9; /* "[%5d]: "	*/
	else			  pid_length = 2; /* ": "	*/

	if ( ident_length ) {
		if ( ident_length + pid_length >= PL_MSG_MAXSIZE - PL_DATETIME_SIZE - 1 ) {
			ident_length = PL_MSG_MAXSIZE - PL_DATETIME_SIZE - 1 - 1 - pid_length;
		}

		if( (log->ident = (char *)malloc(ident_length + 1)) == NULL ) {
			close(fd);
			free(log);
			errno = ENOMEM;
			return NULL;
		}else{
			strncpy(log->ident, ident, ident_length);
			log->ident[ident_length] = 0;
		}
	}

	return log;
}

void	stoplog(PLOG *log)
{
	if ( log ) {
		if ( log->ident[0] ) free(log->ident);
		close(log->fd);
		free(log);
	}
}
