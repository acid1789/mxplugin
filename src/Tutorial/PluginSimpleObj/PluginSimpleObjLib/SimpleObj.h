#pragma once

#ifdef SIMPLEOBJLIB_EXPORTS
#define SIMPLEOBJLIB_API __declspec(dllexport)
#else
#define SIMPLEOBJLIB_API __declspec(dllimport)
#endif

#include <string>
#include "..\..\..\PublicInterfaces\Tutorial\ISimpleObj.h"


//Creates a new class which inherits properties from IFrame
class SIMPLEOBJLIB_API SimpleObj : public ISimpleObj	
{
public:
	SimpleObj(void);							
	virtual ~SimpleObj(void);	
							
	virtual const char* GetMsg() { return "SimpleObj::GetMsg() is invoked";} 
	virtual void		Delete() {delete this;}
};


extern "C" SIMPLEOBJLIB_API SimpleObj *Create();