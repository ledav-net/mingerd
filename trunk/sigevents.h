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

#ifndef __SIGEVENTS_H
#define __SIGEVENTS_H

#include "common.h"

extern bool		terminate;				/* True when the process have to terminate	*/
extern bool		sig_event;				/* True when a handled signal was caught	*/
extern int		sig_num;				/* Signal received				*/

extern int		install_sigevents	(void);		/* Install signal handler & intr mask		*/

#endif
