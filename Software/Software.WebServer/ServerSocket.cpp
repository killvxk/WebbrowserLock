/**
 **	File ......... ServerSocket.cpp
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

#include "Deliver.h"
#include "ServerSocket.h"
#include "ServerHandler.h"

#define DEB(x) x


ServerSocket::ServerSocket(ISocketHandler& h)
:HTTPSocket(h)
,Gemensamt()
{
}


ServerSocket::~ServerSocket()
{
}


void ServerSocket::OnFirst()
{
	//printf("IsRequest: %s\n",IsRequest() ? "YES" : "NO");
	if (IsRequest())
	{
		printf(" Method: %s\n",GetMethod().c_str());
		printf(" URL: %s\n",GetUrl().c_str());
		printf(" Http version: %s\n",GetHttpVersion().c_str());
	}

	//printf("IsResponse: %s\n",IsResponse() ? "YES" : "NO");
	if (IsResponse())
	{
		//printf(" Http version: %s\n",GetHttpVersion().c_str());
		//printf(" Status: %s\n",GetStatus().c_str());
		//printf(" Status text: %s\n",GetStatusText().c_str());
	}

}


void ServerSocket::OnHeader(const std::string& key,const std::string& value)
{
	if (!strcasecmp(key.c_str(),"host"))
	{
		m_strQuestHand.append(key);
		m_strQuestHand.append(": ");
		m_strQuestHand.append("www.baidu.com");
		m_strQuestHand.append("\r\n");
		return ;
	}
	m_strQuestHand.append(key);
	m_strQuestHand.append(": ");
	m_strQuestHand.append(value);
	m_strQuestHand.append("\r\n");
	//printf("OnHeader(): %s: %s\n",key.c_str(),value.c_str());
	if (!strcasecmp(key.c_str(),"host"))
	{
		SetServer(value);
	}
	else
	if (!strcasecmp(key.c_str(),"user-agent"))
	{
		SetUserAgent(value);
	}
	else
	if (!strcasecmp(key.c_str(),"referer"))
	{
		SetHttpReferer(value);
	}
}


void ServerSocket::OnData(const char *,size_t)
{
}


void ServerSocket::OnHeaderComplete()
{
	if (!Detach())
	{
DEB(		printf("Detach() failed\n");)
		SetCloseAndDelete();
	}
}


#include <string>
#include "Socket.h" 

void ServerSocket::OnDetached()
{
	printf(m_strQuestHand.c_str());
	try 
	{
		std::string strURL = ("GET ");
		strURL.append(GetUrl());
		strURL.append(GetHttpVersion());
		SocketClient s("www.baidu.com", 80);
		s.SendLine("GET / HTTP/1.0");
		s.SendBytes(m_strQuestHand);
// 		s.SendLine("GET / HTTP/1.0");
// 		s.SendLine("Host: www.baidu.com");
		s.SendLine("");
		while (1) 
		{
			std::string l = s.ReceiveLine();
			if (l.empty()) break;
			Send(l);
		}

	} 
	catch (...) 
	{
		SetCloseAndDelete();
	}


	SetCloseAndDelete();
	return ;

	Deliver d(*this,GetServer(),"http",GetMethod(),GetUrl());


	d.Run();
}


void ServerSocket::Init() 
{
	ServerHandler& ref = static_cast<ServerHandler&>(Handler());
	if (GetParent() -> GetPort() == ref.GetInt("secure/port"))
	{
		//EnableSSL();
	}
}


void ServerSocket::OnAccept() 
{
	printf("Accept!\n");
	HTTPSocket::OnAccept();
}


