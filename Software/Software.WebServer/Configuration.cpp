/**
 **	File ......... Configuration.cpp
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
#include "Configuration.h"




Configuration::Configuration(const std::string& filename)
//:BaseXMLFile(filename, "", "configuration")
{
}


Configuration::~Configuration()
{
}


// TiXmlHandle Configuration::Find(TiXmlHandle base,const std::string& path)
// {
// 	Parse pa(path,"/");
// 	std::string ord = pa.getword();
// 	std::string str = pa.getrest();
// 	TiXmlHandle p = GetFirstElement(base,ord);
// 	if (p && str.size())
// 	{
// 		return Find(p,str);
// 	}
// 	return p;
// }
// 

