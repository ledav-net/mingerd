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

#include "emails.h"

#include "common.h"
#include "mingerd.h"
#include "log.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct cache {
	char		*cache;
	char		*cursor;
	int		size;
	int		uselen;
	time_t		last;		} CACHE;

static CACHE	sysuser,
		aliases;

static int	increase_cache_buffer(CACHE *c)
{
	char	*t;

	if ( (t = realloc(c->cache, c->size + CACHE_EXT)) == NULL ) {
		printlog(stdlog, PL_ERR, "Error allocating memory: %m");
		return -1;
	}
	c->cache  = t;
	c->cursor = c->cache + c->uselen;
	c->size  += CACHE_EXT;

	return 0;
}

static int	reset_cache(CACHE *c)
{
	if ( ! c->size && increase_cache_buffer(c) < 0 )
		return -1;

	c->cache[0] = '|';
	c->cursor   = c->cache + 1;
	c->uselen   = 1;
	c->last     = 0;

	return 0;
}

static int	add_to_cache(CACHE *c, const char *u)
{
	int	len;

	len = c->uselen + strlen(u) + 1 /*'|'*/ + 1 /*'\0'*/;
	
	while ( len > c->size )
		if ( increase_cache_buffer(c) < 0 ) return -1;

	len = sprintf(c->cursor, "%s|", u);
	c->cursor += len;
	c->uselen += len;

	return 0;
}

int	load_sysuser_cache(const char *fpasswd)
{
	FILE	*f;
	char	b[256], *u, *i;

	if ( (f = fopen(fpasswd, "rt")) == NULL ) {
		printlog(stdlog, PL_ERR, "Error opening file %s for reading: %m", fpasswd);
		return -1;
	}

	if ( reset_cache(&sysuser) < 0 ) {
		fclose(f);
		return -1;
	}

	while ( fgets(b, sizeof(b), f) ) {
		trim(b);
		if ( ! b[0] || b[0] == '#' ) continue; /* Skip comments & empty lines */

		/* username:x:510:510::/home/username:/bin/bash	*/

		u = strtok(b, ":");	/* user */
		strtok(NULL, ":");	/* skipped field */
		i = strtok(NULL, ":");	/* we take only users >= 500 */

		if ( i == NULL ) {
			printlog(stdlog, PL_CRIT, "Parsing error on file %s !", fpasswd);
			return -1;
		}

		if ( atoi(i) >= 500 && add_to_cache(&sysuser, u) < 0 ) return -1;
	}

	fclose(f);

	return 0;
}

int	load_aliases_cache(const char *faliases)
{
	FILE	*f;
	char	b[256], *u;
	
	if ( (f = fopen(faliases, "rt")) == NULL ) {
		printlog(stdlog, PL_ERR, "Error opening file %s for reading: %m", faliases);
		return -1;
	}

	if ( reset_cache(&aliases) < 0 ) {
		fclose(f);
		return -1;
	}

	while ( fgets(b, sizeof(b), f) ) {
		trim(b);
		if ( ! b[0] || b[0] == '#' ) continue; /* Skip comments & empty lines */

		/* useralias: username */

		u = strtok(b, ":");

		if ( u == NULL ) {
			printlog(stdlog, PL_ERR, "Parsing error on file %s !", faliases);
			return -1;
		}
		
		if ( add_to_cache(&aliases, trim(u)) < 0 ) return -1;
	}

	fclose(f);

	return 0;
}

static int	search_user(const char *user)
{
	char	b[256];
	
	sprintf(b,"|%s|", user);
	if ( strstr(sysuser.cache, b)
	||   strstr(aliases.cache, b) )
		return ST_SUCCESS;
	
	return ST_EMAIL_NOT_FOUND;
}

bool	check_email(const char *em)
{
	if ( ! em )
		return false;
	if ( strcmp(em, "*") == 0 )	/* Special test case */
		return true;
	if ( strlen(em) >= 5 && ! strchr(em, ' ') && strchr(em, '@') && strchr(em, '.') )
		return true;
	return false;
}

int	check_email_active(const char *em)
{
	struct stat	s;
	char		user[32];
	int		l;

	if ( strcmp(em, "*") == 0 ) /* Special test case */
		return ST_SUCCESS;

	if ( stat(PASSWDF, &s) < 0 ) {
		printlog(stdlog, PL_ERR, "Cannot access %s: %m", PASSWDF);
		return -1;
	}
	if ( s.st_mtime > sysuser.last ) {
		if ( load_sysuser_cache(PASSWDF) < 0 ) return -1;
		sysuser.last = s.st_mtime;
	}

	if ( stat(ALIASESF, &s) < 0 ) {
		printlog(stdlog, PL_ERR, "Cannot access %s: %m", ALIASESF);
		return -1;
	}
	if ( s.st_mtime > aliases.last ) {
		if ( load_aliases_cache(ALIASESF) < 0 ) return -1;
		aliases.last = s.st_mtime;
	}

	l = strchr(em, '@') - em;
	strncpy(user, em, l);
	user[l] = 0;

	return search_user(strlwr(user));
}
