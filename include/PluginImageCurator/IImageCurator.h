#pragma once
#include "IMxPluginBase.h"
#include "IMxPluginLibDetails.h"
#include "IImageCuratorFnDef.h"

static const GUID IImageCuratorGUID = { 0x91125f9d, 0x971c, 0x4ead, { 0xac, 0x4b, 0xa6, 0xe5, 0x30, 0xe4, 0x5f, 0x57 }};  // {91125F9D-971C-4EAD-AC4B-A6E530E45F57}

class IImageCuratorIID
{
public:
	static const GUID		  Guid() { return IImageCuratorGUID; }
	static const char * const Name() { return "Interface implemented by PluginImageCurator used by Plugins implementing IImageCuratorPluginLib"; }
};

#define IImageCuratorInvalidStr		"[invalid]"	
#define IImageCuratorInvalidInt		-1
#define IImageCuratorInvalidDouble	-1.0

class IImageCuratorPluginLib;
class IImageCuratorFn;
class IImageCuratorFnCall;

class  IImageCurator :  public IMxPluginBase
{
public:

	virtual bool		MXSTDMETHOD SetImageCuratorPluginLib(IImageCuratorPluginLib *plugin ) = 0;
	virtual bool		MXSTDMETHOD SaveImagePluginSpec(const char *PathFileName) = 0;

	virtual bool		MXSTDMETHOD CreateFunctionDef(GUID fnID, const char *label, const char *desc, const char *fnToolboxClass, const char *fnToolboxLabel, int fnToolboxIconID, IImageCuratorFnCall *call) = 0;
	virtual bool		MXSTDMETHOD RemoveFunctionDef(const GUID FnID, const char *FnLabel ) = 0;
	virtual bool		MXSTDMETHOD AddFunctionParamDef(GUID fnID, const char *name, IImageCuratorFnDef::Direction io, IImageCuratorFnDef::ParamType paramType, const char *valDefault, const char *label, const char *desc, const char *valMin=nullptr, const char *valMax=nullptr) = 0;
	virtual bool		MXSTDMETHOD RemoveFunctionParamDef(const GUID FnID, const char *name ) = 0;

	virtual bool		MXSTDMETHOD RemoveAllFunctionDefs() = 0;

	virtual IImageCuratorPluginLib *MXSTDMETHOD	GetIImageCuratorPluginLib() = 0;
	virtual IImageCuratorFnDef*	MXSTDMETHOD GetIImageCuratorFnDef(const GUID FnID ) = 0;
	virtual IImageCuratorFn*	MXSTDMETHOD GetIImageCuratorFn(const GUID FnID ) = 0;
	virtual bool		MXSTDMETHOD IsExistIImageCuratorFn(const GUID FnID ) = 0;

	virtual int			MXSTDMETHOD GetFunctionCount() = 0;
	virtual const GUID	MXSTDMETHOD GetFunctionID(int index) = 0;

	virtual bool		MXSTDMETHOD Validate() = 0;
	virtual bool		MXSTDMETHOD IsValid() = 0;

	virtual long		MXSTDMETHOD DestroyPtr(void) = 0;
	virtual long		MXSTDMETHOD DuplicatePtr(void) = 0;
	virtual void *		MXSTDMETHOD Dynamic_cast(const GUID IId) = 0;
	virtual const GUID  MXSTDMETHOD GetInterfaceGUID(void) = 0;	
	virtual const char *MXSTDMETHOD GetInterfaceName(void) = 0;


};