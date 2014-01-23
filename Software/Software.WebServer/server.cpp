/**
 **	File ......... server.cpp
 **	Published ....  2004-07-13
 **	Author ....... grymse@alhem.net
**/
/*
Copyright (C) 2004  Anders Hedstrom

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "ServerHandler.h"
#include <Function.Internal.SocketsControl/Sockets/ListenSocket.h>
#include "ServerSocket.h"


int main3()
{
	ServerHandler h("config.xml");
	ListenSocket<ServerSocket> ll_open(h);
	ListenSocket<ServerSocket> ll_secure(h);

	if (!h.GetBoolean("open/disable"))
	{
		if (ll_open.Bind(h.GetString("open/bind"),h.GetInt("open/port"),h.GetInt("open/queue_size")))
		{
			return -1;
		}
		h.Add(&ll_open);
	}
	else
	{
		printf("open server disabled\n");
	}
	if (!h.GetBoolean("secure/disable"))
	{
		if (ll_secure.Bind(h.GetString("secure/bind"),h.GetInt("secure/port"),h.GetInt("secure/queue_size")))
		{
			return -1;
		}
		h.Add(&ll_secure);
	}
	else
	{
		printf("secure server disabled\n");
	}
	bool quit = false;
	while (!quit)
	{
		h.Select(1,0);
	}
}


