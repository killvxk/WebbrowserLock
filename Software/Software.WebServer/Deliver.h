/**
 **	File ......... Deliver.h
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
#ifndef _DELIVER_H
#define _DELIVER_H

#include <string>
#include <Function.Internal.SocketsControl/Sockets/TcpSocket.h>


class Deliver
{
public:
	Deliver(TcpSocket&,
		const std::string& host,
		const std::string& protocol,
		const std::string& method,
		const std::string& url);
	~Deliver();

	void Run();
	void url_this(const std::string& url_in,
		std::string& host,int& port,std::string& url,std::string& file,std::string& ext);

private:
	TcpSocket& m_socket;
	std::string m_host;
	std::string m_protocol; // http / https
	std::string m_method;
	std::string m_url;
};




#endif // _DELIVER_H
