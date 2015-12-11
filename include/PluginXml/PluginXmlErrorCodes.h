//ErrorCodes for PluginXml Solution - each code is used only once at a specific line in the codebase
//		when adding statement like MXLOG(MX2000, MxError::InvalidParams, MxError::QuietReport, "test me %d", 5);
//			1. Define ErrorCode in appropriate section below
//				value is in range 1000 - 40000
//			    comment is End User msg
//			2. Add appropriate string to the Application's string table 
//				ID= IDS_MX2000
//				Val= same as #define value 
//				Caption = End User msg (The string must be no longer than 4097 chars)

//PluginXml - base 6000 - this is the default message for any error code that doesn't have a corresponding error string in STRING TABLE
#define MX6000		6000	//-record this error number, terminate the app and then submit a defect report
#define MX6001		6001	//PluginXmlAttribute Interface cast not supported. Install the latest version of PlugXml and try again
#define MX6002		6002	//Interface unsupported. Please install PluginXML and try again
#define MX6003		6003	//MX6000
#define MX6004		6004	//MX6000
#define MX6005		6005	//MX6000
#define MX6006		6006	//Invalid name. Please enter another value and try again
#define MX6007		6007	//MX6000
#define MX6008		6008	//MX6000
#define MX6009		6009	//MX6000
#define MX6010		6010	//MX6000
#define MX6011		6011	//MX6000
#define MX6012		6012	//MX6000
#define MX6013		6013	//MX6000
#define MX6014		6014	//MX6000
#define MX6015		6015	//MX6000
#define MX6016		6016	//MX6000
#define MX6017		6017	//MX6000
#define MX6018		6018	//MX6000
#define MX6019		6019	//MX6000
#define MX6020		6020	//MX6000
#define MX6021		6021	//MX6000
#define MX6022		6022	//MX6000
#define MX6023		6023	//MX6000
#define MX6024		6024	//MX6000
#define MX6025		6025	//MX6000
#define MX6026		6026	//MX6000
#define MX6027		6027	//MX6000
#define MX6028		6028	//MX6000
#define MX6029		6029	//MX6000
#define MX6030		6030	//MX6000
#define MX6031		6031	//MX6000
#define MX6032		6032	//MX6000
#define MX6033		6033	//MX6000
#define MX6034		6034	//MX6000
#define MX6035		6035	//MX6000
#define MX6036		6036	//MX6000
#define MX6037		6037	//PluginXml Interface cast not supported. Install the latest version of PlugXml and try again
#define MX6038		6038	//MX6000
#define MX6039		6039	//Interface not supported. Install the latest version of PlugXml and try again
#define MX6040		6040	//MX6000
#define MX6041		6041	//PluginXmlDoc Interface cast not supported. Install the latest version of PlugXml and try again
#define MX6042		6042	//Validation that your document conforms to the xmlSchema file is not currently supported. Please remove xmlSchema and try again
#define MX6043		6043	//MX6000
#define MX6044		6044	//Your document is invalid. Please fix the problem and try again
#define MX6045		6045	//MX6000
#define MX6046		6046	//MX6000
#define MX6047		6047	//MX6000
#define MX6048		6048	//MX6000
#define MX6049		6049	//MX6000
#define MX6050		6050	//MX6000
#define MX6051		6051	//MX6000
#define MX6052		6052	//Unable to create new document. Install the latest version of PlugXml and try again
#define MX6053		6053	//Unable to open selected document. Select another file and try again.
#define MX6054		6054	//MX6000
#define MX6055		6055	//Unable to open selected document. Select another file and try again.
#define MX6056		6056	//MX6000
#define MX6057      6057	//Unable to open selected document. Select another file and try again.
#define MX6058		6058	//MX6000
#define MX6059		6059	//Unable to open selected document. Select another file and try again.
#define MX6060		6060	//Unable to save your document as it has invalid content. Open or create another file and try again.
#define MX6061		6061	//Unable to save your document as its file no longer exists. Try to save as a different file.
#define MX6062		6062	//Unable to save your document. Try to save as a different file.
#define MX6063		6063	//Unable to save your document in the selected file. Try to save as a different file.
#define MX6064		6064	//Unable to save your document in the selected file. Try to save as a different file.
#define MX6065		6065	//MX6000
#define MX6066		6066	//MX6000
#define MX6067		6067	//MX6000
#define MX6068		6068	//File not found. Select another file and try again.
#define MX6069		6069	//MX6000
#define MX6070		6070	//PluginXmlElement Interface cast not supported. Install the latest version of PlugXml and try again
#define MX6071		6071	//MX6000
#define MX6072		6072	//MX6000
#define MX6073		6073	//Unable to find expected attribute for Xml element. Fix the file and try again
#define MX6074		6074	//Insufficient memory to complete operation. Please terminate this program, reboot your computer and try aain
#define MX6075		6075	//MX6000
#define MX6076		6076	//Attribute name is not valid. Fix the file and try again.
#define MX6077		6077	//Attribute name is not found in Xml Element. Fix the file and try again.
#define MX6078		6078	//MX6000
#define MX6079		6079	//Attribute name is invalid. Enter the information again and retry.
#define MX6080		6080	//MX6000
#define MX6081		6081	//Attribute name or value is invalid. Enter the information again and retry.
#define MX6082		6082	//Attribute name is not found in Xml Element. Fix the file and try again.
#define MX6083		6083	//MX6000
#define MX6084		6084	//Attribute name is invalid. Enter the information again and retry.
#define MX6085		6085	//MX6000
#define MX6086		6086	//Attribute name is invalid. Enter the information again and retry.
#define MX6087		6087	//Attribute name is not found in Xml Element. Fix the file and try again.
#define MX6088		6088	//MX6000
#define MX6089		6089	//Attribute name is invalid. Enter the information again and retry.
#define MX6090		6090	//Attribute value is invalid. Enter the information again and retry.
#define MX6091		6091	//MX6000
#define MX6092		6092	//Invalid attribute array. Please fix the file and try again.
#define MX6093		6093	//Invalid attribute array. Please fix the file and try again.
#define MX6094		6094	//MX6000
#define MX6095		6095	//MX6000
#define MX6096		6096	//Invalid attribute array. Please fix the file and try again.
#define MX6097		6097	//MX6000
#define MX6098		6098	//MX6000
#define MX6099		6099	//Invalid attribute array. Please fix the file and try again.
#define MX6100		6100	//Invalid attribute array. Please fix the file and try again.
#define MX6101		6101	//Invalid attribute array. Please fix the file and try again.
#define MX6102		6102	//MX6000
#define MX6103		6103	//Invalid attribute array. Please fix the file and try again.
#define MX6104		6104	//Invalid attribute array. Please fix the file and try again.
#define MX6105		6105	//MX6000
#define MX6106		6106	//MX6000
#define MX6107		6107	//Attribute name is not found in Xml Element. Fix the file and try again.
#define MX6108		6108	//MX6000
#define MX6109		6109	//Invalid TEXT attribute. Please fix the file and try again.
#define MX6110		6110	//MX6000
#define MX6111		6111	//Text attribute is invalid. Enter the information again and retry.
#define MX6112		6112	//Text attribute is invalid. Enter the information again and retry.
#define MX6113		6113	//MX6000
#define MX6114		6114	//Invalid attribute value. Please fix the file and try again.
#define MX6115		6115	//MX6000
#define MX6116		6116	//Invalid attribute value. Please fix the file and try again.
#define MX6117		6117	//Invalid attribute value. Please fix the file and try again.
#define MX6118		6118	//Invalid attribute TEXT value. Please fix the file and try again.
#define MX6119		6119	//MX6000
#define MX6120		6120	//MX6000
#define MX6121		6121	//MX6000
#define MX6122		6122	//Insufficient memory to complete operation. Please terminate this program, reboot your computer and try aain
#define MX6123		6123	//Node not found in file. Fix your document and try again.
#define MX6124		6124	//MX6000
#define MX6125		6125	//MX6000
#define MX6126		6126	//Insufficient memory to complete operation. Please terminate this program, reboot your computer and try aain
#define MX6127		6127	//MX6000
#define MX6128		6128	//MX6000
#define MX6129		6129	//Insufficient memory to complete operation. Please terminate this program, reboot your computer and try aain
#define MX6130		6130	//Insufficient memory to complete operation. Please terminate this program, reboot your computer and try again
#define MX6131		6131	//MX6000
#define MX6132		6132	//Insufficient memory to complete operation. Please terminate this program, reboot your computer and try again
#define MX6133		6133	//Node not found in file. Fix your document and try again.
#define MX6134		6134	//MX6000
#define MX6135		6135	//Insufficient memory to complete operation. Please terminate this program, reboot your computer and try again
#define MX6136		6136	//MX6000
#define MX6137		6137	//MX6000
#define MX6138		6138	//MX6000
#define MX6139		6139	//MX6000
#define MX6140		6140	//MX6000
#define MX6141		6141	//MX6000
#define MX6142		6142	//MX6000
#define MX6143		6143	//MX6000
#define MX6144		6144	//MX6000
#define MX6145		6145	//MX6000
#define MX6146		6146	//Insufficient memory to complete operation. Please terminate this program, reboot your computer and try again
#define MX6147		6147	//MX6000
#define MX6148		6148	//MX6000
#define MX6149		6149	//MX6000
#define MX6150		6150	//MX6000

//PluginXmlApp
#define MX6300		6300	//record this error number, terminate the app and then submit a defect report
#define MX6301		6301	//MX7000


