#pragma once
#include "IMxPluginBase.h"

static const GUID IImageCuratorFnDefGUID = { 0x9f638463, 0xfe21, 0x4572, { 0x96, 0x5d, 0x20, 0x7a, 0xd8, 0x33, 0x26, 0x3e }};  // {9F638463-FE21-4572-965D-207AD833263E}
class IImageCuratorFnDefIID
{
public:
	static const GUID		  Guid() { return IImageCuratorFnDefGUID; }
	static const char * const Name() { return "Interface for defining functions implemented by an ImageCurator Plugin"; }
};

class  IImageCuratorFnDef :  public IMxPluginBase
{
public:

	static const int		  IImageCuratorFnDef::Invalid = -1;

	enum ParamType
	{                            
		String = 1,	
		Integer,
		Double, 
		Bool,				 
	};

	enum Direction
	{                            
		Input = 1,	
		Output, 
		IO, 
		Unknown,				 
	};

virtual const char *MXSTDMETHOD GetToolboxClass() = 0;
virtual int			MXSTDMETHOD GetToolboxIconID() = 0;
virtual const char *MXSTDMETHOD GetToolboxLabel() = 0;
virtual const char *MXSTDMETHOD GetLabel() = 0;
virtual const char *MXSTDMETHOD GetDescription() = 0;
virtual const GUID  MXSTDMETHOD GetFnId() = 0;

virtual bool		MXSTDMETHOD AddParamDef(const char *name, Direction io, ParamType paramType, const char *ValDefault, const char *Label, const char *Desc, const char *ValMin, const char *ValMax) = 0; 
virtual bool		MXSTDMETHOD RemoveParamDef(const char *name) = 0;

virtual int			MXSTDMETHOD GetParamCount(Direction io) = 0;
virtual const char *MXSTDMETHOD GetParamName(Direction io, int index) = 0;
virtual const char *MXSTDMETHOD GetParamDefaultVal(const char *ParamName) = 0;
virtual const char *MXSTDMETHOD GetParamErrorVal(const char *ParamName) = 0;

virtual Direction   MXSTDMETHOD GetParamDirection(const char *ParamName) = 0;
virtual const char *MXSTDMETHOD GetParamLabel(const char *ParamName) = 0;
virtual const char *MXSTDMETHOD GetParamValType(const char *ParamName) = 0;
virtual const char *MXSTDMETHOD GetParamDesc(const char *ParamName) = 0;
virtual bool		MXSTDMETHOD IsParamValMinSet(const char *ParamName) = 0;
virtual bool		MXSTDMETHOD IsParamValMaxSet(const char *ParamName) = 0;
virtual int			MXSTDMETHOD GetParamValMin(const char *ParamName) = 0;
virtual int			MXSTDMETHOD GetParamValMax(const char *ParamName) = 0;
virtual double		MXSTDMETHOD GetParamDoubleValMin(const char *ParamName) = 0;
virtual double		MXSTDMETHOD GetParamDoubleValMax(const char *ParamName) = 0;
virtual const char *MXSTDMETHOD GetParamValMinStr(const char *ParamName) = 0;
virtual const char *MXSTDMETHOD GetParamValMaxStr(const char *ParamName) = 0;

virtual const char *MXSTDMETHOD ConvertParamTypeToStr(ParamType paramType) = 0;
virtual ParamType	MXSTDMETHOD ConvertStrToParamType(const char *paramType) = 0;

virtual bool		MXSTDMETHOD IsValid() = 0;

virtual long		MXSTDMETHOD DestroyPtr(void) = 0;
virtual long		MXSTDMETHOD DuplicatePtr(void) = 0;
virtual void *		MXSTDMETHOD Dynamic_cast(const GUID IId) = 0;
virtual const GUID	MXSTDMETHOD GetInterfaceGUID(void) = 0;
virtual const char *MXSTDMETHOD GetInterfaceName(void) = 0;

};