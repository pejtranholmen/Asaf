#include "..\stdafx.h"
#include "..\std.h"
#include "..\Util.h"
#include "..\MainFrm.h"
#include ".\Link.h"
#include "..\CUG\SimDoc\Mycug.h"
#include "..\CoupModel.h"
//#include ".\Datatrans.h"
#ifndef COUP2013
#include "..\SQL\SQL_Trans.h"
#include "..\SQL\SQL_exchange.h"
static 	MYSQL           *myData;      // pointer to database
static	MYSQL_RES	    *res;         // pointer to result set
static	MYSQL_ROW	    row;          // variable for handling row data
#endif
#include "afxdb.h"
#include "odbcinst.h"


Link::Link() {
 m_readflag=0;

}
Link::~Link()
{

		//MySingle::instance().GetCugPointer()->OnReset();

}

void Link::ReportReady(CCoupModelDoc *pDoc)
{

	MySingle::instance().GetCugPointer()->SetDocument(pDoc);
	//MySingle::instance().GetCugPointer()->OnSetup();
	MySingle::instance().GetFileViewPointer()->FillFileView();
	MySingle::instance().GetChartCugPointer()->RealTimeMode(FALSE);

}
void Link::ReportUpdate(CCoupModelDoc *pDoc)
{
	MySingle::instance().GetCugPointer()->ReportUpdate();
	MySingle::instance().GetCugPointer()->UpdateRealTimeChart();

}
string Link::GetExcelFile()
{
	m_ExcelSumFile=MFC_Util::GetProfileStringStd("ExcelSumFile", m_ExcelSumFile);
	return m_ExcelSumFile;
}
string Link::GetExcelDriver()
{
    TCHAR szBuf[2001];
    WORD cbBufMax = 2000;
    WORD cbBufOut;
    TCHAR *pszBuf = szBuf;
    string sDriver;

    // Get the names of the installed drivers ("odbcinst.h" has to be included )
   if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
        return "";
    
 /*   // Search for the driver...
    do
    {
        if( strstr( pszBuf, "Excel" ) != 0 )
        {
            // Found !
            sDriver = string( pszBuf );
            break;
        }
        pszBuf = strchr( pszBuf, '\0' ) + 1;
    }
    while( pszBuf[1] != '\0' );
	*/
    return sDriver;
}

