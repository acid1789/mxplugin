//ErrorCodes for MxUtils Solution - each code is used only once at a specific line in the codebase
//		when adding statement like MXLOG(MX2000, MxError::InvalidParams, MxError::Always, "test me %d", 5);
//			1. Define ErrorCode in appropriate section below
//				value is in range 1000 - 40000
//			    comment is End User msg
//			2. Add appropriate string to the Application's string table 
//				ID= IDS_MX2000
//				Val= same as #define value 
//				Caption = End User msg (The string must be no longer than 4097 chars)

//MxUtilsApp - base 1000 - this is the default message for any error code that doesn't have a corresponding error string in STRING TABLE
#define MX1000		1000	//record this error number, terminate the app and then submit a defect report
#define MX1001		1001	//MX1000 - not used
#define MX1002		1002	//Unit Test Error	
#define MX1003		1003	//Unable to delete registry items for [App]. Please report this problem.
#define MX1004		1004	//Unable to find registry key for [App]. Please report this problem.
#define MX1005		1005	//MX1000
#define MX1006		1006	//MX1000
#define MX1007		1007	//MX1000
#define MX1008		1008	//MX1000
#define MX1009		1009	//Unable to create registry key for [App]. Please report this problem.
#define MX1010		1010	//MX1000
#define MX1011		1011	//Unable to create registry subkey for [App]. Please report this problem.
#define MX1012		1012	//MX1000
#define MX1013		1013	//Unable to set registry integer value for [App]. Please report this problem.
#define MX1014		1014	//MX1000
#define MX1015		1015	//Unable to read registry integer value for [App]. Please report this problem.
#define MX1016		1016	//MX1000
#define MX1017		1017	//Unable to set registry string value for [App]. Please report this problem.
#define MX1018		1018	//MX1000
#define MX1019		1019	//Unable to read registry string value for [App]. Please report this problem.
#define MX1020		1020	//Unable to save string value in registry because it is too long. Please enter a shorter string and try again.
#define MX1021		1021	//MX1000
#define MX1022		1022	//Attempting to open a particular registry key failed for reasons other than not found. Please report this problem.
#define MX1023		1023	//Failed to close a particular registry key. Please report this problem.
#define MX1024		1024	//MX1000
#define MX1025		1025	//MX1000
#define MX1026		1026	//MX1000
#define MX1027		1027	//MX1000
#define MX1028		1028	//MX1000
#define MX1029		1029	//Command failed to run. Your PC may not be setup to run this command. Please report this problem.
#define MX1030		1030	//MX1000
#define MX1031		1031	//MX1000
#define MX1032		1032	//MX1000
#define MX1033		1033	//MX1000
#define MX1034		1034	//MX1000
#define MX1035		1035	//MX1000
#define MX1036		1036	//MX1000
#define MX1037		1037	//MX1000
#define MX1038		1038	//MX1000
#define MX1039		1039	//MX1000
#define MX1040		1040	//MX1000
#define MX1041		1041	//MX1000
#define MX1042		1042	//new connection string doesn't contain User ID, change connection string and try again
#define MX1043		1043	//new connection string doesn't contain Password, change connection string and try again
#define MX1044		1044	//new connection string doesn't contain Data Source, change connection string and try again
#define MX1045		1045	//new connection string doesn't contain Initial Catalog, change connection string and try again
#define MX1046		1046	//MX1000
#define MX1047		1047	//MX1000
#define MX1048		1048	//encryption engine failed, close application and try again
#define MX1049		1049	//memory shortage, close application and try again
#define MX1050		1050	//configuration problem. Install Code page 932 and try again
#define MX1051		1051	//encryption engine failed, close application and try again
#define MX1052		1052	//MX1000 - DecryptString, invalid params
#define MX1053		1053	//MX1000
#define MX1054		1054	//memory shortage, close application and try again
#define MX1055		1055	//encryption engine failed, close application and try again
#define MX1056		1056	//MX1000
#define MX1057		1057	//MX1000
#define MX1058		1058	//MX1000
#define MX1059		1059	//unable to open password file, reinstall file and try again
#define MX1060		1060	//unable to write password to file, enter another password and try again
#define MX1061		1061	//MX1000 - exception
#define MX1062		1062	//MX1000 - bad params
#define MX1063		1063	//unable to open password file, reinstall file and try again
#define MX1064		1064	//password file is invalid, reinstall file and try again
#define MX1065		1065	//cannot read password file, reinstall file and try again
#define MX1066		1066	//MX1000 - exception
#define MX1067		1067	//MX1000 - exception
#define MX1068		1068	//MX1000
#define MX1069		1069	//MX1000
#define MX1070		1070	//MX1000