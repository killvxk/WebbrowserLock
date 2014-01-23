#pragma once

#include <UtilFunc/PluginInc.h>


const char* const ClsidBootrun = "9CED84E4-B569-468B-9511-7A34E17B3DC4";

class IPlugin : public Ix_Object
{
public:
	X3DEFINE_IID(IPlugin);
	virtual bool Initialization(PVOID pData) = 0;
};