void Link::ReadExcel() 
{
/*    CDatabase* pDatabase;
    string sSql;
    CString sItem1, sItem2, sItem3;
    string sDriver;
    CString sDsn;
	
    string sFile = GetExcelFile();        // the file name. Could also be something like C:\\Sheets\\WhatDoIKnow.xls
    
    // Clear the contents of the listbox
//    m_ctrlList.Res/etContent();
    
    // Retrieve the name of the Excel driver. This is 
    // necessary because Microsoft tends to use language
    // specific names like "Microsoft Excel Driver (*.xls)" versus
    // "Microsoft Excel Treiber (*.xls)"
    sDriver = GetExcelDriver();
	pDatabase= new CDatabase;
    if( sDriver.size()==0 )
    {
        // Blast! We didn4t find that driver!
        AfxMessageBox(_T("No Excel ODBC driver found"));
        return;
    }
    
    // Create a pseudo DSN including the name of the Driver and the Excel file
    // so we don4t have to have an explicit DSN installed in our ODBC admin
    sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"),sDriver,sFile);

    TRY
    {
        // Open the database using the former created pseudo DSN
        pDatabase->Open(nullptr,false,false,sDsn);
        
        // Allocate the recordset
        CRecordset recset(pDatabase );

        // Build the SQL string
        // Remember to name a section of data in the Excel sheet using "Insert->Names" to be
        // able to work with the data like you would with a table in a "real" pDatabase-> There
        // may be more than one table contained in a worksheet.
   //     sSql = "SELECT field_1, field_2 "       
   //              "FROM demo_table "                 
   //              "ORDER BY field_1";
   //     sSql = "SELECT RunDate, Version "       
   //              "FROM RunInfo "                 
   //              "ORDER BY RunDate";
        sSql = "SELECT Name, RunNo, Val "       
                 "FROM parameter_values "   
//				 "WHERE Name==TempAirMean "
                 "ORDER BY Runno";
    
        // Execute that query (implicitly by opening the recordset)
        recset.Open(CRecordset::forwardOnly,sSql.c_str(),CRecordset::readOnly);

        // Browse the result
        while( !recset.IsEOF() )
        {
            // Read the result line
            recset.GetFieldValue("Name",sItem1);
            recset.GetFieldValue("Val",sItem2);
            recset.GetFieldValue("RunNo",sItem3);
			if(sItem1=="TempAirMean") {
				  sItem3="k";

			}

            // Insert result into the list
//            m_ctrlList.AddString( sItem1 + " --> "+sItem2 );

            // Skip to the next resultline
            recset.MoveNext();
        }

        // Close the database
        pDatabase->Close();
		delete pDatabase;
                             
    }
    CATCH(CDBException, e)
    {
        // A database exception occured. Pop out the details...
        AfxMessageBox("Database error: "+e->m_strError);
    }
    END_CATCH;*/
}
#ifndef COUP2013
string Link::GetMySQLDriver()
{
    char szBuf[2001];
    WORD cbBufMax = 2000;
    WORD cbBufOut;
    char *pszBuf = szBuf;
    string sDriver;

    // Get the names of the installed drivers ("odbcinst.h" has to be included )
   if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
        return "";
    
    // Search for the driver...
    do
    {
        if( strstr( pszBuf, "MySQL" ) != 0 )
        {
            // Found !
            sDriver = string( pszBuf );
            break;
        }
        pszBuf = strchr( pszBuf, '\0' ) + 1;
    }
    while( pszBuf[1] != '\0' );

    return sDriver;
}
bool Link::SQL_Call(string Query, string DefError)
{
 	if(mysql_query( myData, Query.c_str() )){
 		res = mysql_store_result( myData );
 		if(res!=nullptr) 
 			row = mysql_fetch_row( res );
// 		row[0]=DefError;
// 		TRACE0(row[0]);
		if(DefError.find(_T("No"))>=0) return false;
 		Query+=DefError;
		string message;
		message=Query.substr(0,40)+"\n";
 		//TRACE0(message);
		return false;
	}
	return true;

}
int Link::SQL_RES()
{
	CString test;
	if(m_SQL_Lines.GetSize()>0) m_SQL_Lines.RemoveAll();	


	res = mysql_store_result( myData );
	if(res!=nullptr) {
		int irow = (int) mysql_num_rows( res );
		int ifields= (int) mysql_num_fields	(res);
		m_SQL_fields1=ifields;
		while ( row = mysql_fetch_row( res ) ) {
			for (int n=0;n<ifields;n++) {
				m_SQL_Lines.Add(row[n]);
			}
		//m_ListRecords.AddString(test);
		}
		return irow;
	}
	return 0;
}
int Link::SQL_RES2()
{
	CString test;
	if(m_SQL_Lines2.GetSize()>0) m_SQL_Lines2.RemoveAll();	


	res = mysql_store_result( myData );
	if(res!=nullptr) {
		int irow = (int) mysql_num_rows( res );
		int ifields= (int) mysql_num_fields	(res);
		m_SQL_fields2=ifields;
		while ( row = mysql_fetch_row( res ) ) {
			for (int n=0;n<ifields;n++) {
				m_SQL_Lines2.Add(row[n]);
			}
		//m_ListRecords.AddString(test);
		}
		return irow;
	}
	return 0;
}
int Link::SQL_GetNumberofRows()
{
	if(m_SQL_fields1>0) 
		return m_SQL_Lines.GetSize()/m_SQL_fields1;
	else
		return 0;
}
int Link::SQL_GetNumberofRows2()
{
	if(m_SQL_fields2>0) 
		return m_SQL_Lines2.GetSize()/m_SQL_fields2;
	else
		return 0;
}
int Link::SQL_GetNumberofFields()
{
	if(m_SQL_fields1>0) 
		return m_SQL_fields1;
	else
		return 0;
}
CString Link::SQL_GetRow(int i)
{
	CString Result="";
	if(res!=nullptr) {
		int irow = (int) mysql_num_rows( res );
		int ifields= (int) mysql_num_fields	(res);
		int istart;
		istart=ifields*i;
		for (int n=istart;n<istart+ifields;n++) {
				Result+=m_SQL_Lines.GetAt(n)+"\t";
		}
		return Result;
	}
	return Result;

}
CString Link::SQL_GetItem(int irec, int icol)
{
	if(res!=nullptr) {
		int i=m_SQL_fields1*irec+icol;
		if(i<m_SQL_Lines.GetSize())
			return m_SQL_Lines.GetAt(i);
		return "";
	}
	return "";

}
CString Link::SQL_GetItem2(int irec, int icol)
{
	if(res!=nullptr) {
		int i=m_SQL_fields2*irec+icol;
		if(i<m_SQL_Lines2.GetSize())
			return m_SQL_Lines2.GetAt(i);
		return "";
	}
	return "";

}
bool Link::SQL_Check()
{

	CString		grp,typ, status;
	CStdioFile ParFile;
	CString namn;

	CString sSql;
	CString Line,temporary_String;
 	CString m_User,m_Computer;
  
  	

 		char cHost[256] = "pej_home"; // mysql server is installed on a local PC
		char szDB[]     = "CoupModel" ;  // the database that we will use
		char szSQL[]    = "SELECT * FROM kalle"; // a default SQL statement
		CWinApp* pApp = AfxGetApp();
		CString strApp=((CCoupModelApp*)pApp)->m_AppString;
		 
		CString login, host, password;
		host=pApp->GetProfileString(strApp  ,_T("MySQL_Server")  , host);    
		login=pApp->GetProfileString(strApp    ,_T("MySQL_User")    , login);        
		password=pApp->GetProfileString(strApp,_T("MySQL_Password"), password); 

		m_User=pApp->GetProfileString(strApp, _T("UserName"), m_User);
		m_Computer=pApp->GetProfileString(strApp, _T("ComputerName"), m_Computer);
	if( (myData = mysql_init((MYSQL*) 0)) &&
                  mysql_real_connect( myData, host, login, password, nullptr, MYSQL_PORT,
			      nullptr, 0 ) ){
			if ( mysql_select_db( myData, szDB ) < 0 ) {
					MFC_Util::MessageBox(_T("Does database exist?"),_T("Database selection failed!"),MB_OK | MB_ICONERROR);
					mysql_close( myData ) ;
					return false;
			}
	}
	else {
			  MFC_Util::MessageBox(_T("Is MySQL server running?"),_T("Unable to connect to server!"),MB_OK | MB_ICONERROR);
			mysql_close( myData ) ;
	        return false;
	}
	return true;

}
bool Link::SQL_UPDATEVERSION()
{

	CString	grp,typ, status;


	CStdioFile ParFile;
	CString namn;

	CString sSql;
	CString Line,temporary_String;
 	CString m_User,m_Computer;
  


 		char cHost[256] = "pej_home"; // mysql server is installed on a local PC
		char szDB[]     = "CoupModel" ;  // the database that we will use
		char szSQL[]    = "SELECT * FROM kalle"; // a default SQL statement
		CWinApp* pApp = AfxGetApp();
		CString strApp=((CCoupModelApp*)pApp)->m_AppString;
		 
		CString login, host, password;
		host=pApp->GetProfileString(strApp  ,_T("MySQL_Server")  , host);    
		login=pApp->GetProfileString(strApp    ,_T("MySQL_User")    , login);        
		password=pApp->GetProfileString(strApp,_T("MySQL_Password"), password); 

		m_User=pApp->GetProfileString(strApp, _T("UserName"), m_User);
		m_Computer=pApp->GetProfileString(strApp, _T("ComputerName"), m_Computer);
	if( (myData = mysql_init((MYSQL*) 0)) &&
                  mysql_real_connect( myData, host, login, password, nullptr, MYSQL_PORT,
			      nullptr, 0 ) ){
			if ( mysql_select_db( myData, szDB ) < 0 ) {
//					MessageBox("Does database exist?","Database selection failed!",MB_OK | MB_ICONERROR);
					mysql_close( myData ) ;
					return false;
			}
	}
	else {
  //			  MessageBox("Is MySQL server running?","Unable to connect to server!",MB_OK | MB_ICONERROR);
			mysql_close( myData ) ;
	        return false;
	}

		SQL_Call("SELECT Version FROM DefVersion;", "No" );
		int i=SQL_RES();
		if(i>0) {
			int problem=10;
			MFC_Util::MessageBox(sSql,_T("SQL Warning Record already exist - No action taken"),MB_OK|MB_ICONEXCLAMATION);
		}

/*

	for(int j=0;j<8;j++) {	
	//Choose the current name for the type
		switch (j)	{
		case 0:	
			typ="Parameters";	
  		break;
		case 1:	
			typ="Switches";
   		break;
		case 2:	
			typ="Tables";
    		break;
		case 3:	typ="State Variables";			break;
		case 4:	typ="Flow Variables";			break;
		case 5:	typ="Auxiliary Variables";		break;
		case 6:	typ="Driving Variables";		break;
		case 7:	
			typ="Files";
		};
			
		string Namnvektor, Groupvektor; Groupvektor=Namnvektor="ENUM (";	
   		pos=GetFortranMap()->reg.GetGroupStart();
		int i=0;

		while (pos!=nullptr) {
		//Get the array with this type and the current group
		if(GetFortranMap()->reg.GetArray(typ,grp,arr)) {
			Sw=arr->GetAt(0);
			//namn=Sw->GetGroup()+"\"H"+Sw->GetGroupID();
			//Groupvektor+="'"+Sw->GetGroup()+"',";
			i++;

		}
		GetFortranMap()->reg.GetNextGroup(pos,grp);
		   //	Sw=arr->GetAt(0);
		  //	namn=Sw->GetGroup()+"\"H"+Sw->GetGroupID();
			Groupvektor+="'"+grp+"',";
			i++;

	}
	Groupvektor.TrimRight(_T(","));
	Groupvektor+=")";
	CString Typvektor;
	Typvektor="ENUM ('Switches','Parameters','Tables','State Variables','Flow Variables','Auxiliary Variables','Driving Variables','Input Files')";
	sSql="ALTER TABLE GroupsUsed CHANGE Groups Groups "+Groupvektor+";"	;

	if(j==0) SQL_Call(sSql,"Files"); 

	pos=GetFortranMap()->reg.GetGroupStart();
	int jj=0;
	//Traverse all groups
	while(pos!=nullptr) {
		int i=0;
		jj=jj+1;
	//Get next group
		GetFortranMap()->reg.GetNextGroup(pos,grp);
	//Get the array with this type and the current group
		if(GetFortranMap()->reg.GetArray(typ,grp,arr)){
   			bool isgroup=true;
			Namnvektor="ENUM (";
			//Loop through all the variables in the array to see whether they are disabled or not
				while(i<arr->GetSize())	{
		   ///		int jjj=arr->GetSize();

		   			Sw=arr->GetAt(i);
					Namnvektor+="'"+Sw->GetName()+"'," ;


		   //		if(!Sw->IsDisabled())
		   //			isgroup=true;
		   		i++;
		   	}
			Namnvektor.TrimRight(_T(","));
			Namnvektor+=")";
			namn=Sw->GetGroup()+"`";
					switch (j)	{
						case 0:	
							typ="Parameters";
							// Create SQL command
							sSql="ALTER TABLE `ValP_"+namn+" CHANGE Parameter Parameter "+Namnvektor+";";
							SQL_Call(sSql, "Par"); 
  							break;
						case 1:	
							typ="Switches";
  							// Create SQL command
							sSql="ALTER TABLE `ValSw_"+namn+" CHANGE Switch Switch "+Namnvektor+";";
							SQL_Call(sSql, "Par"); 
   						break;
						case 2:	
							typ="Tables";
							// Create SQL command
							sSql="ALTER TABLE `ValPTable_"+namn+" CHANGE ParameterTable ParameterTable "+Namnvektor+";";
							SQL_Call(sSql, "Par"); 
							break;
						case 3:	typ="State Variables";
							// Create SQL command
							sSql="ALTER TABLE `ValState_"+namn+" CHANGE State State "+Namnvektor+";";
							SQL_Call(sSql, "Par"); 
  							break;
						case 4:	typ="Flow Variables";
							// Create SQL command
							sSql="ALTER TABLE `ValFlow_"+namn+" CHANGE Flow Flow "+Namnvektor+";";
							SQL_Call(sSql, "Par"); 
  							break;
						case 5:	typ="Auxiliary Variables";
							// Create SQL command
							sSql="ALTER TABLE `ValAuxiliary_"+namn+" CHANGE Auxiliary Auxiliary "+Namnvektor+";";
							SQL_Call(sSql, "Par"); 
  							break;
						case 6:	typ="Driving Variables";
							// Create SQL command
							sSql="ALTER TABLE `ValDrivings_"+namn+" CHANGE Driving Driving "+Namnvektor+";";
							SQL_Call(sSql, "Par"); 
  							break;
						case 7:	
							typ="Files";
							// Create SQL command
							sSql="ALTER TABLE `ValFiles_"+namn+" CHANGE Files Files "+Namnvektor+";";
							SQL_Call(sSql, "Par"); 
					};			

		}
	}
}*/
   	return true;
}
bool Link::SQL_CREATE()
{/*
	POSITION	pos;
	string		grp,typ, status;
	CBasePtrArray *arr;
	CBase *Sw;
	CStdioFile ParFile;
	string namn;

	string sSql;
	string Line,temporary_String;
 	string m_User,m_Computer;
  
  		pos=GetFortranMap()->reg.GetGroupStart();

 		char cHost[256] = "pej_home"; // mysql server is installed on a local PC
		char szDB[]     = "CoupModel" ;  // the database that we will use
		char szSQL[]    = "SELECT * FROM kalle"; // a default SQL statement
		CWinApp* pApp = AfxGetApp();
		string strApp=((CCoupModelApp*)pApp)->m_AppString;
		 
		string login, host, password;
		host=pApp->GetProfileString(strApp  ,_T("MySQL_Server")  , host);    
		login=pApp->GetProfileString(strApp    ,_T("MySQL_User")    , login);        
		password=pApp->GetProfileString(strApp,_T("MySQL_Password"), password); 

		m_User=pApp->GetProfileString(strApp, _T("UserName"), m_User);
		m_Computer=pApp->GetProfileString(strApp, _T("ComputerName"), m_Computer);
	if( (myData = mysql_init((MYSQL*) 0)) &&
                  mysql_real_connect( myData, host, login, password, nullptr, MYSQL_PORT,
			      nullptr, 0 ) ){
			if ( mysql_select_db( myData, szDB ) < 0 ) {
//					MessageBox("Does database exist?","Database selection failed!",MB_OK | MB_ICONERROR);
					mysql_close( myData ) ;
					return false;
			}
	}
	else {
  //			  MessageBox("Is MySQL server running?","Unable to connect to server!",MB_OK | MB_ICONERROR);
			mysql_close( myData ) ;
	        return false;
	}
// General Tables
	if(SQL_Call("CREATE TABLE DefVersion (Version VARCHAR(40))","DefVersion")) 
		sSql="INSERT INTO DefVersion VALUES ('"+m_ExeFileDate+"')";
	else {
		SQL_Call("SELECT * FROM DefVersion;","Get version");
		int i=SQL_RES();
		if(i>0)	 {
			string PreviousVersion=m_SQL_Lines[0];
		if(PreviousVersion!=m_ExeFileDate) {
			string message="Version of Current CoupModel does not correspond to pDatabase-> Do you want update mySQL tables ? ";
			int Currentchoice=MFC_Util::MessageBox( LPCTSTR(message),_T("Create mySQL Database"),MB_YESNO|MB_ICONQUESTION|MB_TASKMODAL);
			if(Currentchoice==IDYES) {
				SQL_UPDATEVERSION();
			}; 
		}
		}
		sSql="UPDATE DefVersion SET Version='"+m_ExeFileDate+"';";
	}
	SQL_Call(sSql, "Insert");

	sSql = "CREATE TABLE simulationinfo (Id INT ZEROFILL AUTO_INCREMENT, RunNo INT,StartDate DATETIME,EndDate DATETIME,";

		SQL_Call( SqlCreateRunInfoTable1(),"No");
		if(!SQL_Call( SqlCreateRunInfoTable2(),"No")) {
			string message="Database already exist Do you want exit from current change of mySQL tables ? ";
			int Currentchoice=MFC_Util::MessageBox( LPCTSTR(message),_T("Create mySQL Database"),MB_YESNO|MB_ICONQUESTION|MB_TASKMODAL);
			if(Currentchoice==IDYES) {
				return false;
			}; 
		}

		SQL_Call(SqlCreateValTable(),"No");

	for(int j=0;j<8;j++) {	
	//Choose the current name for the type
	switch (j)	{
		case 0:	
			typ="Parameters";	
  		break;
		case 1:	
			typ="Switches";
   		break;
		case 2:	
			typ="Tables";
    		break;
		case 3:	typ="State Variables";			break;
		case 4:	typ="Flow Variables";			break;
		case 5:	typ="Auxiliary Variables";		break;
		case 6:	typ="Driving Variables";		break;
		case 7:	
			typ="Files";
	};
			
	string Namnvektor, Groupvektor; Groupvektor=Namnvektor="ENUM (";	
   	pos=GetFortranMap()->reg.GetGroupStart();
	int i=0;

	while (pos!=nullptr) {
		//Get the array with this type and the current group
		if(GetFortranMap()->reg.GetArray(typ,grp,arr)) {
			Sw=arr->GetAt(0);
			//namn=Sw->GetGroup()+"\"H"+Sw->GetGroupID();
			//Groupvektor+="'"+Sw->GetGroup()+"',";
			i++;

		}
		GetFortranMap()->reg.GetNextGroup(pos,grp);
		   //	Sw=arr->GetAt(0);
		  //	namn=Sw->GetGroup()+"\"H"+Sw->GetGroupID();
			Groupvektor+="'"+grp+"',";
			i++;

	}
	Groupvektor.TrimRight(_T(","));
	Groupvektor+=")";
	string Typvektor;
	Typvektor="ENUM ('Switches','Parameters','Tables','State Variables','Flow Variables','Auxiliary Variables','Driving Variables','Input Files')";
	sSql="CREATE TABLE GroupsUsed (Id Int ZEROFILL, Num TINYINT,Groups "+Groupvektor+",Types "+Typvektor+", "+
						"PRIMARY KEY (Id, Num), FOREIGN KEY(Id) REFERENCES simulationinfo (Id) ON DELETE CASCADE) TYPE = INNODB;"	;

	if(j==0) SQL_Call(sSql,"Files"); 

	pos=GetFortranMap()->reg.GetGroupStart();
	int jj=0;
	//Traverse all groups
	while(pos!=nullptr) {
		int i=0;
		jj=jj+1;
	//Get next group
		GetFortranMap()->reg.GetNextGroup(pos,grp);
	//Get the array with this type and the current group
		if(GetFortranMap()->reg.GetArray(typ,grp,arr)){
   			bool isgroup=true;
			Namnvektor="ENUM (";
			//Loop through all the variables in the array to see whether they are disabled or not
				while(i<arr->GetSize())	{
		   ///		int jjj=arr->GetSize();

		   			Sw=arr->GetAt(i);
					Namnvektor+="'"+Sw->GetName()+"'," ;


		   //		if(!Sw->IsDisabled())
		   //			isgroup=true;
		   		i++;
		   	}
			Namnvektor.TrimRight(_T(","));
			Namnvektor+=")";
			namn=Sw->GetGroup()+"`";
					switch (j)	{
						case 0:	
							typ="Parameters";
							// Create SQL command
							sSql="CREATE TABLE `ValP_"+namn+" (Id Int ZEROFILL, Num TINYINT ZEROFILL, Parameter "+Namnvektor+", "+
								"PRIMARY KEY (Id,Num), FOREIGN KEY(Id) REFERENCES simulationinfo (Id) ON DELETE CASCADE) TYPE = INNODB;"	;
							SQL_Call(sSql, "Par"); 
  							break;
						case 1:	
							typ="Switches";
  							// Create SQL command
							sSql="CREATE TABLE `ValSw_"+namn+" (Id Int ZEROFILL, Num TINYINT ZEROFILL, Switch "+Namnvektor+", "+
								"PRIMARY KEY (Id,Num), FOREIGN KEY(Id) REFERENCES simulationinfo (Id) ON DELETE CASCADE) TYPE = INNODB;"	;
							SQL_Call(sSql, "Par"); 
   						break;
						case 2:	
							typ="Tables";
							// Create SQL command
							sSql="CREATE TABLE `ValPTable_"+namn+" (Id Int ZEROFILL, Num TINYINT ZEROFILL, ParameterTable "+Namnvektor+", "+
								"PRIMARY KEY (Id,Num), FOREIGN KEY(Id) REFERENCES simulationinfo (Id) ON DELETE CASCADE) TYPE = INNODB;"	;
							SQL_Call(sSql, "Par"); 
							break;
						case 3:	typ="State Variables";
							// Create SQL command
							sSql="CREATE TABLE `ValState_"+namn+" (Id Int ZEROFILL, Num TINYINT ZEROFILL, State "+Namnvektor+", LIndex TINYINT, "+
								"PRIMARY KEY (Id,Num), FOREIGN KEY(Id) REFERENCES simulationinfo (Id) ON DELETE CASCADE) TYPE = INNODB;"	;
							SQL_Call(sSql, "Par"); 
  							break;
						case 4:	typ="Flow Variables";
							// Create SQL command
							sSql="CREATE TABLE `ValFlow_"+namn+" (Id Int ZEROFILL, Num TINYINT ZEROFILL, Flow "+Namnvektor+", LIndex TINYINT, "+
								"PRIMARY KEY (Id,Num), FOREIGN KEY(Id) REFERENCES simulationinfo (Id) ON DELETE CASCADE) TYPE = INNODB;"	;
							SQL_Call(sSql, "Par"); 
  							break;
						case 5:	typ="Auxiliary Variables";
							// Create SQL command
							sSql="CREATE TABLE `ValAuxiliary_"+namn+" (Id Int ZEROFILL, Num TINYINT ZEROFILL, Auxiliary "+Namnvektor+", LIndex TINYINT,"+
								"PRIMARY KEY (Id,Num), FOREIGN KEY(Id) REFERENCES simulationinfo (Id) ON DELETE CASCADE) TYPE = INNODB;"	;
							SQL_Call(sSql, "Par"); 
  							break;
						case 6:	typ="Driving Variables";
							// Create SQL command
							sSql="CREATE TABLE `ValDrivings_"+namn+" (Id Int ZEROFILL, Num TINYINT ZEROFILL, Driving "+Namnvektor+", LIndex TINYINT, "+
								"PRIMARY KEY (Id,Num), FOREIGN KEY(Id) REFERENCES simulationinfo (Id) ON DELETE CASCADE) TYPE = INNODB;"	;
							SQL_Call(sSql, "Par"); 
  							break;
						case 7:	
							typ="Files";
							// Create SQL command
							sSql="CREATE TABLE `ValFiles_"+namn+" (Id Int ZEROFILL, Num TINYINT, Files "+Namnvektor+", "+ 
								"PRIMARY KEY (Id,Num), FOREIGN KEY(Id) REFERENCES simulationinfo (Id) ON DELETE CASCADE) TYPE = INNODB;"	;
							SQL_Call(sSql, "Par"); 
  
					};			

			//If any parameter in the array isn´t disabled then isgroup is now true
			//and the following while loop is executed
			if(isgroup)	{
				i=0;
					//Loop through the whole array
				while(i<arr->GetSize())	{
					//Get the parameter
					Sw=arr->GetAt(i);
					namn=Sw->GetName();

//					NoDefault=true;
					if(typ=="Parameters") {
		 				string u=Sw->GetUnit();
						float defv;
						defv=*(float*)Sw->GetOriginalValue();
						namn=Sw->GetName();
						grp=Sw->GetGroup();
						grp.Replace(_T(" "),_T(""));
						namn.Replace(_T(" "),_T(""));

//						if(Currentchoice==IDNO&&Changed_Values==IDNO) v=defv;
//								else  v=*(float*)Sw->GetValue();
						string strdefv=FtoAscii(defv);
						sSql=SqlCreateParTable(Sw->GetGroupIDNumber(),Sw->GetName(),strdefv);
						SQL_Call(sSql, "Par"); 

					}
					else if(typ=="Switches"){
									//Set the name to the string to be written
  						int defnval;
  						defnval=*(int*)Sw->GetOriginalValue();
						CSw*node=(CSw*)Sw;
						string EnumString=" ENUM(";
					    for (int k=0;!node->GetOption(k).IsEmpty();k++) {
							EnumString+="'"+node->GetOption(k)+"',";
						}
						EnumString.TrimRight(_T(","));
						EnumString+=")";
					   
						string strdefv="'"+node->GetOption(defnval)+"'";
						 sSql=SqlCreateSwitchTable(Sw->GetGroupIDNumber(),Sw->GetName(),EnumString, strdefv);
						SQL_Call(sSql, "Par"); 
					}
	
			   		else if(typ=="Files") {
			   		  	//Set the name to the string to be written
			   		  	CF *node=(CF*)Sw;
			   		  	string val=node->GetValue();
					  	namn=Sw->GetName();
					  	grp=Sw->GetGroup();
					  	grp.Replace(_T(" "),_T(""));
					  	namn.Replace(_T(" "),_T(""));

			   		  	sSql=SqlCreateFileTable(Sw->GetGroupIDNumber(),Sw->GetName(),val);
						SQL_Call(sSql, "Par"); 
	   				}
			   		else if(typ=="Tables")	{
		   				int nval=*(int*)((CTab*)Sw)->GetValue();
	   				//If the parameter isn´t disabled
	   				
					   //	namn.Format(namn+" No of elements in Table: %d", nval);
					   //	ParFile.WriteString(namn+"\n");
						string Label="\"Index\"", text, unit;
						CPt *pPt;
						string Row;
						float fValue, defv;
						int nCol;
					
						//Create string to write for the labels of the columns
						for (nCol=0;((CTab*)Sw)->GetTitle(nCol,&text);nCol++){
							pPt=((CTab*)Sw)->GetCPtPointer(nCol);
							if(pPt!=nullptr) {
								int nRow=0; 

								((CTab*)Sw)->GetCell(nCol, nRow,&fValue);
								defv=pPt->GetOriginalValue(nRow);
								string strdefv=FtoAscii(defv);
								((CTab*)Sw)->GetTitle(nCol,&Label);
								grp=Sw->GetGroup();
								sSql=SqlCreateVectorParTable(Sw->GetGroupIDNumber(),Sw->GetName(),Label, strdefv);	
								SQL_Call(sSql, "Par"); 
			   				 }
			   			}
			   					
					}
			   		else if(typ=="State Variables"||typ=="Flow Variables")	{
			   		  	//Write the status of the auxiliary and driving flags
			   		  	//namn+=Sw->GetUnit();
				  	  		string typSum="SumX";
					  		if(typ=="Flow Variables") typSum="SumT";
					  		sSql=SqlCreateSumTable(typSum,Sw->GetGroupIDNumber(),Sw->GetName());	
							SQL_Call(sSql, "Par"); 
 	   				}
		   			else 	{
					   	grp=Sw->GetGroup();
					   	grp.Replace(_T(" "),_T(""));
					   	namn.Replace(_T(" "),_T(""));
					   	string typSum="SumG";
					   	if(typ=="Driving Variables") typSum="SumD";
					   	sSql=SqlCreateSumTable(typSum,Sw->GetGroupIDNumber(),Sw->GetName());	
						SQL_Call(sSql, "Par"); 
				   	   	//Write the status of the auxiliary and driving flags
			   		  //	namn+="\t\""+Sw->GetUnit();
			   		}

			   			//if the parameter isn´t disabled then write the parameter
//			   			if((Currentchoice==IDNO&&typ!="Tables")||(!Sw->IsDisabled()&&typ!="Tables"&&Changed_Values==IDNO)
//			   				||(!Sw->IsDisabled()&&typ!="Tables"&&NoDefault==true))
//			   				  	ParFile.WriteString(namn+"\n");
			   			//Increase the counter for the array traversal
			   		i++;
				}
			}
		string GroupReady;

		}

	}
//	m_SavedSum=true;
	}
	*/
	return true;
}
bool Link::SQL_WRITE() {
	/*
	CString		grp,typ, status;
	CBasePtrArray *arr;
	CBase *Sw;
	CStdioFile ParFile;
	CString namn;
	CString Name, Group, Type;
	CString pzPathName=CDocument::GetPathName();
	int editflag    = 0;          // Flag whether to edit record
    CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // exactly the same name as in the ODBC-Manager
//  CString sExcelFile = fileName;                // Filename and path for the file to be created
//	CString sDriver = "MICROSOFT ACCESS DRIVER (*.MDB)"; // exactly the same name as in the ODBC-Manager
	CString sSql, Last_Id;
	CString Line,temporary_String;
 	CString m_User,m_Computer;
   
 		char cHost[256] = "pej_home"; // mysql server is installed on a local PC
		char szDB[]     = "CoupModel" ;  // the database that we will use
		char szSQL[]    = "SELECT * FROM kalle"; // a default SQL statement
		CWinApp* pApp = AfxGetApp();
		CString strApp=((CCoupModelApp*)pApp)->m_AppString;
			// Get Last ID
 		 
		CString login, host, password;
		host=pApp->GetProfileString(strApp  ,_T("MySQL_Server")  , host);    
		login=pApp->GetProfileString(strApp    ,_T("MySQL_User")    , login);        
		password=pApp->GetProfileString(strApp,_T("MySQL_Password"), password); 

		m_User=pApp->GetProfileString(strApp, _T("UserName"), m_User);
		m_Computer=pApp->GetProfileString(strApp, _T("ComputerName"), m_Computer);

		if( (myData = mysql_init((MYSQL*) 0)) &&
                  mysql_real_connect( myData, host, login, password, nullptr, MYSQL_PORT,
			      nullptr, 0 ) ){
				if ( mysql_select_db( myData, szDB ) < 0 ) {
					if ( mysql_select_db( myData,"CREATE DATABASE coupmodel;") < 0 )	{
						MFC_Util::MessageBox(_T("Does database exist?"),_T("Database selection failed!"),MB_OK | MB_ICONERROR);
						mysql_close( myData ) ;
						return false;
					}
					mysql_select_db( myData, szDB );
				}

			}
		else {
			  MFC_Util::MessageBox( _T("Is MySQL server running?"),_T("Unable to connect to server!"),MB_OK | MB_ICONERROR);
				mysql_close( myData ) ;
	          return false;
		}
		status="Start Writing to MySQL database at: "+host+" using login at:"+login;
		SetRunReportArray(status);
		SetRunReportArray(" PLEASE WAIT for completions");

 		//m_pView->writeStatus(status);
	POSITION posd=GetFirstViewPosition();
//	m_pView=(CCoupModelView*)GetNextView(posd);
   //	CMainFrame *p_parent;
   //	p_parent = (CMainFrame*)GetParentFrame();
   //	p_parent->SetMessageText(message);
 

//		((CCoupModelView*)m_pView)->writeStatus(status);
//	m_pView->Invalidate();

		int imonth=0; int ifind=-1;
		CString DateTimeQ;
		for (imonth=1; imonth<13; imonth++)	{
			time_t CMonth(1980,imonth,1,1,1,1);
			CString CMon=CMonth.Format("%B");
			int ifind=m_DateRun.Find(CMon);
			if(ifind==0) {
				   int i1=m_DateRun.Find(_T(","));
 				   DateTimeQ.Format(_T("%s-%.2d-%s %s"),m_DateRun.Mid(i1+2,4), imonth, m_DateRun.Mid(i1-2,2), m_DateRun.Mid(i1+8,8));
				   imonth=13;
			}
		}
	 
		CString TimeReport="";
		int ncount=m_RunReport_Array.GetSize();
		if(ncount>0) {
			CString TimeUsed=m_RunReport_Array.GetAt(ncount-1);
			int ipos=TimeUsed.Find(_T("used"));
			if(ipos>0) {
				CString hour=TimeUsed.Mid(ipos+5,2);
				CString minut=TimeUsed.Mid(ipos+11,2);
				CString sekund=TimeUsed.Mid(ipos+18,2);
				TimeReport.Format(_T("%s:%s:%s"),hour,minut,sekund);
				TimeReport.Replace(_T(" "),_T("0"));
			}
		}
		
			// MultiRunInformation
  		CString MR_USE="No";
  		if(PARAMETERS_mp_NODIMCOUNTERS>0) MR_USE="Yes";

  		Name="TimeResolution";
  		Group="Hidden";
  		Type="Switches";
  		Sw=GetDocFortranMap()->reg.GetPtr(Name, Type, Group, arr);
  		int Timeres=*(int*)Sw->GetValue();
  		ncount=m_P_CH_Array.GetSize();
  		CString CHAPAR_USE="No";
  		if(ncount>0) CHAPAR_USE="Yes";

			
			
		sSql.Format(_T("INSERT INTO simulationinfo VALUES(nullptr,%i,'%s','%s',%i,%i,%i,'%s','%s',%i,%i,%i,'%s','%s');"), 
		m_SimulationRunNo,StringDatum(Fort.RunOpt.longMinStart),StringDatum(Fort.RunOpt.longMinEnd),Fort.RunOpt.oidays*1440+Fort.RunOpt.oiminutes,Fort.RunOpt.noofiter,
				  m_TimePeriodScaleFactor, CHAPAR_USE, MR_USE, 	MR_Get_ActualRunNumber(), MR_Get_TotalNumberofRuns(),MR_GetNumberOfDimensions(), m_RunId, m_Comments);
		SQL_Call(sSql, "Insert");
		SQL_Call("SELECT LAST_INSERT_ID();", "No" );
		int i=SQL_RES();
		if(i>0)
			Last_Id=m_SQL_Lines[0];
		CString RunNotest;
		RunNotest.Format(_T("%i && RunDate = '%s'"),m_SimulationRunNo, DateTimeQ);
		sSql="SELECT RunNo, RunDate FROM simulationinfo, runinfo WHERE RunNo="+RunNotest+";";
		SQL_Call(sSql,"Select");
		if(SQL_RES()>0)	  {
			int problem=10;
			MFC_Util::MessageBox(sSql,_T("SQL Warning Record already exist - No action taken"),MB_OK|MB_ICONEXCLAMATION);
			return false;
		}
		char m_eComments[256];
		mysql_real_escape_string(myData,m_eComments, m_CurrentFile, strlen(m_CurrentFile));
		char m_eCurrentDirectory[256];
		mysql_real_escape_string(myData,m_eCurrentDirectory, m_CurrentDirectory, strlen(m_CurrentDirectory));

		sSql.Format(_T("INSERT INTO runinfo (Id, RunDate,Time_Used,Version,Directory,FileName,User,Host) VALUES(%s,'%s','%s','%s','%s','%s','%s','%s');"), 
			Last_Id, DateTimeQ, TimeReport, m_ExeFileDate, m_eCurrentDirectory, m_eComments,m_User,m_Computer);
		SQL_Call(sSql, "Table2"); 
		int iTestPrev, NumLines, lindex;
		bool selected;
		CString TestPrevGrp, TestGrp, test, TableName;
	  	CF* pValFile;

// Validation Table
		pValFile=ValidationFilePointer();
		NumLines=pValFile->GetNumberOfSelectedVal();
		if(NumLines>0) SetRunReportArray(_T("Storing of Validation results"));
//		m_pView->Invalidate();

		for(int i=0; i<NumLines; i++)	{
		 	int ind=m_ValidationData.GetValFileIndex(i);
			test=m_ValidationData.GetValName(i);
			int iv=m_ValidationData.GetValLocalIndex(i);
			if(iv>-1) test+="("+ItoAscii(iv+1)+")";		
			sSql=SqlInsertToValTable(Last_Id, i+1,"Validation",pValFile->GetDataDescription(ind+1),test, m_ValidationData.GetValn(i),
				m_ValidationData.GetValR2(i),m_ValidationData.GetValA0(i),m_ValidationData.GetValA1(i),m_ValidationData.GetValME(i),
				m_ValidationData.GetValRMSE(i),m_ValidationData.GetValMSIM(i),m_ValidationData.GetValMVAL(i));
			SQL_Call(sSql, "Val");
		}
		ncount=m_SW_Array.GetSize();
		if(ncount>0)  SetRunReportArray("Storing of Switch values");
		//m_pView->Invalidate();


		CString previousgroup=" ";
		int igr=0;
		typ="Switches";
		SetRunReportArray("Switches");
		for(int i=0; i<ncount; i++) {
			Sw = (CBase*)m_SW_Array.GetAt(i);	
			CString namnsw=Sw->GetName();
			int nval=*(int*)Sw->GetValue();
			CSw *node=(CSw*)Sw;
			CString soption=node->GetOption(nval); 
			if(Sw->GetGroup()!=previousgroup) {
				igr++;
				sSql.Format(_T("INSERT INTO groupsused VALUES (%s,%d,'%s','%s');"),Last_Id,igr,Sw->GetGroup(), typ);
				SQL_Call(sSql,"Groups Used");
				previousgroup=Sw->GetGroup();
			}
			sSql.Format(_T("INSERT INTO `ValSw_%s` VALUES (%s,%d,'%s');"), Sw->GetGroup(),Last_Id, i+1, namnsw);
			SQL_Call(sSql," - Sw");
		    SQL_Call(SqlInsertToSwitchTable(Sw->GetGroupIDNumber(),namnsw, soption, Last_Id)," - Switches");
		}

//Parameters		
		
		ncount=m_P_Array.GetSize();
		previousgroup=" ";
  		typ="Parameters";
		if(ncount>0)  SetRunReportArray("Storing of Parameter values");
		//m_pView->Invalidate();


		for(int i=0; i<ncount; i++) {
		  Sw = (CBase*)m_P_Array.GetAt(i);	
		  float v=*(float*)Sw->GetValue();
		  namn=Sw->GetName();
			if(Sw->GetGroup()!=previousgroup) {
				igr++;
				sSql.Format(_T("INSERT INTO groupsused VALUES (%s,%d,'%s','%s');"),Last_Id,igr,Sw->GetGroup(), typ);
				SQL_Call(sSql,"Groups Used");
				previousgroup=Sw->GetGroup();
			}
		  sSql.Format(_T("INSERT INTO `ValP_%s` VALUES (%s,%d,'%s');"), Sw->GetGroup(),Last_Id, i+1, namn);
		  SQL_Call(sSql, "ValPar");
		  SQL_Call(SqlInsertToParTable(Sw->GetGroupIDNumber(),namn, v,  Last_Id),"Insert Par");
   		}

		//Parameter Tables
		ncount=m_TAB_Array.GetSize();
		if(ncount>0)  SetRunReportArray("Storing of Parameter Table values");
	//	m_pView->Invalidate();


		previousgroup=" ";
 		typ="Tables";
		for(int i=0; i<ncount; i++) {
			Sw = (CBase*)m_TAB_Array.GetAt(i); 
			namn=Sw->GetName();
			if(Sw->GetGroup()!=previousgroup) {
				igr++;
				sSql.Format(_T("INSERT INTO groupsused VALUES (%s,%d,'%s','%s');"),Last_Id,igr,Sw->GetGroup(), typ);
				SQL_Call(sSql,"Groups Used");
				previousgroup=Sw->GetGroup();
			}
			float Value;
			sSql.Format(_T("INSERT INTO `ValPTable_%s` VALUES (%s,%d,'%s');"), Sw->GetGroup(),Last_Id, i+1, namn);
			SQL_Call(sSql," ValP");
// New implementation of Tables from m_FileVersion number 21
//			int ncol=((CTab*)Sw)->GetNumberOfColumns();
			for(int nCol=0; nCol<((CTab*)Sw)->GetNumberOfColumns();nCol++) {
				CString strTitle=((CTab*)Sw)->GetTitle(nCol);
				int numrow=*(int*)((CTab*)Sw)->GetValue();// Number of rows in table
				
				for(int nRow=0; ((CTab*)Sw)->GetCell(nCol, nRow,&Value); nRow++){
					SQL_Call(SqlInsertToVectorParTable(Sw->GetGroupIDNumber(),namn,strTitle, nRow, Value,Last_Id),"__ParTable");
					   
				}
			}														
		}
//File
		previousgroup=" ";
 
		ncount=m_F_Array.GetSize();
		if(ncount>0)  SetRunReportArray("Storing of File names");
	//	m_pView->Invalidate();


		typ="Input Files";
		for(int i=0; i<ncount; i++) {
			Sw = (CBase*)m_F_Array.GetAt(i);	
			namn=Sw->GetName();
			if(Sw->GetGroup()!=previousgroup) {
				igr++;
				sSql.Format(_T("INSERT INTO groupsused VALUES (%s,%d,'%s','%s');"),Last_Id,igr,Sw->GetGroup(), typ);
				SQL_Call(sSql,"Groups Used");
				previousgroup=Sw->GetGroup();
			}
			CF *node=(CF*)Sw;
			CString val=node->GetValue();
			char m_eCurrentFile[256];
			mysql_real_escape_string(myData,m_eCurrentFile, val, strlen(val));

			sSql.Format(_T("INSERT INTO `Valfiles_%s` VALUES (%s,%d,'%s');"), Sw->GetGroup(),Last_Id, i+1, namn);
		   	SQL_Call(sSql," - Sw");
		    SQL_Call(SqlInsertToFileTable(Sw->GetGroupIDNumber(),namn, m_eCurrentFile, Last_Id)," - Switches");
		}
// Outputs
		iTestPrev=0;

//		Summary Table State Variable
	  	NumLines=m_XTGD_Array.GetSize();
		int countx,countt,countg,countd;
		countx=countt=countg=countd=0;
		if(ncount>0)  SetRunReportArray("Storing of Output Variables");
	//	m_pView->Invalidate();

	 
		for(int i=0; i<NumLines; ++i)	{
			CBase *Sw = (CBase*)m_XTGD_Array.GetAt(i);	
			int itest=m_XTGD_Type_Array.GetAt(i);
			if(itest!=iTestPrev) {
				TestPrevGrp="  ";	
				switch (itest){
				case 1:
					test="State_Variables";
  					TableName=test;
				   	break;
				case 2:
					test="Flow_Variables";
					TableName=test;
					break;
				case 3:
					test="Auxiliary_Variables";
					TableName=test;
					break;
				case 4:
					test="Driving_Variables";
					TableName=test;
					break;
				}
				test.Replace(_T("_"),_T(" "));
			   
				iTestPrev=itest;
			}
		



			if (Sw->IsKindOf(RUNTIME_CLASS(CGD))){
				for(int ii=0;ii<((CGD*)Sw)->GetNumberOfFlags();ii++){
					if(((CGD*)Sw)->GetFlag(ii, &selected))			 
						if(selected) {
							TestGrp=Sw->GetGroup();
							if(TestGrp!=TestPrevGrp) {
//									ParFile.WriteString(Line);
									igr++;
									sSql.Format(_T("INSERT INTO groupsused VALUES (%s,%d,'%s','%s');"),Last_Id,igr,Sw->GetGroup(), test);
									SQL_Call(sSql,"Groups Used");
									TestPrevGrp=TestGrp;
							}
							CString RunNo, Lind, grp, namn	;
							namn=Sw->GetName();
							lindex=((CGD*)Sw)->GetLocalIndex(ii);
							Lind="1001";
							if(lindex>-1&&((CGD*)Sw)->GetNumberOfFlags()>1) {
								Lind=ItoAscii(ii+1);
								lindex=ii+1;

							}
							
					  		CString typSum="SumG";
					  		if(TableName=="Driving_Variables") typSum="SumD";

							sSql=SqlInsertToSumTable(typSum,Sw->GetGroupIDNumber() , namn, Lind, ((CGD*)Sw)->GetInitial(ii),((CGD*)Sw)->GetFinal(ii),((CGD*)Sw)->GetMin(ii), ((CGD*)Sw)->GetMax(ii),((CGD*)Sw)->GetMean(ii),((CGD*)Sw)->GetAccumulated(ii),Last_Id);
							SQL_Call(sSql," InsertSumGD");
							if(typSum=="SumG") { countg++;
								 sSql.Format(_T("INSERT INTO `ValAuxiliary_%s` VALUES (%s,%i,'%s',%i);"),TestGrp, Last_Id, countg, namn, lindex+1);
							}
							else  {
								countg++;
								sSql.Format(_T("INSERT INTO `ValDrivings_%s` VALUES (%s,%i,'%s',%i);"),TestGrp, Last_Id, countg, namn, lindex+1);
							}
							SQL_Call(sSql,"Sumgd");
							
					}
				}
			}

			else {
				for(int ii=0;ii<((CGD*)Sw)->GetNumberOfFlags();ii++){
					if(((CXT*)Sw)->GetFlag(ii, &selected))			 
						if(selected) {
							TestGrp=Sw->GetGroup();
							if(TestGrp!=TestPrevGrp) {
//									ParFile.WriteString(Line);
									igr++;
									sSql.Format(_T("INSERT INTO groupsused VALUES (%s,%d,'%s','%s');"),Last_Id,igr,Sw->GetGroup(), test);
									SQL_Call(sSql,"Groups Used");
									TestPrevGrp=TestGrp;
							}
							CString RunNo, Lind, grp, namn	;
							namn=Sw->GetName();
							lindex=((CXT*)Sw)->GetLocalIndex(ii);
							Lind="1001";
							if(lindex>-1&&((CGD*)Sw)->GetNumberOfFlags()>1) {
								Lind=ItoAscii(ii+1);
								lindex=ii+1;
							}
						   					  		
					  		CString typSum="SumX";
					  		if(TableName=="Flow_Variables") typSum="SumT";

							sSql=SqlInsertToSumTable(typSum,Sw->GetGroupIDNumber() , namn, Lind, ((CGD*)Sw)->GetInitial(ii),((CGD*)Sw)->GetFinal(ii),((CGD*)Sw)->GetMin(ii), ((CGD*)Sw)->GetMax(ii),((CGD*)Sw)->GetMean(ii),((CGD*)Sw)->GetAccumulated(ii),Last_Id);
							SQL_Call(sSql," InsertSumXT");

							if(typSum=="SumX") {
								countx++;
								 sSql.Format(_T("INSERT INTO `ValState_%s` VALUES (%s,%i,'%s',%i);"),TestGrp, Last_Id, countx, namn,lindex);
							}
							else { countt++;
								sSql.Format(_T("INSERT INTO `ValFlow_%s` VALUES (%s,%i,'%s',%i);"),TestGrp, Last_Id, countt, namn,lindex);
							}
							SQL_Call(sSql,"-  SumXT");
						}
 					}
 				}
			}
			*/
		return true; 
}
#endif
bool Link::WriteFile(string str)
{ 
//REMAIN
/*	POSITION	pos;
	CString		grp,typ, status;
	CBasePtrArray *arr;
	CBase *Sw;
	CStdioFile ParFile;
	CString namn;
	CString pzPathName=m_Map->GetCurrentSimFile().c_str();
	CString fileName=str.c_str();

	CDatabase* pDatabase;
    CDatabase database;
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // exactly the same name as in the ODBC-Manager
//  CString sExcelFile = fileName;                // Filename and path for the file to be created
//	CString sDriver = "MICROSOFT ACCESS DRIVER (*.MDB)"; // exactly the same name as in the ODBC-Manager
	CString sExcelFile = fileName;                // Filename and path for the file to be created
  CString sSql;
	CString Line,temporary_String;
    
	CCoupModelApp* pApp;

	pApp = (CCoupModelApp*)AfxGetApp();
    // Build the creation string for access without DSN
	POSITION posd=GetFirstViewPosition();
//	m_pView=(CCoupModelView*)GetNextView(posd);

    
	status="Wait while writing summary to excel file";
//	((CCoupModelView*)m_pView)->writeStatus(status);
	m_Map->SetRunReportArray(status);
//	m_pView->Invalidate();


    bool NewFile;
	sDriver=GetExcelDriver();
    sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=false;OPEN_DB=\"%s\";DBQ=%s"), sDriver,sExcelFile,sExcelFile);
//    sSql.Format("DRIVER={%s};DSN='';READONLY=false;CREATE_DB=\"%s\";DBQ=%s", sDriver,sExcelFile,sExcelFile);

	
	pDatabase =new CDatabase;
	TRY {
    // Create the database (i.e. Excel sheet)
    if( pDatabase->OpenEx(sSql,CDatabase::noOdbcDialog) ) {
		NewFile=false;
	}
	else {
			sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=false;CREATE_DB=\"%s\";DBQ=%s"), sDriver,sExcelFile,sExcelFile);
			NewFile=true;
			pDatabase->OpenEx(sSql,CDatabase::noOdbcDialog); 
	}
	}
	CATCH_ALL (e) {

	  	sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=false;CREATE_DB=\"%s\";DBQ=%s"), sDriver,sExcelFile,sExcelFile);
			NewFile=true;
			pDatabase->OpenEx(sSql,CDatabase::noOdbcDialog); 


	}
	END_CATCH_ALL;

  TRY
  {

	 {
      
//	  sSql = "CREATE TABLE RunInfo (CoupModel TEXT, Info TEXT)";
//      pDatabase->ExecuteSQL(sSql);
		sSql=SqlCreateRunInfoTable();
		TRY {	pDatabase->ExecuteSQL(sSql); }
		CATCH_ALL(e)
		{ 		   NewFile=true;	}
		END_CATCH_ALL;
		float likelihood1, likelihood2;
		likelihood1=float(GENERAL_mp_LOGL1);
		likelihood2=float(VALIDATION_mp_EXTRA_LIKELIHOOD);
		sSql=SqlInsertToRunInfoTable(m_SimulationRunNo,m_DateRun,m_ExeFileDate,StringDatum(Fort.RunOpt.longMinStart),StringDatum(Fort.RunOpt.longMinEnd),Fort.RunOpt.oidays*1440+Fort.RunOpt.oiminutes,Fort.RunOpt.noofiter, m_RunId, m_Comments,m_CurrentDirectory, m_CurrentFile,PARAMETERS_mp_PRESIMULATIONDAYS,likelihood1, likelihood2);
		TRY(pDatabase->ExecuteSQL(sSql));
		CATCH_ALL(e){
			::countinfo++;
			sSql=SqlCreateRunInfoTable();
			pDatabase->ExecuteSQL(sSql);
			sSql=SqlInsertToRunInfoTable(m_SimulationRunNo,m_DateRun,m_ExeFileDate,StringDatum(Fort.RunOpt.longMinStart),StringDatum(Fort.RunOpt.longMinEnd),Fort.RunOpt.oidays*1440+Fort.RunOpt.oiminutes,Fort.RunOpt.noofiter, m_RunId, m_Comments,m_CurrentDirectory, m_CurrentFile,PARAMETERS_mp_PRESIMULATIONDAYS,likelihood1, likelihood2);		
			pDatabase->ExecuteSQL(sSql);
		}
		END_CATCH_ALL;




		int iTestPrev, NumLines, lindex;
		bool selected;
		CString TestPrevGrp, TestGrp, test, TableName;
	  	CF* pValFile;

		iTestPrev=0;
//  
	// New section for multirun information
    NumLines=0;
	CP *pPar;
	CPt* pCPt;
	CString strname,strkey, CreateString, InsertString;
	float Ratio, Float, min, max;
	int	ii, irep, icount, nTabIndex ;

	PARAMETERS_mp_NODIMCOUNTERS=MR_GetNumberOfDimensions();
	PARAMETERS_mp_ACTUALNOMULRUN=MR_Get_ActualRunNumber();
	PARAMETERS_mp_TOTALNOMULRUN=MR_Get_TotalNumberofRuns();
			sSql = "CREATE TABLE ";
			sSql +="MultiRun"+ItoAscii(::countmulti);
			sSql +=" (RunNo NUMBER";
		//	sSql+= "Dimensio";
		//	sSql+=" TEXT";
	if(MR_GetNumberOfDimensions()>0) {
	for(int i=0;i<MR_GetNumberOfDimensions();i++){
			irep=m_MultiRun_Array.GetAt(i);
			icount=MR_GetDimCounter(i);
			PARAMETERS_mp_DIMCOUNTERS[i]=icount;
			for(ii=0; ii<MR_Get_NumberOfParametersWithinDim(i+1);ii++){
				pPar=MR_Get_pPar(i+1,ii+1);
				nTabIndex=MR_GetTabIndex(i+1,ii+1);
				CString namep=pPar->GetName().c_str();
				if(nTabIndex>0) namep+=ItoAscii(nTabIndex+1);
				int N=namep.Replace(_T(" "),_T("_"));
				sSql+=","+namep;
				
				if(MR_GetMethod(i+1,ii+1)==5) {
					sSql+="DataBaseKey"+ItoAscii(i+1)+"_"+ItoAscii(ii+1)+" TEXT";

				}
				else
				{
					sSql+=" NUMBER";
				}
				//Check validation
			}
			
		}
        
		sSql+=", Acceptance NUMBER, Total_Likelihood NUMBER, Partial_Likelihood NUMBER, Best_RunNo NUMBER)";
		CreateString=sSql;
		TRY {	pDatabase->ExecuteSQL(sSql); }
		CATCH_ALL(e)
		{ 		   NewFile=true;	}
		END_CATCH_ALL;	
		sSql="INSERT INTO MultiRun"+ItoAscii(::countmulti)+" VALUES("+ItoAscii(m_SimulationRunNo)+",";
	for(int i=0;i<MR_GetNumberOfDimensions();i++){
			irep=m_MultiRun_Array.GetAt(i);
			icount=MR_GetDimCounter(i);
			PARAMETERS_mp_DIMCOUNTERS[i]=icount;
			for(ii=0; ii<MR_Get_NumberOfParametersWithinDim(i+1);ii++){
				pPar=MR_Get_pPar(i+1,ii+1);
				CString namep=pPar->GetName().c_str();
				nTabIndex=MR_GetTabIndex(i+1,ii+1);
				min=MR_GetMin(i+1,ii+1); // This variable does not always have realistic values.
				max=MR_GetMax(i+1,ii+1); // " "
				switch (MR_GetMethod(i+1,ii+1)) {
				case 0: Ratio=(float)(icount-1)/(float)(irep-1);// linear
						Float=Ratio*max+(1-Ratio)*min;
						break;
				case 1: //Logarithmic
					Ratio=(float)(icount-1)/(float)(irep-1);	
						Float=exp(Ratio*log(max)+(1-Ratio)*log(min));
						break;
				case 2:
				case 3:
				case 6:
				case 7:
				case 8:
				case 9:
				case 11:

						if(pPar->IsKindOf(RUNTIME_CLASS(CP))) {
							Float=*(float*)pPar->GetValue();
						}
							else {
								pCPt=(CPt*)pPar;
								Float=pCPt->GetValue(nTabIndex);
						}
						break; 
                case 4: // Table  
					    if (nTabIndex == -1) // Table, Note that ii is not incremented during MRs!!
					      Float = pPar->MR_Extract_Value(icount - 1);  // CP case
						else 
						  Float = pPar->MR_Extract_Value(nTabIndex, icount - 1);  // CP case
				       	break;
						// 	Float=MR_GetTableValue(i+1,ii+1,icount-1);				 	    break;
				case 5: // Database Records
						strname=((CDB*)pPar)->GetName();
						strkey=((CDB*)pPar)->MR_GetKeySelection(icount-1);
					    break;
				};
				if(MR_GetMethod(i+1,ii+1)!=5) {			
					CString stringv=FtoAscii(Float);
					sSql+=stringv+",";
				}
				else if(pPar->IsKindOf(RUNTIME_CLASS(CDB))) {
// Retrieve and apply
					CString stringv=strkey;
					sSql+="'"+stringv+"',";
				}

				//Check validation
			}
			

			}
			CString stringv;
			if(BAYESIANCALIB_mp_ACCEPTEDRUN)
				stringv="1,";
			else
				stringv="0,";

			sSql+=stringv;
			stringv=FtoAscii(likelihood1);
			sSql+=stringv+",";
			stringv=FtoAscii(likelihood2);
			sSql+=stringv+",";
			stringv=ItoAscii(BAYESIANCALIB_mp_BESTRUNNO);
			sSql+=stringv+",";
			sSql.TrimRight(_T(","));
			sSql+=")";
			InsertString=sSql;
			bool error=true;
			while(error) {
			TRY { pDatabase->ExecuteSQL(sSql); }
				error=false;
				CATCH_ALL(e) {
					::countmulti++;
					CreateString.Replace("MultiRun"+ItoAscii(::countmulti-1),"MultiRun"+ItoAscii(::countmulti));
					TRY{ pDatabase->ExecuteSQL(CreateString); }
					error=false;
					CATCH_ALL(e) {
						countmulti++;
						CreateString.Replace("MultiRun"+ItoAscii(::countmulti-1),"MultiRun"+ItoAscii(::countmulti));
						error=true;
						sSql=CreateString;
					}
					END_CATCH_ALL;
					InsertString.Replace("MultiRun"+ItoAscii(::countmulti-1),"MultiRun"+ItoAscii(::countmulti));
					pDatabase->ExecuteSQL(InsertString);

				}
				END_CATCH_ALL;
			}

	}
// Validation Table
		pValFile=ValidationFilePointer();
		NumLines=pValFile->GetNumberOfSelectedVal();
		if(NumLines>0) {

			sSql=SqlCreateValidation();
		TRY {	pDatabase->ExecuteSQL(sSql); }
		CATCH_ALL(e)
		{ 		   NewFile=true;	}
		END_CATCH_ALL;

		}		
				GENERAL_mp_LOGL1=0;
				for(int i=0;i<NumLines;i++) {
					GENERAL_mp_LOGL1+=m_ValidationData.GetValLogLi(i);
				}
				for(int i=0;i<m_ValidationData.GetNumSumVarVariables();i++) {
					GENERAL_mp_LOGL1+=m_ValidationData.GetValSumVarLogLi(i);
				}

				if(BAYESIANCALIB_mp_ACCEPTEDRUN||!((CCoupModelApp*)pApp)->Running) {
				for(int i=0; i<NumLines; i++){
					int ind=m_ValidationData.GetValFileIndex(i);
					test=m_ValidationData.GetValName(i);
					int iv=m_ValidationData.GetValLocalIndex(i);
					if(iv>-1) test+="("+ItoAscii(iv+1)+")";		
					CString sheet;
					sheet="Validation"+ItoAscii(countvalidation);
					sSql=SqlInsertToValTable(sheet,pValFile->GetDataDescription(ind+1),test, m_ValidationData.GetValn(i),
						m_ValidationData.GetValR2(i),m_ValidationData.GetValA0(i),m_ValidationData.GetValA1(i),m_ValidationData.GetValME(i),
						m_ValidationData.GetValRMSE(i),m_ValidationData.GetValMSIM(i),m_ValidationData.GetValMVAL(i), m_ValidationData.GetValLogLi(i),
						m_ValidationData.GetValP_Error(i), m_ValidationData.GetValA_Error(i), m_SimulationRunNo);
			//pDatabase->ExecuteSQL(sSql);

					TRY {	pDatabase->ExecuteSQL(sSql); }
					CATCH_ALL(e){
						::countvalidation++;
						sSql=SqlCreateValidation();
						pDatabase->ExecuteSQL(sSql);
						sheet="Validation"+ItoAscii(countvalidation);
						sSql=SqlInsertToValTable(sheet,pValFile->GetDataDescription(ind+1),test, m_ValidationData.GetValn(i),
							m_ValidationData.GetValR2(i),m_ValidationData.GetValA0(i),m_ValidationData.GetValA1(i),m_ValidationData.GetValME(i),
							m_ValidationData.GetValRMSE(i),m_ValidationData.GetValMSIM(i),m_ValidationData.GetValMVAL(i), m_ValidationData.GetValLogLi(i),
							m_ValidationData.GetValP_Error(i), m_ValidationData.GetValA_Error(i), m_SimulationRunNo);
						pDatabase->ExecuteSQL(sSql);
					}

					END_CATCH_ALL;
				}
			}


// Extra Validation Table

		NumLines=m_ValidationData.GetNumSumVarVariables();
		if(NumLines>0) {

			sSql=SqlCreateValSum();
			SetRunReportArray("Writing to: SumVariableValidation");
//			m_pView->Invalidate();

				TRY {	pDatabase->ExecuteSQL(sSql); }
				CATCH_ALL(e)
				{ 	
				
				}
				END_CATCH_ALL;


		}
		//New


		if(BAYESIANCALIB_mp_ACCEPTEDRUN||!((CCoupModelApp*)pApp)->Running) {
		for(int i=0; i<NumLines; i++)
		{
		 	test=m_ValidationData.GetValSumVarName(i);
			int iv=m_ValidationData.GetValSumVarLocalIndex(i);
			if(iv>-1) test+="("+ItoAscii(iv+1)+")";		
			float v2=m_ValidationData.GetValSumVarME(i);
			CString sheet;
			sheet="SumVariableValidation"+ItoAscii(::countvalsum);
			sSql=SqlInsertToValSumVarTable(sheet,test, m_ValidationData.GetStringValSumVarOutputType(i),
				m_ValidationData.GetValSumVarME(i),m_ValidationData.GetValSumVarRMSE(i),m_ValidationData.GetValSumVarMSIM(i),
				m_ValidationData.GetValSumVarMVAL(i), m_ValidationData.GetValSumVarLogLi(i),m_ValidationData.GetValSumVarP_Error(i),
				m_ValidationData.GetValSumVarA_Error(i), m_SimulationRunNo);

			TRY {	pDatabase->ExecuteSQL(sSql); }
			CATCH_ALL(e)
			{ 
				::countvalsum++;
				sheet="SumVariableValidation"+ItoAscii(::countvalsum);
				sSql=SqlCreateValSum();
				pDatabase->ExecuteSQL(sSql);
				sSql=SqlInsertToValSumVarTable(sheet,test, m_ValidationData.GetStringValSumVarOutputType(i),
					m_ValidationData.GetValSumVarME(i),m_ValidationData.GetValSumVarRMSE(i),
					m_ValidationData.GetValSumVarMSIM(i),m_ValidationData.GetValSumVarMVAL(i), 
					m_ValidationData.GetValSumVarLogLi(i),m_ValidationData.GetValSumVarP_Error(i),
					m_ValidationData.GetValSumVarA_Error(i), m_SimulationRunNo);
				pDatabase->ExecuteSQL(sSql);
			}
			END_CATCH_ALL;
		}
	}
		//END

// Extra Output of Validated Parameters

		NumLines=BAYESIANCALIB_mp_NP;
		if(NumLines>0) {
			PARAMETERS_mp_NODIMCOUNTERS=MR_GetNumberOfDimensions();
			PARAMETERS_mp_ACTUALNOMULRUN=MR_Get_ActualRunNumber();
			PARAMETERS_mp_TOTALNOMULRUN=MR_Get_TotalNumberofRuns();
			sSql = "CREATE TABLE ";
			sSql +="Calibrated_Parameters";
			sSql +="(PName TEXT";
	
			if(MR_GetNumberOfDimensions()>0) {
				for(int i=0;i<MR_GetNumberOfDimensions();i++){
					irep=m_MultiRun_Array.GetAt(i);
					icount=MR_GetDimCounter(i);
					PARAMETERS_mp_DIMCOUNTERS[i]=icount;
						for(ii=0; ii<MR_Get_NumberOfParametersWithinDim(i+1);ii++){
							pPar=MR_Get_pPar(i+1,ii+1);
							nTabIndex=MR_GetTabIndex(i+1,ii+1);
							CString namep=pPar->GetName().c_str();
							if(nTabIndex>=0) namep+=ItoAscii(nTabIndex+1);
							int N=namep.Replace(_T(" "),_T("_"));
							if(MR_GetMethod(i+1,ii+1)==9||MR_GetMethod(i+1,ii+1)==11) {
								sSql+=","+namep;
				
								if(MR_GetMethod(i+1,ii+1)==5) {
									sSql+="DataBaseKey"+ItoAscii(i+1)+"_"+ItoAscii(ii+1)+" TEXT";

								}
								else
								{
									sSql+=" NUMBER";
								}
							}
						}//Check validation
			}
			
		}
        
			sSql+=",RunNo NUMBER,TotalNoRuns NUMBER,AcceptedNoRuns NUMBER)";

			TRY {	pDatabase->ExecuteSQL(sSql); }
			CATCH_ALL(e)
			{ 		  	}
			END_CATCH_ALL;

		}											   

		if(BAYESIANCALIB_mp_INDEXRUN%50==49) {

		if(BAYESIANCALIB_mp_INDEXRUN==49) {
				// Min Value
				test = "INSERT INTO Calibrated_Parameters VALUES ('Min '";
				if(MR_GetNumberOfDimensions()>0) {
					for(int i=0;i<MR_GetNumberOfDimensions();i++){
						irep=m_MultiRun_Array.GetAt(i);
						icount=MR_GetDimCounter(i);
						PARAMETERS_mp_DIMCOUNTERS[i]=icount;
						for(ii=0; ii<MR_Get_NumberOfParametersWithinDim(i+1);ii++){
							if (MR_GetMethod(i+1,ii+1)==9||MR_GetMethod(i+1,ii+1)==11){
								float v=MR_GetMin(i+1,ii+1);
								test+=","+FtoAscii(v);
							}
						}
					}
				}
				sSql=test+","+ItoAscii(0)+","+ItoAscii(0)+","+ItoAscii(0)+")";
				pDatabase->ExecuteSQL(sSql);
				// Max Value
				test = "INSERT INTO Calibrated_Parameters VALUES ('Max '";
				if(MR_GetNumberOfDimensions()>0) {
					for(int i=0;i<MR_GetNumberOfDimensions();i++){
						irep=m_MultiRun_Array.GetAt(i);
						icount=MR_GetDimCounter(i);
						PARAMETERS_mp_DIMCOUNTERS[i]=icount;
						for(ii=0; ii<MR_Get_NumberOfParametersWithinDim(i+1);ii++){
							if (MR_GetMethod(i+1,ii+1)==9||MR_GetMethod(i+1,ii+1)==11){
								float v=MR_GetMax(i+1,ii+1);
								test+=","+FtoAscii(v);
							}
						}
					}
				}
				sSql=test+","+ItoAscii(0)+","+ItoAscii(0)+","+ItoAscii(0)+")";
				pDatabase->ExecuteSQL(sSql);
				// Start Value
				test = "INSERT INTO Calibrated_Parameters VALUES ('Start '";
				if(MR_GetNumberOfDimensions()>0) {
					for(int i=0;i<MR_GetNumberOfDimensions();i++){
						irep=m_MultiRun_Array.GetAt(i);
						icount=MR_GetDimCounter(i);
						PARAMETERS_mp_DIMCOUNTERS[i]=icount;
						for(ii=0; ii<MR_Get_NumberOfParametersWithinDim(i+1);ii++){
							if (MR_GetMethod(i+1,ii+1)==9||MR_GetMethod(i+1,ii+1)==11){
								float v=MR_GetStart(i+1,ii+1);
								test+=","+FtoAscii(v);
							}
						}
					}
				}
				sSql=test+","+ItoAscii(0)+","+ItoAscii(0)+","+ItoAscii(0)+")";
				pDatabase->ExecuteSQL(sSql);
		}
	       
		for(int i=0; i<NumLines; i++){
				test="INSERT INTO Calibrated_Parameters VALUES ('";
				test+=MR_Get_CalibParName(i);
				//+GetValSumVarName(i);
				//int iv=GetValSumVarLocalIndex(i);
				//if(iv>-1) test+="("+ItoAscii(iv+1)+")";		
				test+="'";
				for(int ii=0; ii<NumLines;ii++) {
		 			test+=","+FtoAscii(MR_Get_BayesCoVar(i,ii));
				}
				sSql=test+","+ItoAscii(m_SimulationRunNo)+","+ItoAscii(BAYESIANCALIB_mp_INDEXRUN)+","+ItoAscii(BAYESIANCALIB_mp_NUMIMPROVEDRUN)+")";
				pDatabase->ExecuteSQL(sSql);	
		}
	
		for(int i=0; i<NumLines; i++){
				test="INSERT INTO Calibrated_Parameters VALUES ('";
				test+=MR_Get_BayesPar(i);
				//+GetValSumVarName(i);
				//int iv=GetValSumVarLocalIndex(i);
				//if(iv>-1) test+="("+ItoAscii(iv+1)+")";		
				test+="'";
				for(int ii=0; ii<NumLines;ii++) {
		 			test+=","+FtoAscii(MR_Get_BayesCoCor(i,ii));
				}
				sSql=test+","+ItoAscii(m_SimulationRunNo)+","+ItoAscii(BAYESIANCALIB_mp_INDEXRUN)+","+ItoAscii(BAYESIANCALIB_mp_NUMIMPROVEDRUN)+")";
				pDatabase->ExecuteSQL(sSql);	
		}
		test = "INSERT INTO Calibrated_Parameters VALUES ('MEAN '";
		for(int ii=0; ii<NumLines;ii++) {
	 		test+=","+FtoAscii(MR_Get_BayesMean(ii));
		
		}
		sSql=test+","+ItoAscii(m_SimulationRunNo)+","+ItoAscii(BAYESIANCALIB_mp_INDEXRUN)+","+ItoAscii(BAYESIANCALIB_mp_NUMIMPROVEDRUN)+")";
	
		pDatabase->ExecuteSQL(sSql);

		test = "INSERT INTO Calibrated_Parameters VALUES ('CV '";
		for(int ii=0; ii<NumLines;ii++) {
			float v;
			v=MR_Get_BayesCoCor(ii,ii)/MR_Get_BayesMean(ii);
	 		test+=","+FtoAscii(v);
		}
		sSql=test+","+ItoAscii(m_SimulationRunNo)+","+ItoAscii(BAYESIANCALIB_mp_INDEXRUN)+","+ItoAscii(BAYESIANCALIB_mp_NUMIMPROVEDRUN)+")";
	
		pDatabase->ExecuteSQL(sSql);


		}
		 // Modulus on calibrated parameters


		//}			// Try end

//		Summary Table State Variable
	  	NumLines=m_XTGD_Array.GetSize();

	 
		for(int i=0; i<NumLines; ++i)
		{
			CBase *Sw = (CBase*)m_XTGD_Array.GetAt(i);	
			int itest=m_XTGD_Type_Array.GetAt(i);
			if(itest!=iTestPrev) {
				TestPrevGrp="  ";	
				switch (itest){
				case 1:
					test="State_Variables";
					TableName=test+ItoAscii(::count_x);
					sSql=SqlCreateSumTable(TableName);
					TRY {	pDatabase->ExecuteSQL(sSql); }
					CATCH_ALL(e)
					{ 		   NewFile=true;	}
					END_CATCH_ALL;

					SetRunReportArray("Writing to:"+TableName);
		//			m_pView->Invalidate();


					break;
				case 2:
					test="Flow_Variables";
					TableName=test+ItoAscii(::count_t);
					sSql=SqlCreateSumTable(TableName);
					SetRunReportArray("Writing to:"+TableName);
		//			m_pView->Invalidate();


					TRY {	pDatabase->ExecuteSQL(sSql); }
					CATCH_ALL(e)
					{ 		   NewFile=true;	}
					END_CATCH_ALL;
					break;
				case 3:
					test="Auxiliary_Variables";
					TableName=test+ItoAscii(::count_g);
					SetRunReportArray("Writing to:"+TableName);
				//	m_pView->Invalidate();
					sSql=SqlCreateSumTable(TableName);
					TRY {	pDatabase->ExecuteSQL(sSql); }
					CATCH_ALL(e)
					{ 		   NewFile=true;	}
					END_CATCH_ALL;
					break;
				case 4:
					test="Driving_Variables";
					TableName=test+ItoAscii(::count_d);
					SetRunReportArray("Writing to:"+TableName);
				//	m_pView->Invalidate();

					sSql=SqlCreateSumTable(TableName);
					TRY {	pDatabase->ExecuteSQL(sSql); }
					CATCH_ALL(e)
					{ 		   NewFile=true;	}
					END_CATCH_ALL;
					break;
				}

				iTestPrev=itest;
			}
			if (Sw->IsKindOf(RUNTIME_CLASS(CGD))){
				for(int ii=0;ii<((CGD*)Sw)->GetNumberOfFlags();ii++){
					if(((CGD*)Sw)->GetFlag(ii, &selected))			 
						if(selected) {
							TestGrp=Sw->GetGroup();
							if(TestGrp!=TestPrevGrp) {
//									ParFile.WriteString(Line);
									TestPrevGrp=TestGrp;
							}
							test=Sw->GetName();
							lindex=((CGD*)Sw)->GetLocalIndex(ii);
							if(lindex>-1&&((CGD*)Sw)->GetNumberOfFlags()>1) test+="("+ItoAscii(ii+1)+")";
							sSql=Sql_Excel_InsertToSumTable(TableName, TestPrevGrp, test, ((CGD*)Sw)->GetInitial(ii),((CGD*)Sw)->GetFinal(ii),((CGD*)Sw)->GetMin(ii), ((CGD*)Sw)->GetMax(ii),((CGD*)Sw)->GetMean(ii),((CGD*)Sw)->GetAccumulated(ii), Sw->GetUnit(),m_SimulationRunNo);
							if(BAYESIANCALIB_mp_ACCEPTEDRUN||!((CCoupModelApp*)pApp)->Running) {
								TRY {	pDatabase->ExecuteSQL(sSql); }
								CATCH_ALL(e)
								{ 		   
									if(itest==3){
										::count_g++;
										TableName="Auxiliary_Variables"+ItoAscii(::count_g);
										sSql=SqlCreateSumTable(TableName);
									}
									else if(itest==4){
										::count_d++;
										TableName="Driving_Variables"+ItoAscii(::count_d);
										sSql=SqlCreateSumTable(TableName);
									}

									pDatabase->ExecuteSQL(sSql);
									sSql=Sql_Excel_InsertToSumTable(TableName, TestPrevGrp, test, ((CGD*)Sw)->GetInitial(ii),((CGD*)Sw)->GetFinal(ii),((CGD*)Sw)->GetMin(ii), ((CGD*)Sw)->GetMax(ii),((CGD*)Sw)->GetMean(ii),((CGD*)Sw)->GetAccumulated(ii), Sw->GetUnit(),m_SimulationRunNo);
									pDatabase->ExecuteSQL(sSql);
								}
								END_CATCH_ALL;	
							}



						}
				}
			}
			else {
				for(int ii=0;ii<((CGD*)Sw)->GetNumberOfFlags();ii++){
					if(((CXT*)Sw)->GetFlag(ii, &selected))			 
						if(selected) {
							TestGrp=Sw->GetGroup();
							if(TestGrp!=TestPrevGrp) {
//									ParFile.WriteString(Line);
									TestPrevGrp=TestGrp;
							}
							test=Sw->GetName();
							lindex=((CXT*)Sw)->GetLocalIndex(ii);
							if(lindex>-1&&((CXT*)Sw)->GetNumberOfFlags()>1) test+="("+ItoAscii(ii+1)+")";
							sSql=Sql_Excel_InsertToSumTable(TableName,TestPrevGrp, test, ((CXT*)Sw)->GetInitial(ii),((CXT*)Sw)->GetFinal(ii), ((CXT*)Sw)->GetMin(ii), ((CXT*)Sw)->GetMax(ii),((CXT*)Sw)->GetMean(ii), ((CXT*)Sw)->GetAccumulated(ii),Sw->GetUnit(),m_SimulationRunNo);
							if(BAYESIANCALIB_mp_ACCEPTEDRUN||!((CCoupModelApp*)pApp)->Running) {
								TRY {	pDatabase->ExecuteSQL(sSql); }
								CATCH_ALL(e)
								{ 		   	
									if(itest==1){
										::count_x++;
										TableName="State_Variables"+ItoAscii(::count_x);
										sSql=SqlCreateSumTable(TableName);
									}
									else if(itest==2){
										::count_t++;
										TableName="Flow_Variables"+ItoAscii(::count_t);
										sSql=SqlCreateSumTable(TableName);
									}

									pDatabase->ExecuteSQL(sSql);
									sSql=Sql_Excel_InsertToSumTable(TableName, TestPrevGrp, test, ((CGD*)Sw)->GetInitial(ii),((CGD*)Sw)->GetFinal(ii),((CGD*)Sw)->GetMin(ii), ((CGD*)Sw)->GetMax(ii),((CGD*)Sw)->GetMean(ii),((CGD*)Sw)->GetAccumulated(ii), Sw->GetUnit(),m_SimulationRunNo);
									pDatabase->ExecuteSQL(sSql);
								
								}
								END_CATCH_ALL;	
							}
						}
				}
 			}
		}

	CString message;
	int Currentchoice;
	int Changed_Values, NoDefault;
	m_SavedSum=false;

	if(((CCoupModelApp*)pApp)->Running||IsMultiSimulation()) {
	  Currentchoice=IDYES;
	  Changed_Values=IDYES;
 	}
	else {
		message="Do you want to  save current setting (=Yes) or all default values (No) : ";
		Currentchoice=MFC_Util::MessageBox( LPCTSTR(message),_T("Change of values and default values"),MB_YESNO|MB_ICONQUESTION|MB_TASKMODAL);
		if(Currentchoice==IDYES) {
			message=_T("Do you want to  save values different from default values (=Yes) or all actual values (No) : ");
			Changed_Values=MFC_Util::MessageBox( LPCTSTR(message),_T("Change of values and default values"),MB_YESNO|MB_ICONQUESTION|MB_TASKMODAL);
			};
	};



	icount=MR_GetDimCounter(0);
	//icount=0; // Write all outputs to file
	if(icount<2) {
// Switches and paramters
	// Write Values on Switches parameters outputs
		for(int j=0;j<8;j++)
		{	
			//Choose the current name for the type
			switch (j)
				{
					case 0:	
						typ="Parameters";	
						sSql=SqlCreateParTable();
						SetRunReportArray("Writing to: "+typ);
				//		m_pView->Invalidate();


						TRY {pDatabase->ExecuteSQL(sSql); }
						CATCH_ALL(e)
						{ 		   NewFile=true;	}
						END_CATCH_ALL;
						break;
					case 1:	
						typ="Switches";
						sSql=SqlCreateSwitchTable();
						SetRunReportArray("Writing to: "+typ);
					//	m_pView->Invalidate();


						TRY {	pDatabase->ExecuteSQL(sSql); }
						CATCH_ALL(e)
						{ 		   NewFile=true;	}
						END_CATCH_ALL;
						break;
	 				case 2:	
						typ="Tables";
						sSql=SqlCreateVectorParTable();
						SetRunReportArray("Writing to: "+typ);
					//	m_pView->Invalidate();

						TRY {	pDatabase->ExecuteSQL(sSql); }
						CATCH_ALL(e)
						{ 		   NewFile=true;	}
						END_CATCH_ALL;
						break;
					case 3:	typ="State Variables";			break;
					case 4:	typ="Flow Variables";			break;
					case 5:	typ="Auxiliary Variables";		break;
					case 6:	typ="Driving Variables";		break;
					case 7:	
						typ="Files";
						sSql=SqlCreateFileTable();
						SetRunReportArray("Writing to: "+typ);
					//	m_pView->Invalidate();

						TRY {	pDatabase->ExecuteSQL(sSql); }
						CATCH_ALL(e)
						{ 		   NewFile=true;	}
						END_CATCH_ALL;
						break;
						
						break;
				}
			//Write type header
//			if(typ=="Parameters") 
//				ParFile.WriteString("****** "+typ+"******\tSimulation #\n");
//			else
//				ParFile.WriteString("****** "+typ+"******"+"\n");
				;
			//Get start position for traversing the map by the group 
			pos=GetFortranMap()->reg.GetGroupStart();
			while (pos!=nullptr) {
				int i=0;
				GetFortranMap()->reg.GetNextGroup(pos,grp);
				//Get the array with this type and the current group
				if(GetFortranMap()->reg.GetArray(typ,grp,arr)) {
					Sw=arr->GetAt(0);
					namn=Sw->GetGroup()+"\"H"+Sw->GetGroupID();
 //					if(Currentchoice==IDNO)	ParFile.WriteString(namn+"\n");
				}
			}
			pos=GetFortranMap()->reg.GetGroupStart();
			int jj=0;
			//Traverse all groups
			while(pos!=nullptr) 
				{
					int i=0;
					jj=jj+1;
					//Get next group
					GetFortranMap()->reg.GetNextGroup(pos,grp);
					
					//Get the array with this type and the current group
					if(GetFortranMap()->reg.GetArray(typ,grp,arr))
						{
							CString d;
							d.Format(_T("no : %d "),jj);
							bool isgroup=false;
							//Loop through all the variables in the array to see whether they are disabled or not
							while(i<arr->GetSize())	{
								int jjj=arr->GetSize();
								Sw=arr->GetAt(i);
								if(!Sw->IsDisabled())
									isgroup=true;
								i++;
							}
							//If any parameter in the array isn´t disabled then isgroup is now true
							//and the following while loop is executed
							if(isgroup||Currentchoice==IDNO)	
								{
									//Write the group name to the file
/*									if(Currentchoice==IDNO)
//										ParFile.WriteString("*** Group : "+Sw->GetGroup()+"\n H"+Sw->GetGroupID()+"\n"+typ+"\n");
									else if(typ=="Parameters"||typ=="Switches")
										ParFile.WriteString("*** Group : "+Sw->GetGroup()+"*** \t"+ItoAscii(m_SimulationRunNo)+"\n");
									else if(typ=="Tables")
										ParFile.WriteString("*** Group : "+Sw->GetGroup()+"*** \n");
  									//Reset the counter i
									i=0;
									//Loop through the whole array
									while(i<arr->GetSize())
										{
											//Get the parameter
			 								Sw=arr->GetAt(i);
											//Initialize the name string
											if(Currentchoice==IDNO)
												namn="Name:"+Sw->GetName();
											else {
												namn=Sw->GetName();
											}
//											namn+="\"\t";

											NoDefault=true;
											if(typ=="Parameters") {
								 				CString u=Sw->GetUnit();
												float v, defv;
												defv=*(float*)Sw->GetOriginalValue();
												if(Currentchoice==IDNO&&Changed_Values==IDNO) v=defv;
												else  v=*(float*)Sw->GetValue();
												CString stringv=FtoAscii(v);
												namn+=stringv+"\t"+u+"\t";
												if(v==defv) NoDefault=false;
													else NoDefault=true;
												if((!Sw->IsDisabled()&&Changed_Values==IDNO)||
													(!Sw->IsDisabled()&&NoDefault==true)){
													sSql=SqlInsertToParTable(Sw->GetGroup(),Sw->GetName(), v, u, m_SimulationRunNo);
    												pDatabase->ExecuteSQL(sSql);
												}


											}
											else if(typ=="Switches") 
												{
													//Set the name to the string to be written
													int nval, defnval;
													defnval=*(int*)Sw->GetOriginalValue();
													CSw *node=(CSw*)Sw;
													if(Currentchoice==IDNO) {
														for (int k=0;!node->GetOption(k).IsEmpty();k++) {
															if(defnval==k) namn=node->GetOption(k);
															 else namn= node->GetOption(k);
														}
													}
													else  {
														nval=*(int*)Sw->GetValue();
 														CString nop=node->GetOption(nval);
														namn =nop;
														if(defnval==nval) NoDefault=false;
															else NoDefault=true;
													}
													if((!Sw->IsDisabled()&&Changed_Values==IDNO)||
														(!Sw->IsDisabled()&&NoDefault==true)){
														sSql=SqlInsertToSwitchTable(Sw->GetGroup(),Sw->GetName(), namn, m_SimulationRunNo);
   														pDatabase->ExecuteSQL(sSql);
													}


												}
											else if(typ=="Files") 
												{
													//Set the name to the string to be written
													CF *node=(CF*)Sw;
													CString val=node->GetValue();
													if((!Sw->IsDisabled()&&Changed_Values==IDNO)||
													(!Sw->IsDisabled()&&NoDefault==true)){
														sSql=Sql_Excel_InsertToFileTable(Sw->GetGroup(),Sw->GetName(), val, m_SimulationRunNo);
														pDatabase->ExecuteSQL(sSql);
													}

												}
											else if(typ=="Tables")
												{
													
													int nval=*(int*)((CTab*)Sw)->GetValue();
													//If the parameter isn´t disabled
													
													if(!Sw->IsDisabled()||Currentchoice==IDNO)
														{
														
															//Write the number of elements and the name of the parameter
															namn.Format(namn+" No of elements in Table: %d", nval);
														   //	ParFile.WriteString(namn+"\n");
															CString Label="\"Index\"", text, unit;
															CPt *pPt;
															CString Row;
															float fValue, defv;
															int nCol;
															
															//Create string to write for the labels of the columns
														  	if(Currentchoice==IDNO) {
																for (nCol=0;((CTab*)Sw)->GetTitle(nCol,&text);nCol++){
																	pPt=((CTab*)Sw)->GetCPtPointer(nCol);
																	if(pPt!=nullptr) {
																		for(int nRow=0; ((CTab*)Sw)->GetCell(nCol, nRow,&fValue); nRow++) {
																			defv=pPt->GetOriginalValue(nRow);
																			((CTab*)Sw)->GetTitle(nCol,&Label);
																			((CTab*)Sw)->GetUnit(nCol,&unit);
																			sSql=SqlInsertToVectorParTable(Sw->GetGroup(),Sw->GetName(), Label, nRow, fValue, unit, m_SimulationRunNo );	
																			pDatabase->ExecuteSQL(sSql);
																		}
																	}
																}
															}
															else {
																for (nCol=0;((CTab*)Sw)->GetTitle(nCol,&text);nCol++){
																	Label=text;
																	pPt=((CTab*)Sw)->GetCPtPointer(nCol);
																	if(pPt!=nullptr) {
																		for(int nRow=0; ((CTab*)Sw)->GetCell(nCol, nRow,&fValue); nRow++){
																			defv=pPt->GetOriginalValue(nRow);
																			((CTab*)Sw)->GetTitle(nCol,&Label);
																			((CTab*)Sw)->GetUnit(nCol,&unit);
																			sSql=SqlInsertToVectorParTable(Sw->GetGroup(),Sw->GetName(), Label, nRow, fValue, unit, m_SimulationRunNo );	
																			pDatabase->ExecuteSQL(sSql);
																		}
																	}
																}
															}	
															
														}
														
														
												}
											else if(typ=="State Variables"||typ=="Flow Variables")
												{
													//Write the status of the auxiliary and driving flags
													if(Currentchoice==IDNO)	namn+="\n"+typ+"\n\""+Sw->GetUnit();
													else {namn+=CreateFlagString(Sw); NoDefault=false;};					
												
												}
											else 
												{
													//Default case in this else if structure
													//Write the status of the auxiliary and driving flags
													if(Currentchoice==IDNO)	namn+="\t\""+Sw->GetUnit();
													else {namn+=CreateFlagString(Sw); NoDefault=false;};					
												}

											//if the parameter isn´t disabled then write the parameter
//											if((Currentchoice==IDNO&&typ!="Tables")||(!Sw->IsDisabled()&&typ!="Tables"&&Changed_Values==IDNO)
//												||(!Sw->IsDisabled()&&typ!="Tables"&&NoDefault==true))
//												  	ParFile.WriteString(namn+"\n");
											//Increase the counter for the array traversal
											i++;
										}
								}	
						}
				}
		}

		}
	}

    pDatabase->Close();
	m_SavedSum=true;
	//pDatabase->
  }
  CATCH_ALL(e)
  {
	if(pDatabase->IsOpen()) pDatabase->Close();
	delete pDatabase;
	//database.Free();
   TRACE1("Driver not installed: %s",sDriver);
   return false;
  }
  END_CATCH_ALL;*/
  return true;
}
int Link::ReadFile(string lpszPathName)
{
//REMAIN
/*
   	POSITION	pos;
   	CString		grp,typ;
   	CBasePtrArray *arr;
   	CBase *Sw;
	int nlinecount = 0;
	int ntab = 0, npos=0, nspace=0;
	float fvalue;
	CString row, Parname, Parvalue;
	CString StartDatum;
	CString EndDatum;

	CStdioFile ParFile;
	CFileException fe;
	if (!ParFile.Open(lpszPathName, CFile::typeText |CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe,
			false, AFX_IDP_FAILED_TO_OPEN_DOC);
		TRACE("Can't Open file %s´\n", lpszPathName); 
		return false;
	}


	//Start traversing the map
	pos=GetFortranMap()->reg.GetGroupStart();
	m_readflag=0;		// OK when not indicated else PEJ 4/10
	//Parse the file and update the parameters
	while(ParFile.ReadString(row))
		{
		//Parse the read line row

		//Setting the type variable
		if(row.Left(6)==_T("******"))
			{
				typ=row.Mid(7);
				typ=typ.SpanExcluding(_T("*"));
				continue;
			}
		//Setting the group variable
		if(row.Left(12)=="*** Group : ")
			{
				grp=row.Mid(12);
				grp=grp.SpanExcluding(_T("*"));
				continue;
			}
		//Setting the start parameters
    	if(row.Left(5)==_T("Start"))
			 {
				int ncol;
				ncol=row.Find(':');
				row=row.Mid(ncol+1);
				int iposition = 0;
				int next=row.Find('-'); 
				while(next>0)
					{
					 Parvalue=row.Mid(0,next);
					 iposition++;
					 switch (iposition) 
					 {
						 case (1) :	 Fort.RunOpt.StartYear=AtoInt(Parvalue);
								StartDatum=Parvalue.Mid(1,4)+'-';break;
						 case (2) :	 Fort.RunOpt.StartMonth=AtoInt(Parvalue); 
								StartDatum+=Parvalue.Mid(0,2)+'-';break;
					     case (3) :	 Fort.RunOpt.StartDay=AtoInt(Parvalue); 
								StartDatum+=Parvalue.Mid(0,2)+' ';break;
						 case (4) :	 Fort.RunOpt.StartHour=AtoInt(Parvalue);
								StartDatum+=Parvalue.Mid(0,2)+':';break;
						 case (5) :	 Fort.RunOpt.StartMinute=AtoInt(Parvalue);
								StartDatum+=Parvalue.Mid(0,2);
								Fort.RunOpt.longMinStart=MinutConv(StartDatum);
					 }
			    	 row=row.Mid(next+1);
						if(iposition<4)next=row.Find('-');
						else next=row.GetLength();
				    }
				Parvalue=row.Mid(0);
				Fort.RunOpt.StartMinute=AtoInt(Parvalue);
				continue;
			 }
		//Setting the end parameters    
		if(row.Left(3)==_T("End"))
			{
				int ncol;
				ncol=row.Find(':');
				row=row.Mid(ncol+1);
				int iposition = 0;
				int next=row.Find('-'); 
				while(next>0)
					{
						Parvalue=row.Mid(0,next);
						iposition++;
						switch (iposition) 
						{
							case (1) :	 Fort.RunOpt.EndYear=AtoInt(Parvalue);
								EndDatum=Parvalue.Mid(1,4)+'-';break;
							case (2) :	 Fort.RunOpt.EndMonth=AtoInt(Parvalue); 
								EndDatum+=Parvalue.Mid(0,2)+'-';break;
							case (3) :	 Fort.RunOpt.EndDay=AtoInt(Parvalue); 
								EndDatum+=Parvalue.Mid(0,2)+' ';break;
							case (4) :	 Fort.RunOpt.EndHour=AtoInt(Parvalue); 
								EndDatum+=Parvalue.Mid(0,2)+':';break;
		   					 case (5) :	 Fort.RunOpt.EndMinute=AtoInt(Parvalue);
								EndDatum+=Parvalue.Mid(0,2);
								EndDatum+=Parvalue.Mid(3);
								Fort.RunOpt.longMinEnd=MinutConv(EndDatum);
								break;						
						}

						row=row.Mid(next+1);
						if(iposition<4)next=row.Find('-');
						else next=row.GetLength();
					}
				Parvalue=row.Mid(0);
				Fort.RunOpt.EndMinute=AtoInt(Parvalue);
				continue;
			}
		//Setting the output parameters	
		if(row.Left(6)==_T("Output"))
			{
				int ncol;
				ncol=row.Find(':');
				row=row.Mid(ncol+1);
				int next=row.Find('-'); 
				Parvalue=row.Mid(0,next);
				Fort.RunOpt.oidays=AtoInt(Parvalue);
				Parvalue=row.Mid(next+1);
				Fort.RunOpt.oiminutes=AtoInt(Parvalue);
				continue;
			}
		//Setting the number parameters
		if(row.Left(6)==_T("Number"))
			{
				int ncol;
				ncol=row.Find(':');
				Parvalue=row.Mid(ncol+1);
				Fort.RunOpt.noofiter=AtoInt(Parvalue);
				continue;
			}				
		//Setting the comments parameter
		if (row.Left(7)==_T("Comment"))
		{
			int coloncol;
			coloncol=row.Find(':');
			row=row.Mid(coloncol+1);
			//Trim the row
			row.TrimLeft();
			row.TrimRight();
			//Truncate the row to an apropriate number of characters
			row=row.Left(128);
			//comment is already allocated enough memory in the fortran part
			continue;
		}
		//Setting the comments parameter
		if (row.Left(14)==_T("Run Identifier"))
		{
			int coloncol;
			coloncol=row.Find(':');
			row=row.Mid(coloncol+1);		
			//Trim the row
			row.TrimLeft();
			row.TrimRight();
			//Truncate the row to an apropriate number of characters
			row=row.Left(128);
			//runID is already allocated enough memory in the fortran part
			continue;
		}

		//While the first character is a space or a " remove the first character from the string
		while(row.Find(' ')==0||row.Find('\"')==0)
					row=row.Mid(1);
		//Set ntab and nspace to the positions f the first tab and " characters
    	ntab=row.Find('\t');
		nspace=row.Find('\"');
		//Set npos to the smallest of ntab or nspace
		npos=(ntab < nspace ? ntab : nspace);
		//if npos is less than or eual to zero then set npos to the greatest variable
		//This happens when for instance the firts character matches or when the character doesnt exist within the row
		if(npos<=0) 
			npos=(ntab > nspace ? ntab : nspace);

		//Get the parameter name and the parameter value
		Parname=row.Left(npos);
		Parvalue=row.Mid(npos+1);
		//While the first character is a space, a " or a tab remove the first character from the parameter value
		while(Parvalue.Find(' ')==0||Parvalue.Find('\"')==0||Parvalue.Find('\t')==0)
							Parvalue=Parvalue.Mid(1);
		//Find the position for the " character
		int ncitation=Parvalue.Find('\"');
		
		//If the parameter isnt of the types below
		if (!(typ==_T("State Variables")||typ==_T("Flow Variables")||typ==_T("Auxiliary Variables")||typ==_T("Driving Variables")))
		{
			//If a " has been found then remove the characters left of the " character
			if (ncitation > 0) 
				Parvalue=Parvalue.Left(ncitation);
		}
		//If the typ variable equals to Parameters then read the parameters
		if(typ==_T("Parameters")) 
			{
				//remove characters around the value
				ntab=Parvalue.Find('\t');
				nspace=Parvalue.Find('\"');
				npos=(ntab < nspace ? ntab : nspace);
				if(npos<=0) 
					npos=(ntab > nspace ? ntab : nspace);
				Parvalue=Parvalue.Left(npos);
				//Set the parameter
				CFortranMap *p_fmap;
				p_fmap=GetFortranMap();

				Sw=m_pDoc->GetPtr(Parname, typ, grp);
				//If the parameter doesn´t exist then give a message about that, else
				//add the parameter
				if (Sw==nullptr)
					IncorrectParameterMessage(Parname,lpszPathName,typ,grp);
				else
				{
					fvalue=AtoFloat(Parvalue);
					Sw->SetValue(fvalue);
					//Apply the changes
					Sw->Apply();
				}
				continue;
			}
		//If typ=files					
		if(typ==_T("Files"))
			{
				//get a pointer to the parameter in question
				Sw=GetFortranMap()->reg.GetPtr(Parname, typ, grp);
				//Get a CF pointer by casting Sw to a CF pointer
				CF *node=(CF*)Sw;
				try {
 		    		  if (!node->IsKindOf(RUNTIME_CLASS(CXT))) throw "Bad cast.";
				}
				catch (CString message) {
		  		 // TRACE0(_T());
		  		  break;
				}
				
				//If the parameter doesn´t exist then give a message about that, else
				//add the parameter
				if (Sw==nullptr)
					IncorrectParameterMessage(Parname,lpszPathName,typ,grp);
				else
				{
					//Set the value of the parameter to the string Parvalue
					node->SetValue(Parvalue);
					//Apply the changes
					node->Apply();
				}
				continue;
			}
		//If typ=switches
		if (typ==_T("Switches")) 
			{
				//				ntab=Parvalue.Find("\t");
				//				nspace=Parvalue.Find("\"");
				//				npos=(ntab < nspace ? ntab : nspace);
				//				if(npos<=0) npos=(ntab > nspace ? ntab : nspace);
				//				Parvalue=Parvalue.Left(npos);
				//Get a pointer to the switch
				Sw=GetFortranMap()->reg.GetPtr(Parname, typ, grp);
				//If the parameter doesn´t exist then give a message about that, else
				//add the parameter
				if (Sw==nullptr)
					IncorrectParameterMessage(Parname,lpszPathName,typ,grp);
				else
				{
					//Store the new value
					//if (Sw->SetValue(Parvalue));
					Sw->SetValue(Parvalue);
					//Apply the changes
					Sw->Apply();
				}
				continue;
			}
		//If typ=... or ... or ... or ..
		if(typ==_T("State Variables")||typ==_T("Flow Variables")||typ==_T("Auxiliary Variables")||typ==_T("Driving Variables"))
			{
				//Get a pointer to the parameter
				Sw=GetFortranMap()->reg.GetPtr(Parname, typ, grp);
				//If the parameter doesn´t exist then give a message about that, else
				//add the parameter
				if (Sw==nullptr)
					IncorrectParameterMessage(Parname,lpszPathName,typ,grp);
				else
				{
					//Set the value of the parameter
				if (Sw->IsKindOf(RUNTIME_CLASS(CGD)))
					((CGD*)Sw)->SetFlags(Parvalue);
				else 
					((CXT*)Sw)->SetFlags(Parvalue);
				}
				continue;
			}
		//If typ = tables
		if(typ==_T("Tables"))
			{
				//get a pointer to the parameter
				Sw=GetFortranMap()->reg.GetPtr(Parname, typ, grp);
				//If the parameter doesn´t exist then display an error message
				if (Sw==nullptr)
					IncorrectParameterMessage(Parname,lpszPathName,typ,grp);
				//The lines below doesn´t do anything and therefore I remark them
				//but keep them for reference
				//if(Sw)
				//	int nel=*(int*)((CTab*)Sw)->GetValue();
				int ncol,ncol1,ncol2, nelements, iel=0;
				float value;
				CString strvalue;
				//Get the number of elements in the table
				ncol=Parvalue.Find(_T(":"));
				Parvalue=Parvalue.Mid(ncol+1);
				nelements=AtoInt(Parvalue);
				int oldnelements=*(int*)((CTab*)Sw)->GetValue();
				CTab* probe;
				probe = (CTab*)Sw;
				try {
					if (!probe->IsKindOf(RUNTIME_CLASS(CTab))) throw "bad cast";
				}
				catch (char* message) {
					TRACE0(message);
					break;
				}
				if (nelements!=oldnelements)
				{
					bool Nvalid;
					Nvalid=probe->SetValue(nelements);
					Nvalid=probe->Apply_CE_only();
					m_readflag=1;
//					return(1); Commented out by PEJ
				}
				//Loop through all the rows
				ParFile.ReadString(row);
				for(int nRow=0; nRow < nelements; nRow++)
					{
						ParFile.ReadString(row);
						iel=0;
						ncol1=row.Find('\"');
						row=row.Mid(ncol1+1);
						ncol2=row.Find('\"');
						strvalue=row.Mid(0,ncol2);
						row=row.Mid(ncol2+1);
						value=AtoFloat(strvalue);
						while (row.Find('\"')>=0)
							{
								//Get the values on this line and set the parameters to these values
								ncol1=row.Find(_T("\""));
								row=row.Mid(ncol1+1);
								ncol2=row.Find(_T("\""));
								strvalue=row.Mid(0,ncol2);
								row=row.Mid(ncol2+1);
								value=AtoFloat(strvalue);
								
								//If Sw isn´t equal to nullptr then set the values fo the table parameters
								if (Sw!=nullptr)
								{
									((CTab*)Sw)->GetCell(iel, nRow, &fvalue);
									((CTab*)Sw)->SetCell(iel, nRow, value);
								}
								iel=iel+1;
							}
					 }
					//Apply the changes if Sw isn´t equal to nullptr
					if (Sw!=nullptr&&m_readflag==0)
						((CTab*)Sw)->Apply();
					CTab* probe2;
					probe2 = (CTab*)Sw;
					try {
						if (!probe2->IsKindOf(RUNTIME_CLASS(CTab))) throw "bad cast";
					}
					catch (char* message) {
						TRACE0(message);
						return m_readflag;  // later update this
					}
			}
		//Increase the counter for the number of lines
		nlinecount++;
	}
	TRACE(_T("read /n"));
	//The reading was successful
	ParFile.Close();
	*/
	// m_readflag=0; It suffices that this function returns a value. DSM
	return (m_readflag); // This is not necessary if ReadFile will only be used
		  // by other CCoupModelDoc functions. DSM
}
void Link::WriteExport()
{	

		m_ExcelSumFile=MFC_Util::GetProfileStringStd("ExcelSumFile", m_ExcelSumFile);
  
	if(!WriteFile(m_ExcelSumFile))
		{
			TRACE("Can't Open file %s´\n", m_ExcelSumFile);
			//Display error message
			MFC_Util::MessageBox("For some reason the file specified couldn´t be written to.","File Save Error",MB_OK|MB_ICONEXCLAMATION);
			return;
		}

	//else if( ) {
	//		MFC_Util::MessageBox(_T("The file: ")+newPathName+_T(" was succesfully created"),_T("File Write Message"),MB_OK);
	//	}
}


