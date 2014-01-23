/**
 **	File ......... Deliver.cpp
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

#include <Function.Internal.SocketsControl/Sockets/Parse.h>
#include "ServerHandler.h"
#include "Deliver.h"

#pragma comment(lib,"..\\..\\LIB\\Function\\Function.Internal.SocketsControl.lib")



Deliver::Deliver(TcpSocket& s,const std::string& h,const std::string& p,const std::string& m,const std::string& u)
:m_socket(s)
,m_host(h)
,m_protocol(p)
,m_method(m)
,m_url(u)
{
	if (m_url.substr(m_url.size() - 1) == "/")
	{
		m_url += "index.html";
	}
}


Deliver::~Deliver()
{
}

#include <string>
#include "Socket.h" 
void Deliver::Run()
{
	// use Socket::MasterHandler because we're now in a detached socket
	// Handler() will return the local sockethandler driving the socket
	// MasterHandler() returns the original sockethandler
	ServerHandler& h = static_cast<ServerHandler&>(m_socket.MasterHandler());
	std::string url = m_protocol + "://" + m_host + m_url;
	std::string host;
	int port;
	std::string url_ut;
	std::string file;
	std::string ext;
	url_this(url, host, port, url_ut, file, ext);
	//std::string mime_type =  h.GetMimetype(ext);

// 	m_socket.Send("HTTP/1.0 404 Not Found\n");
// 	m_socket.Send("Server: " + h.GetString("server/identity") + "\n");
// 	m_socket.Send("Content-type: text/html\n");
// 	m_socket.Send("\n<html><body><h1>404 Not Found - Resource not available</h1></html>\n");


	try 
	{
		SocketClient s("www.baidu.com", 80);

		s.SendLine("GET / HTTP/1.0");
		s.SendLine("Host: www.baidu.com");
		s.SendLine("");
		while (1) 
		{
			std::string l = s.ReceiveLine();
			if (l.empty()) break;
			m_socket.Send(l);
		}

	} 
	catch (...) 
	{
		m_socket.SetCloseAndDelete();
	}


	m_socket.SetCloseAndDelete();

	/*

	Database *db = h.GetDatabase();
	Query q(*db);
	std::string safe_host = db -> safestr(m_host);
	std::string safe_protocol = db -> safestr(m_protocol);
	std::string safe_method = db -> safestr(m_method);
	std::string safe_url = db -> safestr(m_url);
	char sql[1000];
	sprintf(sql,"select * from url where host='%s' and protocol='%s' and method='%s' and url='%s'",
	safe_host.c_str(),
	safe_protocol.c_str(),
	safe_method.c_str(),
	safe_url.c_str());
	db::Url xx(db,sql);
	if (!xx.num)
	{
	xx.host = m_host;
	xx.protocol = m_protocol;
	xx.method = m_method;
	xx.url = m_url;
	xx.mime_type = mime_type;
	}
	xx.access_count++;
	xx.save();
*/
	// quick'n dirty
/*
HTTP/1.1 200 OK
Date: Wed, 07 Apr 2004 13:15:18 GMT
Server: Apache/1.3.26 (Unix)
Last-Modified: Mon, 22 Mar 2004 03:11:56 GMT
ETag: "1e70c6-27cb-405e597c"
Accept-Ranges: bytes
Content-Length: 10187
Connection: close
Content-Type: text/html

// */
// 	if (regel.num)
// 	{
// 		switch (regel.typ)
// 		{
// 		case 1: // File
// 			{
// 				std::string filename = regel.path + m_url;
// 				FILE *fil = fopen(filename.c_str(),"rb");
// 				if (fil)
// 				{
// 					char buf[4096];
// 					size_t n;
// 					m_socket.Send("HTTP/1.0 200 OK\n");
// 					m_socket.Send("Server: " + h.GetString("server/identity") + "\n");
// 					if (mime_type.size())
// 					{
// 						std::string str = "Content-type: " + mime_type;
// 						m_socket.Send(str + "\n");
// 					}
// 					m_socket.Send("Connection: close\n");
// 					m_socket.Send("\n");
// 					n = fread(buf, 1, 4096, fil);
// 					while (n > 0)
// 					{
// //printf("read %d bytes\n",n);
// 						m_socket.SendBuf(buf, n);
// 						//
// 						n = fread(buf, 1, 4096, fil);
// 					}
// 					fclose(fil);
// 					m_socket.SetCloseAndDelete();
// 				}
// 				else
// 				{
// 					m_socket.Send("HTTP/1.0 404 Not Found\n");
// 					m_socket.Send("Server: " + h.GetString("server/identity") + "\n");
// 					m_socket.Send("Content-type: text/html\n");
// 					m_socket.Send("\n<html><body><h1>404 Not Found</h1></html>\n");
// 					m_socket.SetCloseAndDelete();
// 				}
// 			}
// 			break;
// 		case 2: // Execute
// 			break;
// 		default:
// 			m_socket.Send("HTTP/1.0 404 Not Found\n");
// 			m_socket.Send("Server: " + h.GetString("server/identity") + "\n");
// 			m_socket.Send("Content-type: text/html\n");
// 			m_socket.Send("\n<html><body><h1>404 Not Found - Resource not available</h1></html>\n");
// 			m_socket.SetCloseAndDelete();
// 		}
// 	}
// 	else
// 	{
// 		m_socket.Send("HTTP/1.0 404 Not Found\n");
// 		m_socket.Send("Server: " + h.GetString("server/identity") + "\n");
// 		m_socket.Send("Content-type: text/html\n");
// 		m_socket.Send("\n<html><body><h1>404 Not Found - Resource not available</h1></html>\n");
// 		m_socket.SetCloseAndDelete();
// 	}
}


void Deliver::url_this(const std::string& url_in,std::string& host,int& port,std::string& url,std::string& file,std::string& ext)
{
	Parse pa(url_in,"/");
	pa.getword(); // http
	host = pa.getword();
	if (strstr(host.c_str(),":"))
	{
		Parse pa(host,":");
		pa.getword(host);
		port = pa.getvalue();
	}
	else
	{
		port = 80;
	}
	url = "/" + pa.getrest();
	{
		Parse pa(url,"/");
		std::string tmp = pa.getword();
		while (tmp.size())
		{
			file = tmp;
			tmp = pa.getword();
		}
	}
	{
		Parse pa(file,".");
		std::string tmp = pa.getword();
		while (tmp.size())
		{
			ext = tmp;
			tmp = pa.getword();
		}
	}
} // url_this


