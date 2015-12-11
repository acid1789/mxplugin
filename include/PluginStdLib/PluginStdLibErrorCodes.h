//=========================================================================================================================================
//PluginStdLibErrorCodes.h - VERSION 1.0.30.0 - do not remove or change error codes, just add new codes to the end of the list
//=========================================================================================================================================
//
// Each code is used only once at a specific line in the codebase. For example, when adding statement like
//	MX_SETERROR(MX1003, MxError::InvalidParams, MxError::Abort,  MxError::QuietReport, "test me %d", 5);
//			1. Define ErrorCode in appropriate section below
//				value is in range 1200-1299 (see MxProductID.h)
//			    comment is End User msg
//			2. Add appropriate string to the Application's string table 
//				ID= IDS_MX2003
//				Val= same as #define value 
//				Caption = End User msg - copy from comment, if english (The string must be no longer than 4097 chars)
//
//=========================================================================================================================================

//PluginStdLib - range 1200 - 1250 (see MxProductID.h)
// MX1800 is the default message for any error code that doesn't have a corresponding error string in STRING TABLE
#define MX1200		1200	//Component is defective so feature is not available. Please record this error number and submit a defect report
#define MX1201		1201	//Component does not supported requested interface. Please reinstall the program and try again.
#define MX1202		1202	//Component does not supported requested interface. Please reinstall the program and try again

//PluginStdLibApp - Error range 1250-1299 (see MxProductID.h)

//=========================================================================================================================================
//				E N D   O F   F I L E
//=========================================================================================================================================