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

#include "digest.h"

#include "common.h"
#include "log.h"

#include <string.h>
#include <stdlib.h>
#include <beecrypt/md5.h>
#include <beecrypt/base64.h>

bool	check_digest(const char *digest, const char *secret, const char *em)
{
	char		buf[256], *b;
	byte		md5[17];
	md5Param	mp;
	bool		ret;
	int		len;

	if ( ! (digest && digest[0] == 'd' && digest[1] == '=') )
		return false;

	digest += 2; /* Skip "d=" */

	/* md5-base64 digest
	 *
	 * "password:email" hashed in md5 (binary value) and returned in base64 encoded form
	 */

	len = sprintf(buf, "%s:%s", secret, em);

	printlog(stdlog, PL_DEBUG, "Calculated pass phrase [%s] (%d)", buf, len);

	md5Reset(&mp);
	md5Update(&mp, (byte *)&buf, len);
	md5Digest(&mp, md5);
	md5[16] = 0;

	printlog(stdlog, PL_DEBUG, "Calculated md5 "
		"[%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x][%02x]"
		, (int)md5[0],(int)md5[1],(int)md5[ 2],(int)md5[ 3],(int)md5[ 4],(int)md5[ 5],(int)md5[ 6],(int)md5[ 7]
		, (int)md5[8],(int)md5[9],(int)md5[10],(int)md5[11],(int)md5[12],(int)md5[13],(int)md5[14],(int)md5[15]);

	b = rtrim(b64encode(&md5, 16));

	printlog(stdlog, PL_DEBUG, "Calculated b64 digest %p [%s]", b, b);

	ret = (strcmp(b, digest) == 0);
	free(b);

	return ret;
}
