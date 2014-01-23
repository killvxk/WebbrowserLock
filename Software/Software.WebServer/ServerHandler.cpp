/**
 **	File ......... ServerHandler.cpp
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
//#include <stdio.h>

#include "ServerHandler.h"




ServerHandler::ServerHandler(const std::string& filename)
:SocketHandler()
,m_config(filename)
,m_mime("mime.types")
{
}


ServerHandler::~ServerHandler()
{
}


int ServerHandler::GetInt(const std::string& path)
{
	return 0;
}


std::string ServerHandler::GetString(const std::string& path)
{
	return "";
}


bool ServerHandler::GetBoolean(const std::string& path)
{
	fprintf(stderr,"config path not found: %s\n",path.c_str());
	return false;
}


std::string ServerHandler::GetMimetype(const std::string& ext)
{
	return m_mime.GetMimeFromExtension(ext);
}


