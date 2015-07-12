/*
 *   IRC - Internet Relay Chat, src/modules/out.c
 *   (C) 2004 The UnrealIRCd Team
 *
 *   See file AUTHORS in IRC package for additional names of
 *   the programmers.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "config.h"
#include "struct.h"
#include "common.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
#include "proto.h"
#include "channel.h"
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include "h.h"
#ifdef _WIN32
#include "version.h"
#endif

DLLFUNC int m_locops(aClient *cptr, aClient *sptr, int parc, char *parv[]);

#define MSG_LOCOPS 	"LOCOPS"	

ModuleHeader MOD_HEADER(m_locops)
  = {
	"m_locops",
	"$Id$",
	"command /locops", 
	"3.2-b8-1",
	NULL 
    };

MOD_INIT(m_locops)
{
	CommandAdd(modinfo->handle, MSG_LOCOPS, m_locops, 1, M_USER);
	MARK_AS_OFFICIAL_MODULE(modinfo);
	return MOD_SUCCESS;
}

MOD_LOAD(m_locops)
{
	return MOD_SUCCESS;
}

MOD_UNLOAD(m_locops)
{
	return MOD_SUCCESS;
}

/*
** m_locops (write to opers who are +g currently online *this* server)
**      parv[1] = message text
*/
DLLFUNC CMD_FUNC(m_locops)
{
	char *message;

	message = parc > 1 ? parv[1] : NULL;

	if (BadPtr(message))
	{
		sendto_one(sptr, err_str(ERR_NEEDMOREPARAMS),
		    me.name, sptr->name, "LOCOPS");
		return 0;
	}
	if (MyClient(sptr) && !ValidatePermissionsForPath("chat:locops",sptr,NULL,NULL,NULL))
	{
		sendto_one(sptr, err_str(ERR_NOPRIVILEGES), me.name, sptr->name);
		return 0;
	}
	sendto_umode(UMODE_OPER, "from %s: %s", sptr->name, message);
	return 0;
}
