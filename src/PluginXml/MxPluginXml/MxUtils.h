#pragma once

#include <time.h>

class MxUtils
{
public:
	static const int	DebugStringLen = 512;			//must be > 5	'...[\n][\0]'
	
public:
	static bool MxUtils::IsExistingFolderName(const char *foldername, size_t MaxFoldernameLen=MAX_PATH);
	static bool MxUtils::IsExistingPathFilename(const char *pathfilename, size_t MaxPathFilenameLen=MAX_PATH);
	static bool MxUtils::IsValidPathFilename(const char *pathfilename, size_t MaxPathFilenameLen=MAX_PATH);

	static void MxUtils::LogMsg(const char *method, const char *printfStr, ...);
private:
	static clock_t		_startTime;

};

