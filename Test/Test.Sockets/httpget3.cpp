/**
 **	File ......... httpget.cpp
 **	Published ....  2004-04-07
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

#define DIRECTORY_LIB				"..\\..\\LIB\\"
#define DIRECTORY_COMMON			"..\\..\\Common\\"

#include <Function.Internal.SocketsControl\Sockets\HttpGetSocket.h>
#include <Function.Internal.SocketsControl\Sockets\SocketHandler.h>
#include <Function.Internal.SocketsControl\Sockets\Parse.h>

#pragma comment(lib,DIRECTORY_LIB "Function\\Function.Internal.SocketsControl.lib")


bool Get(const std::string& url_in,std::list<std::string>& ref)
{
	Parse pa(url_in,":/");
	std::string protocol = pa.getword();
	std::string host = pa.getword();
	port_t port;
	{
		Parse pa(host,":");
		pa.getword();
		port = pa.getvalue();
	}
	std::string url = "/" + pa.getrest();
	std::string file;
	{
		Parse pa(url,"/");
		std::string tmp = pa.getword();
		while (tmp.size())
		{
			file = tmp;
			tmp = pa.getword();
		}
	}
	SocketHandler h;
	bool complete = false;
	if (!strcasecmp(protocol.c_str(),"http") || !strcasecmp(protocol.c_str(),"https"))
	{
		HttpGetSocket s(h, url_in, file);
		h.Add(&s);
		h.Select(1,0);
		while (h.GetCount())
		{
			h.Select(1,0);
		}
		std::string gstring(s.GetStatusText());
		complete = s.Complete();
		unsigned const char * charbuff = (unsigned const char * )s.GetBytesReceived();
	}
	else
	{
		printf("Unknown protocol: '%s'\n",protocol.c_str());
	}
	if (complete)
	{
		FILE *fil = fopen(file.c_str(), "rt");
		char slask[30000];
		fgets(slask, 30000, fil);
		while (!feof(fil))
		{
			slask[strlen(slask) - 1] = 0;
			ref.push_back(slask);
			//
			fgets(slask, 30000, fil);
		}
		fclose(fil);
	}
	return complete;
}


int main(int argc,char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		std::list<std::string> doc;
		if (!Get(argv[i], doc))
		{
			printf("Failed: %s\n",argv[i]);
		}
		else
		{
			for (std::list<std::string>::iterator it = doc.begin(); it != doc.end(); it++)
			{
				std::string str = *it;
				printf("%s\n", str.c_str());
			}
		}
	}
}


