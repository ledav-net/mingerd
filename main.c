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

#include "config.h"
#include "log.h"
#include "init.h"
#include "sigevents.h"
#include "main.h"
#include "emails.h"
#include "digest.h"
#include "mingerd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

static int	replyto(int sockfd, const struct sockaddr *sockaddr, socklen_t sockaddrlen, const char *em, int status)
{
	int	len;
	char	resp[256];

	if ( em ) len = snprintf(resp, sizeof(resp), "%s %d", em, status);
	else	  len = snprintf(resp, sizeof(resp), "%d", status);

	printlog(stdlog, PL_DEBUG, "Sending [%s] (%d bytes)", resp, len);

	return sendto(sockfd, resp, len, MSG_NOSIGNAL, sockaddr, sockaddrlen);
}

int	mingerd_process(const CONFIG *conf)
{
	struct	hostent		*he;
	int			sockfd, ret;
	struct	sockaddr_in	socklocal, sockremote;
	socklen_t		sockremotelen;
	char			buf[1024];

	if ( (he = gethostbyname(conf->listenip)) == NULL ) {
		printlog(stdlog, PL_ERR, "Trying to resolve %s: %m", conf->listenip);
		return -1;
	}

	memset(&socklocal, 0, sizeof(socklocal));

	socklocal.sin_family = AF_INET;
	socklocal.sin_port   = htons(conf->listenport);
	memcpy(&socklocal.sin_addr.s_addr, he->h_addr, he->h_length);

	printlog(stdlog, PL_DEBUG, "Opening Socket ...");

	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
		printlog(stdlog, PL_ERR, "Trying to open socket %s:%s: %m", conf->listenip, conf->listenport);
		return -2;
	}

	printlog(stdlog, PL_DEBUG, "Binding ...");

	if ( bind(sockfd, (struct sockaddr *)&socklocal, sizeof(socklocal)) ) {
		printlog(stdlog, PL_ERR, "Trying to bind to socket: %m");
		close(sockfd);
		return -3;
	}

	printlog(stdlog, PL_INFO, "Ready to answer requests ...");

	sockremotelen = sizeof(sockremote);
	ret = 0;

	while ( ! terminate ) {
		int	len, r;
		char	*em, *digest, *taglist;

		memset(&sockremote, 0, sizeof(sockremote));

		len = recvfrom(sockfd, buf, sizeof(buf)-1, MSG_NOSIGNAL, (struct sockaddr *)&sockremote, &sockremotelen);

		if ( len < 0 ) {
			if ( ! terminate ) {
				printlog(stdlog, PL_ERR, "Trying to receive: %m");
				ret = 1;
			}
			break;
		}
		if ( ! len ) {
			ret = 1;
			break;
		}

		buf[len] = 0;

		printlog(stdlog, PL_DEBUG, "Received [%s] (%d bytes) from %s", buf, len, inet_ntoa(sockremote.sin_addr));

		/* query-string = mailbox [SP %x64 "=" digest] [SP tag-list]	*/
		/* ex: david@ledav.net d=zIi/VNDcyYv5nBHx/OwSRQ==		*/

		em      = strtok(buf,  " "); /* Get the email address		*/
		digest  = strtok(NULL, " "); /* Get the digest			*/
		taglist = strtok(NULL, "");  /* Get all the remaining tags	*/

		printlog(stdlog, PL_DEBUG, "em[%s] digest[%s] taglist[%s]", em, digest, taglist);

		if ( ! check_email(em) ) {
			r = replyto(sockfd, (struct sockaddr *)&sockremote, sockremotelen, em, ST_BAD_REQUEST);
		}else
		if ( ! (conf->anonymous || check_digest(digest, conf->secret, em)) ) {
			r = replyto(sockfd, (struct sockaddr *)&sockremote, sockremotelen, em, ST_ACCESS_INVALID);
		}
		else	r = replyto(sockfd, (struct sockaddr *)&sockremote, sockremotelen, em, check_email_active(em));

		if ( r < 0 ) {
			printlog(stdlog, PL_ERR, "Trying to send: %m");
			ret = 1;
			break;
		}
	}

	printlog(stdlog, PL_DEBUG, "Closing socket ...");
	close(sockfd);

	return ret;
}
