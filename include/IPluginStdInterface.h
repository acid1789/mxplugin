#pragma once

#include "IMxPluginBase.h"

class IPluginStdInterfaceLibIID
{
	public:
		static char *const GUID() {
			return "0ED26723-EB8F-44B7-B45E-7475BDE1E706";
		}

		static char *const Name() {
			return "IPluginStdInterfaceLib";
		}
};

class IPluginStdInterfaceLib: public IMxPluginBase
{
	public:
		virtual const char *MXSTDMETHOD GetComment() = 0;
}
