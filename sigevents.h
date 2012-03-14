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

#ifndef __SIGEVENTS_H
#define __SIGEVENTS_H

#include "common.h"

extern bool		terminate;				/* True when the process have to terminate	*/
extern bool		sig_event;				/* True when a handled signal was caught	*/
extern int		sig_num;				/* Signal received				*/

extern int		install_sigevents	(void);
extern int		restore_sigevents	(void);
extern void		block_sigevents		(void);

#endif
