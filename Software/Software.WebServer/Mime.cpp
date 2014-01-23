/**
 **	File ......... Mime.cpp
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
#include "Mime.h"




Mime::Mime(const std::string& filename)
{
	FILE *fil;
	if ((fil = fopen(filename.c_str(),"rt")) != NULL)
	{
		char slask[1000];
		fgets(slask,1000,fil);
		while (!feof(fil))
		{
			while (strlen(slask) && (slask[strlen(slask) - 1] == 13 || slask[strlen(slask) - 1] == 10))
			{
				slask[strlen(slask) - 1] = 0;
			}
			Parse pa(slask);
			std::string mime_type = pa.getword();
			std::string ext = pa.getword();
			while (ext.size())
			{
				m_mime[ext] = mime_type;
				ext = pa.getword();
			}
			//
			fgets(slask,1000,fil);
		}
		fclose(fil);
	}
	else
	{
		fprintf(stderr,"Couldn't open file '%s'...\n",filename.c_str());
	}
}


Mime::~Mime()
{
}


std::string Mime::GetMimeFromExtension(const std::string& ext)
{
	std::string mime = m_mime[ext];
	return mime;
}


