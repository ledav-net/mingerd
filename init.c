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
 
#include "init.h"
#include "sigevents.h"
#include "common.h"
#include "log.h"
#include "main.h"
#include "emails.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <errno.h>

static int	init_pid = 0;
static bool	debug    = false;

static int	makepid(char *file)
{
	int	fd, pidtmp, readlen;
	char	pid[7];

	if ( (fd = open(file, O_CREAT|O_RDWR, 0644)) < 0 ) return -1;

	if ( flock(fd, LOCK_NB|LOCK_EX) < 0 ) {
		if ( (readlen = read(fd, pid, sizeof(pid)-1)) < 0 ) {
			close(fd);
			return -1;
		}

		pid[readlen] = 0;

		if ( ! (pidtmp = atoi(pid)) ) {
			pidtmp = -1;
			errno = ENODATA;
		}

		close(fd);

		return pidtmp;
	}

	sprintf(pid, "%i\n", getpid());
	write(fd, pid, strlen(pid));

	/* FD is kept open and locked ...	*/

	return 0;
}

static pid_t	startdaemon(void)
{
	register int	i;
	pid_t		cid;

	if ( (cid = fork()) < 0 ) return -1;	/* ERROR	*/
	else
	if ( cid > 0 ) exit(0);			/* PARENT	*/

	/* CHILD */

	setsid();
	chdir("/");
	umask(0);

	for ( i=0 ; i < NOFILE ; i++ ) close(i);
	errno = 0;

	#ifdef	SIGTTOU
	signal(SIGTTOU, SIG_IGN);
	#endif

	#ifdef	SIGTTIN
	signal(SIGTTIN, SIG_IGN);
	#endif

	#ifdef	SIGTSTP
	signal(SIGTSTP, SIG_IGN);
	#endif

	return getpid();
}

static void     remove_pid(void)
{
	if ( getpid() == init_pid ) unlink(PID_FILE);
}

static void	print_help(const char *proc)
{
	printf(
	P_NAME_LONG" v"P_VERSION" made by David De Grave.\n"
	"\n"
	"Usage:\n"
        " %s [-hd]\n"
        "\n"
        "Options:\n"
	" -h         Display this help screen.\n"
        " -d         Turn the debugging mode on (no daemonize + more logging on terminal).\n"
        "\n", proc);
}

static void	process_parameters(int argc, char **argv)
{
	char	opt;
        
	while ( (opt = getopt(argc, argv, "dh")) > 0 ) {
        	switch ( opt ) {
			case 'd':
				debug = true;
				config.logpriority = PL_DEBUG;
				break;

			default:
				print_help(argv[0]);
				exit(0);
		}
	}
}

int	main(int argc, char **argv)
{
	int	r;

	if ( read_config(&config, CFG_FILE) < 0 ) {
		printf("Error reading configuration file "CFG_FILE" !\n");
		exit(10);
	}

	if ( argc > 1 ) process_parameters(argc, argv);

	if ( ! debug ) {
		if ( (init_pid = startdaemon()) < 0 ) {
			perror("startdaemon()");
			exit(11);
		}
		if ( (stdlog = startlog(LOG_FILE, P_NAME_SHORT, PL_PID, config.logpriority)) == NULL ) {
			/* BUG: In case logfile cannot be open, no errors are shown due to the previous
			 *      call to startdaemon() who closes all the file descriptors
			 */
			perror("startlog()");
			exit(12);
		}
	}else{
		init_pid = getpid();
		stdlog   = restartlog(1, P_NAME_SHORT, PL_PID, config.logpriority);
	}

	if ( (r = makepid(PID_FILE)) < 0 ) {
		printlog(stdlog, PL_ERR, "Error creating pid file ...");
		exit(13);
	}else
	if ( r > 0 ) {
		printlog(stdlog, PL_ERR, P_NAME_SHORT" is already running ... (pid %d)", r);
		exit(14);
	}

	printlog(stdlog, PL_NOTICE, P_NAME_SHORT" v"P_VERSION" is running ...");

	atexit(remove_pid);
	install_sigevents();

	if ( load_sysuser_cache(PASSWDF)  < 0
	||   load_aliases_cache(ALIASESF) < 0 ) {
		printlog(stdlog, PL_ERR, "Error loading caches !");
		exit(15);
	}

	printlog(stdlog, PL_DEBUG, "Active config: "
		"listenip[%s] listenport[%d] logpriority[%d] anonymous[%d] secret[%s]"
		, config.listenip, config.listenport, config.logpriority, config.anonymous
		, config.secret);

	do {
		r = mingerd_process(&config);
	}
	while ( r > 0 );

	printlog(stdlog, PL_NOTICE, P_NAME_SHORT" ended ...");

	return r < 0 ? -r : 0;
}
