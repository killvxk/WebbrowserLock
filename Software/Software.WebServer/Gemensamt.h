/**
 **	File ......... Gemensamt.h
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
#ifndef _GEMENSAMT_H
#define _GEMENSAMT_H

#include <string>
#include <Function.Internal.SocketsControl/Sockets/Parse.h>


class Gemensamt
{
public:
	Gemensamt() {}

	void SetServer(const std::string& x) { m_server = x; }
	std::string GetServer() {
		Parse pa(m_server,":");
		std::string str = pa.getword();
		return str;
	}
	void SetUserAgent(const std::string& x) { m_user_agent = x; }
	std::string GetUserAgent() { return m_user_agent; }
	void SetHttpReferer(const std::string& x) { m_http_referer = x; }
	std::string GetHttpReferer() { return m_http_referer; }

private:
	std::string m_server;
	std::string m_user_agent;
	std::string m_http_referer;
};




#endif // _GEMENSAMT_H
