#pragma once

#include "IMxPluginLibDetails.h"

class IPluginStdDetailsLibIID
{
	public:
		static const char *const GUID() {
			return "43719372-B4CF-40CC-8514-AA47A34C8882";
		}

		static const char *const Name() {
			return "IPluginStdDetailsLib";
		}
};

class  IPluginStdDetailsLib: public IMxPluginLibDetails
{
	public:
		virtual const char *MXSTDMETHOD GetComment(void) = 0;
};
