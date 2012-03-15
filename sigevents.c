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

#include "sigevents.h"
#include "common.h"
#include "log.h"

#include <signal.h>

bool	sig_event;
bool	terminate;
int	sig_num;

static struct sigaction	oldsigterm,
			oldsigint,
			oldsigquit;

static sigset_t	sigset;

void	event_handler(int sig)
{
	sig_num   = sig;
	sig_event = true;

	if ( sig == SIGTERM || sig == SIGINT || sig == SIGQUIT ) {
		terminate = true;
	}
	printlog(stdlog, PL_DEBUG, "--- got signal %d", sig);
}

int	install_sigevents(void)
{
	struct sigaction sigopt;

	terminate  =
	sig_event  =
	sig_num    = 0;

	memset(&sigopt, 0, sizeof(sigopt));
	sigopt.sa_handler = event_handler;

	sigaction(SIGTERM, &sigopt, &oldsigterm);
	sigaction(SIGINT,  &sigopt, &oldsigint);
	sigaction(SIGQUIT, &sigopt, &oldsigquit);

	sigemptyset(&sigset);
	sigaddset(&sigset, SIGTERM);
	sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGQUIT);

	return 0;
}

int	restore_sigevents(void)
{
	sigaction(SIGTERM, &oldsigterm, NULL);
	sigaction(SIGINT,  &oldsigint,  NULL);
	sigaction(SIGQUIT, &oldsigquit, NULL);

	return 0;
}

void	block_sigevents(void)
{
	sigprocmask(SIG_BLOCK, &sigset, NULL);
}

void	unblock_sigevents(void)
{
	sigprocmask(SIG_UNBLOCK, &sigset, NULL);
}
