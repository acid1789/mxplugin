#pragma once

#include <string>

class DefUniversalApp
{
	public:
		static const char *const SID()
		{
			std::string s("C80019CD-5D9F-4A90-94D1-2D95C8CB25D6");
			return s.c_str();
		}

		static const char *const Name()
		{
			std::string s("DefUniversalApp");
			return s.c_str();
		}

		static const char *const Description()
		{
			std::string s("General purpose MxPlugin");
			return s.c_str();
		}

		static const char *const SupportSite()
		{
			std::string s("http://mxplugin.codeplex.com/");
			return s.c_str();
		}
};
