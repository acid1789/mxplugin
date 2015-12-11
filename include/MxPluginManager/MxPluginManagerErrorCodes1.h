//ErrorCodes for MxPluginManager Solution - each code is used only once at a specific line in the codebase
//		when adding statement like MXLOG(MX1301, MxError::InvalidParams, MxError::QuietReport, "test me %d", 5);
//			1. Define ErrorCode in appropriate section below - see MxProductID.h for ranges
//				value is in range 1300 - 1499 - lib
//				value is in range 1500 - 1599 - app	
//				value is in range 1600 - 1599 - gui
//			    comment is End User msg
//			2. Add appropriate string to the Application's string table 
//				ID= IDS_MX1307
//				Val= same as #define value 
//				Caption = End User msg (The string must be no longer than 1397 chars)

//MxPluginManager - base 1300 - this is the default message for any error code that doesn't have a corresponding error string in STRING TABLE
#define MX1300		1300	//Record this error number, terminate the app and then submit a defect report
#define MX1301		1301	//MX1300
#define MX1302		1302	//MX1300 
#define MX1303		1303	//MX1300
#define MX1304		1304	//At least one subfolder of the selected root folder does not contain a MxPlugin file. Fix the MxPlugin installations on your PC  and try again.
#define MX1305		1305	//No MxPlugins found in subfolders of selected root folder. Please select another root folder and try again.
#define MX1306		1306	//MX1300 
#define MX1307		1307	//Unable to load MxPlugin. Its features will not be available. Please install the latest version and try again
#define MX1308		1308	//MX1300
#define MX1309		1309	//Unable to use MxPlugin. Its features will not be available. Please install the latest version and try again
#define MX1310		1310	//MX1300
#define MX1311		1311	//MX1300 - invalid params
#define MX1312		1312	//MX1300 - invalid params
#define MX1313		1313	//MX1300
#define MX1314		1314	//No plugins found in subfolders of given root folder
#define MX1315		1315	//MX1300
#define MX1316		1316	//MX1300
#define MX1317		1317	//MX1300
#define MX1318		1318	//MX1300
#define MX1319		1319	//MX1300
#define MX1320		1320	//MX1300
#define MX1321		1321	//MX1300
#define MX1322		1322	//MX1300
#define MX1323		1323	//ProdID not found in registry. Reinstall application and try again. Please report this problem.
#define MX1324		1324	//ProdID not found in registry. Reinstall application and try again. Please report this problem.
#define MX1325		1325	//ProdID not found in registry. Reinstall application and try again. Please report this problem.
#define MX1326		1326	//ProdID not found in registry. Reinstall application and try again. Please report this problem.
#define MX1327		1327	//ProdID not found in registry. Reinstall application and try again. Please report this problem.
#define MX1328		1328	//ProdID not found in registry. Reinstall application and try again. Please report this problem.
#define MX1329		1329	//MX1300
#define MX1330		1330	//MX1300
#define MX1331		1331	//MX1300
#define MX1332		1332	//MX1300
#define MX1333		1333	//Unable to read AppRootKey from registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1334		1334	//MX1300
#define MX1335		1335	//Unable to remove MxPluginManagerGUI settings from Registry. Please report this problem.
#define MX1336		1336	//AppCnt not found in Windows Registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1337		1337	//MX1300
#define MX1338		1338	//MX1300
#define MX1339		1339	//
#define MX1340		1340	//MX1300
#define MX1341		1341	//Unable to write Apps to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1342		1342	//Unable to write App* to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1343		1343	//
#define MX1344		1344	//Unable to write PluginRootFolder to registry. Please report this problem.
#define MX1345		1345	//MX1300
#define MX1346		1346	//Unable to register MxPluginManagerGUI. Please report this problem.
#define MX1347		1347	//MX1300
#define MX1348		1348	//Unable to write Apps to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1349		1349	//Unable to read AppCnt from registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1350		1350	//Unable to write AppCnt to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1351		1351	//Unable to write App* to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1352		1352	//Unable to write App*\Name to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1353		1353	//Unable to write App*\ProdID to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1354		1354	//Unable to write App*\LogLevel to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1355		1355	//Unable to write App*\LogChannel to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1356		1356	//ProdID not found in registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1357		1357	//MX1300
#define MX1358		1358	//Unable to unregister application. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1359		1359	//MX1300
#define MX1360		1360	//MX1300
#define MX1361		1361	//MX1300
#define MX1362		1362	//Unable to read AppKey from registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again. 
#define MX1363		1363	//Unable to read AppName from registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again. 
#define MX1364		1364	//MX1300
#define MX1365		1365	//Unable to read AppLogChannel from registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again. 
#define MX1366		1366	//
#define MX1367		1367	//Unable to write AppLogChannel to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again. 
#define MX1368		1368	//
#define MX1369		1369	//Unable to read AppLogLevel from registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again. 
#define MX1370		1370	//-unassigned
#define MX1371		1371	//Unable to write AppLogLevel to registry. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again. 
#define MX1372		1372	//MX1300
#define MX1373		1373	//Registry AppCnt is invalid. Please report this problem or use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1374		1374	//MX1300
#define MX1375		1375	//-unassigned
#define MX1376		1376	//
#define MX1377		1377	//-unassigned
#define MX1378		1378	//-unassigned
#define MX1379		1379	//MX1300
#define MX1380		1380	//MX1300
#define MX1381		1381	//MX1300
#define MX1382		1382	//MX1300
#define MX1383		1383	//MX1300
#define MX1384		1384	//MX1300
#define MX1385		1385	//MX1300
#define MX1386		1386	//MX1300
#define MX1387		1387	//MX1300
#define MX1388		1388	//MX1300
#define MX1389		1389	//MX1300
#define MX1390		1390	//MX1300
#define MX1391		1391	//MX1300
#define MX1392		1392	//MX1300
#define MX1393		1393	//MX1300
#define MX1394		1394	//MX1300
#define MX1395		1395	//MX1300
#define MX1396		1396	//MX1300
#define MX1397		1397	//MX1300
#define MX1398		1398	//MX1300
#define MX1399		1399	//MX1300
#define MX1400		1400	//MX1300

