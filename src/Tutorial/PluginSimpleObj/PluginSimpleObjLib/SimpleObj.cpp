#include "StdAfx.h"
#include "SimpleObj.h"


SimpleObj::SimpleObj(void)
{

}


SimpleObj::~SimpleObj(void)
{

}

SIMPLEOBJLIB_API SimpleObj *Create()
{
	return new SimpleObj();
}