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

#include "common.h"

#include <string.h>
#include <ctype.h>

static const char	cSpace[] = " \t\n\r\f\v";

char *	strltrim(char *b, const char *clist)
{
	char *e = b;

	if ( ! clist ) clist = cSpace;

	if ( strchr(clist, *e) ) {
		do {
			if ( ! *e ) break;
			e++;
		} while ( strchr(clist, *e) );
		strcpy(b, e);
	}

	return b;
}

char *	strrtrim(char *b, const char *clist)
{
	char	*e;

	if ( ! clist ) clist = cSpace;

	e = b + strlen(b);
	while ( e > b && strchr(clist, *(e-1)) ) e--;
	*e = 0;

	return b;
}

char *	strlwr(char *b)
{
	register char *r;
	for ( r = b ; *r ; r++ ) *r = tolower(*r);
	return b;
}

bool	isnumber(const char *str)
{
	if ( ! *str ) return false;
	while ( isdigit(*str) ) str++;
	return ! *str;
}