//MxPluginManagerApp
#define MX1500		1500	//record this error number, terminate the app and then submit a defect report
#define MX1501		1501	//MX5000
#define MX1502		1502	//Non-standard MxPlugin found. Its features will not be available. Please install the latest version and try again
#define MX1503		1503	//Unexpected error - exception thrown. Please set another Plugin root folder and restart the program


//MxPluginManagerGUI
#define MX1600		1600	//Record this error number, terminate the app and then submit a defect report
#define MX1601		1601	//Required settings not found in Windows Registry. Use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1602		1602	//Unable to register this application. Use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1603		1603	//Program failed to initialise properly. Please submit a defect report.
#define MX1604		1604	//Program ended unexpectedly. Please submit a defect report.
#define MX1605		1605	//Program failed to initialise properly. Please submit a defect report.
#define MX1606		1606	//Required settings not found in Windows Registry. Use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1607		1607	//Failed to add all Plugins found to the list. Please report this problem.
#define MX1608		1608	//MX1600
#define MX1609		1609	//MX1600
#define MX1610		1610	//MX1600
#define MX1611		1611	//MX1600
#define MX1612		1612	//MX1600
#define MX1613		1613	//MX1600
#define MX1614		1614	//MX1600
#define MX1615		1615	//MX1600
#define MX1616		1616	//MX1600
#define MX1617		1617	//MX1600
#define MX1618		1618	//Program failed to initialise properly.Use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1619		1619	//MX1600
#define MX1620		1620	//MX1600
#define MX1621		1621	//MX1600
#define MX1622		1622	//Program failed to initialise properly. Please submit a defect report.
#define MX1623		1623	//MX1600
#define MX1624		1624	//MX1600
#define MX1625		1625	//MX1600
#define MX1626		1626	//Program failed to initialise properly. Please submit a defect report.
#define MX1627		1627	//MX1600
#define MX1628		1628	//MX1600
#define MX1629		1629	//MX1600
#define MX1630		1630	//MX1600
#define MX1631		1631	//MX1600
#define MX1632		1632	//MX1600
#define MX1633		1633	//Unable to save your new MxPlugin root folder. Please select a different folder and try again.
#define MX1634		1634	//MX1600
#define MX1635		1635	//MX1600
#define MX1636		1636	//MX1600
#define MX1637		1637	//MX1600
#define MX1638		1638	//Invalid values found in Windows Registry. Use MxPluginManagerGUI Options dialog box to set Factory Defaults and try again.
#define MX1639		1639	//MX1600
#define MX1640		1640	//MX1600
#define MX1641		1641	//MX1600
#define MX1642		1642	//MX1600
#define MX1643		1643	//MX1600
#define MX1644		1644	//MX1600
#define MX1645		1645	//MX1600
#define MX1646		1646	//MX1600
#define MX1647		1647	//MX1600
#define MX1648		1648	//MX1600
#define MX1649		1649	//MX1600
#define MX1650		1650	//MX1600
#define MX1651		1651	//MX1600
#define MX1652		1652	//
#define MX1653		1653	//OutputDebugStr viewer application not found. Select another viewer application and try again
#define MX1654		1654	//MX1600
#define MX1655		1655	//Log file not found. Select another log file and try again
#define MX1656		1656	//MX1600
#define MX1657		1657	//Unexpected error copying log file. Check that you have stopped the Trace and try again
#define MX1658		1658	//Source log file not found. Select another log file and try again
#define MX1659		1659	//Destination log file already exists or cannot be created.  Select another destination for the log file\r\nand try again
#define MX1660		1660	//You do not have the privileges required to run Logman. Pleae run MxPluginManager as Admin and try again
#define MX1661		1661	//Problem running Logman. Try running the command from DOS prompt
#define MX1662		1662	//The specified Trace isn't setup on this PC. Enter MxUtilsTrace (default Trace) and try again
#define MX1663		1663	//
#define MX1664		1664	//MX1600
#define MX1665		1665	//MX1600
#define MX1666		1666	//MX1600
#define MX1667		1667	//MX1600