int Link::ReadImport()
{
	int readSuccessID=0;
	
	/*CString pszFileName ;
	LPCTSTR lpszDefExt=_T("Par");
	LPCTSTR lpszFilter;

	//Create a read dialog
		CString szFilter = _T("Parameter Files (*.Par)|*.Par||");
		CFileDialog	ParFileDlg(true,lpszDefExt=_T("Par"),lpszFilter= _T("*.Par"),OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,szFilter);
	//Show the dialog
		fileDlgResult=ParFileDlg.DoModal();
	//If the cancel button has been pushed then return
		if (fileDlgResult==IDCANCEL)
			return(0);
	//Retrieve the filename of the file that is to be opened
		pszFileName = ParFileDlg.GetPathName();*/

		FILEDLG FDlg = MFC_Util::GetFileDialog("Par", "", "Parameter Files (*.Par)|*.Par||");
	//If the reading of the file was successful

		if (FDlg.opengood) {
			readSuccessID = ReadFile(FDlg.filename);

		}
		
	if (readSuccessID==-1)	
		{
			TRACE("Can't read file %s´\n", FDlg.filename);
			//Display error message
			MFC_Util::MessageBox("For some reason the file specified couldn´t be read.","File read Error",MB_OK|MB_ICONEXCLAMATION);
			return(readSuccessID);
			// Actions without file name ?
		}
		((Doc*)m_Map)->UpdateArchiveLists();
		return(readSuccessID);

}
bool Link::WriteCSV_File(string st)

