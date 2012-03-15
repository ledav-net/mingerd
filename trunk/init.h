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

#ifndef _INIT_H
#define _INIT_H

#include "config.h"
#include "log.h"

#define P_VERSION	"1.0.0"
#define P_NAME_LONG	"Minger Email Address Verification Daemon"
#define P_NAME_SHORT	"mingerd"

#define PID_FILE	CF_D_RUN "/" P_NAME_SHORT ".pid"
#define LOG_FILE	CF_D_LOG "/" P_NAME_SHORT ".log"
#define CFG_FILE	CF_D_ETC "/" P_NAME_SHORT ".conf"

#endif
