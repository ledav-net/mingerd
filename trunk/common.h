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

#ifndef __COMMON_H_
#define __COMMON_H_

#include <string.h>

#ifndef numberof
#define numberof(v)	(sizeof(v)/sizeof(v[0]))
#endif

#ifndef	__HAVE_BOOL
#define	__HAVE_BOOL
typedef	enum	{ false = 0, true }		bool;
#endif

#ifndef BOOLNAME
#define BOOLNAME(b)	((b) ? "true" : "false")
#endif

#ifndef SWITCH
#define SWITCH(a,b,c)	(((a) == (b)) ? ((a) = (c)) : ((a) = (b)))
#endif

#define MIN2SEC(m)	((m)*60)		/* Min  -> seconds	*/
#define MIN2HOUR(m)	((m)/60)		/* Min  -> hours	*/
#define MIN2DAY(m)	(MIN2HOUR(m)/24)	/* Min  -> days		*/

#define HOUR2SEC(h)	(MIN2SEC(h)*60)		/* Hour -> seconds	*/
#define HOUR2MIN(h)	((h)*60)		/* Hour -> minutes	*/
#define HOUR2DAY(h)	((h)/24)		/* Hour -> days	*/

#define DAY2SEC(d)	(HOUR2SEC(d)*24)	/* Day  -> seconds	*/
#define DAY2MIN(d)	(HOUR2MIN(d)*24)	/* Day  -> minutes	*/
#define DAY2HOUR(d)	((d)*24)		/* Day  -> hours	*/

#define MTOU(v)		((v)*1000)		/* mSec -> µSec		*/
#define MTOS(v)		((v)/1000)		/* mSec -> Sec  	*/

#define STOU(v)		((v)*1000000)		/* Sec  -> µSec 	*/
#define STOM(v)		((v)*1000)		/* Sec  -> mSec 	*/

#define UTOM(v)		((v)/1000)		/* µSec -> mSec 	*/
#define UTOS(v)		((v)/1000000)		/* µSec -> Sec  	*/

#ifndef checkbits
#define checkbits(a,b)          (((a) & (b)) == (b))
#endif

#ifndef checknbits
#define checknbits(a,b)		(((a) & (b)) == 0)
#endif

#ifndef setbits
#define setbits(a,b)		((a) |= (b))
#endif

#ifndef unsetbits
#define unsetbits(a,b)		((a) &= ~(b))
#endif

#ifndef strmcpy
#define strmcpy(dst,src,sze)	((dst)[(sze)-1] = 0, strncpy((dst), (src), (sze)-1))
#endif

#define strtrim(str, c)	strrtrim(strltrim((str), (c)), (c))	/* Macro to trim both right and left characters			*/
#define rtrim(str)	strrtrim((str), NULL)
#define ltrim(str)	strltrim((str), NULL)
#define trim(str)	strtrim((str), NULL)

extern char *		strltrim(				/* Left trim specified characters				*/
				char *,				/* string to trim						*/
				const char *);			/* character list to remove (NULL = all space chars)		*/
								/* Return: Input string						*/

extern char *		strrtrim(				/* Remove specied characters on the right of the string		*/
				char *,				/* string to trim						*/
				const char *);			/* character list to remove (NULL = all space chars)		*/
								/* Return: Input string						*/

extern char *		strlwr(					/* Put a string in lowercase					*/
				char *);			/* string to work on						*/

extern bool		isnumber(				/* Check if a string is a number				*/
				const char *);			/* string to check						*/

#endif