{
	string str=st;
	CPG* pPG;
	string strnum, filename;
	if(str.size()>0) {
		CPG *pCPG= new CPG();
		strnum=FUtil::ItoNumAscii(m_Map->m_DocFile.m_SimulationRunNo);
		filename="COUP_"+strnum+".Bin";
		if(pCPG->Open(st)) {
			pPG=pCPG;
		}
		else {
			delete pCPG;
			return false;
		}

	}
	else {
		pPG=m_Map->m_PlotStorage.CPG_GetAt(0);
		if(pPG==nullptr) {
			CPG *pCPG= new CPG();

			if(m_Map->m_DocFile.m_SimulationRunNo>999) {
				strnum=FUtil::ItoNumAscii(m_Map->m_DocFile.m_SimulationRunNo);
				filename="COUP_"+strnum+".Bin";
			}
			else {
				strnum=FUtil::ItoNumAscii(m_Map->m_DocFile.m_SimulationRunNo);
				filename="COUP_"+strnum+".Bin";
			}
			if(pCPG->Open(filename)) {
				m_Map->m_PlotStorage.CPG_Add(pCPG);
				pPG=pCPG;
			}
			else {
				delete pCPG;
				return false;
			}
		}
	}


	if(pPG->WriteExcelFile().size()>0) {
		((ModelBase*)m_Map)->m_SavedTimeSerie=true;
		pPG->CloseFile();
		if(str.size()>0)
			delete pPG;
	}
	else
		((ModelBase*)m_Map)->m_SavedTimeSerie=true;
	return true;
}
