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
#include "common.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _nv { char *name; int value; };

CONFIG		config = {
			4069,			/* Default port 			*/
			PL_NOTICE,		/* Default logging priority		*/
			false,			/* Anonymous requests not allowed	*/
			"0.0.0.0",		/* Default ip to listen on		*/
			""			/* No default for secret password	*/
		};

static int	read_string(char *value, void *dst)
{
	*(char **)dst = strdup(value);
	return 0;
}

static int	read_int(char *value, void *dst)
{
	if ( ! isnumber(value) ) return -1;
	*(int *)dst = atoi(value);
	return 0;
}

static int	read_bool(char *value, void *dst)
{
	static const struct _nv tab[] = {
		{ "yes", true  }, { "true",  true  }, { "1", true  },
		{ "no",  false }, { "false", false }, { "0", false }};

	int	i;

	strlwr(value);

	for ( i=0 ; i < numberof(tab) ; i++ )
		if ( strcmp(tab[i].name, value) == 0 ) {
			*(bool *)dst = (bool)tab[i].value;
			break;
		}

	if ( i == numberof(tab) ) return -1;
	return 0;
}

static int	read_loglevel(char *value, void *dst)
{
	static const struct _nv	tab[] = {
		{ "debug",     PL_DEBUG   }, { "7", PL_DEBUG   },
		{ "info",      PL_INFO    }, { "6", PL_INFO    },
		{ "notice",    PL_NOTICE  }, { "5", PL_NOTICE  },
		{ "warning",   PL_WARNING }, { "4", PL_WARNING },
		{ "error",     PL_ERR     }, { "3", PL_ERR     },
		{ "critical",  PL_CRIT    }, { "2", PL_CRIT    },
		{ "alert",     PL_ALERT   }, { "1", PL_ALERT   },
		{ "emergency", PL_EMERG   }, { "0", PL_EMERG   }};

	int	i;

	strlwr(value);

	for ( i=0 ; i < numberof(tab) ; i++ )
		if ( strcmp(tab[i].name, value) == 0 ) {
			*(int *)dst = tab[i].value;
			break;
		}

	if ( i == numberof(tab) ) return -1;
	return 0;
}

int	read_config(CONFIG *conf, const char *fn)
{
	static const CONFITEM	confitems[] = {
		{ "listenip", 		read_string,		&config.listenip	},
		{ "listenport",		read_int,		&config.listenport	},
		{ "logpriority",	read_loglevel,		&config.logpriority	},
		{ "anonymous",		read_bool,		&config.anonymous	},
		{ "secret",		read_string,		&config.secret		}};

	FILE	*c;
	int	i;
	char	buf[256], *v;

	if ( (c = fopen(fn, "rt")) == NULL )
		return -1;

	while ( fgets(buf, sizeof(buf), c) ) {

		trim(buf);

		if ( buf[0] == '#' || buf[0] == 0 ) continue; /* Skip comments & empty lines */

		if ( (v = strchr(buf, '=')) == NULL )
			return -1;

		*(v++) = 0;
		rtrim(buf);
		ltrim(v);

		for ( i=0 ; i < numberof(confitems) ; i++ )
			if ( strcmp(confitems[i].name, buf) == 0
			&&   confitems[i].handler(v, confitems[i].value) < 0 )
				return -1;
	}

	fclose(c);

	return 0;
}
