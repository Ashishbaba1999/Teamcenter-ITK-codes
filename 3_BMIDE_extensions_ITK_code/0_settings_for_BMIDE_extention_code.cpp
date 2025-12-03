// BMIDE extention for method custominsation (pre-condition)

// first have to configure BMIDE for BMIDE extention
// step 1 - Update bmide.bat file
// C/SOFTWARE/SIEMENS/TCROOT/bmide/client -> bmide.bat or bmide > RC > Edit with notepad++
// set variables 
/*
set JRE64_HOME=%TC_JRE_HOME%
set JAVA_HOME=C:\SOFTWARE\JAVA\jdk-20
set JDK_HOME=C:\SOFTWARE\JAVA\jdk-20
set FMS_HOME=C:\SOFTWARE\SIEMENS\TCROOT\tccs
set JRE64_HOME=C:\SOFTWARE\JAVA\jdk-20
*/
// call 2 bat files
/*
set PATH=%JRE_HOME%\bin;%JDK_HOME%\bin;%TC_ROOT%\lib;%FMS_HOME%\bin;%FMS_HOME%\lib;%FMS_HOME%;%PATH%

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat"
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// set environment - this pc>RC>Proprties>Advance system settings> Environment variable>
// in system variables >  click >new> 
// Varible Name- MSDEV_HOME>
// Variable Value - C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64 >
// OK > 
// in system variables > path > edit > new > copy paste > C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin
// OK> OK

// Restart the machine

///////////////////////////////////////////////////////////////////////////////////////////////////////
// go to > TCROOT >bin >find and open with notepad++ > bmide_generatecode.bat file 
// set TC_BIN=C:\SOFTWARE\SIEMENS\TCROOT\bin

// to genarate and save lib and dll files autoatically we done this setting
///////////////////////////////////////////////////////////////////////////////////////////////////////
// create project in bmide i.e. p2ashish > RC > properties > Teamcenter > build Configuration > Windows 64 > 
// Teamcenter Installation > C:\SOFTWARE\SIEMENS\TCROOT
// Compiler Home > C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64
// Apply and close

// create custom item i.e. P2ashishItem
// search > P2ashishItemRevision
//............................................................................................................
// to create dll --

// go to extentions >p2ashish > code generation > releases > teamcenter14 > teamcenter 14.2 > RC > Organise > set as active release. -->it will show green arrow
// go to extentions >p2ashish > code generation > libraries > RC > new library > P2method_customisation > discription > restrict to revise > 
// depentdent on > add.. like , ae,bom epm etc .... > finish
//............................................................................................................
// to create extention --
// go to extentions >p2ashish > rules > extentions > RC > new extention defination.. > name - P2check_release_status
// >Library > P2method_customisation
// > Availability > add > 
// business object name >P2ashishItemRevision
// business object or property > select --> type
// operation Name > ITEM_copy_rev
// Extention point > PreCondition
// okay > finish
//....................................................................................................................
// to attach extention (P2check_release_status) to business object (P2ashishItemRevision) --

// go to > P2ashishItemRevision > operations > serch operation > ITEM_copy_rev > select > click on >extention attachment > add 
// extention point > pre condtion
// extention > browse >select > P2check_release_status> finish

// save the data model
//..................................................................................................................
// to write code in .cxx file(.cpp file) and .hxx file(.h file)
// create files
// p2ashish > RC > Generate code > c++ classes
//.............................................................................


 
#ifndef P2CHECK_RELEASE_STATUS_HXX
#define P2CHECK_RELEASE_STATUS_HXX
#include <tccore/method.h>
#include<tccore/custom.h>
#include <iostream>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/workspaceobject.h> //WSOM_ask_name2()
#include <P2method_customisation/libp2method_customisation_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif
#define PLM_error (EMH_USER_error_base + 2)
extern P2METHOD_CUSTOMISATION_API int P2check_release_status(METHOD_message_t* msg, va_list args);
                 
#ifdef __cplusplus
                   }
#endif
                
#include <P2method_customisation/libp2method_customisation_undef.h>
                
#endif  // P2CHECK_RELEASE_STATUS_HXX
//.........................................................................................




#include <P2method_customisation/P2check_release_status.hxx>

int P2check_release_status( METHOD_message_t * msg, va_list /*args*/ )
{
	using namespace std;
	//tag_t tSource_rev = NULLTAG;
	char* cValue = NULL;
	char* cName = NULL;
	tag_t tSource_rev = msg->object_tag;
	//tSource_rev = va_arg(args, tag_t);
	AOM_UIF_ask_value(tSource_rev, "release_status_list", &cValue);
	WSOM_ask_name2(tSource_rev, &cName);           // for printing object name in error message
	if (tc_strlen(cValue) > 0)
	{
		TC_write_syslog("\n\n DLL Revised Success \n\n");
	}
	else
	{
		// TCROOT/lang/testserver/en_US/ue_errors.xml--->write custom error as
		// <error id="2">Custom exit : %1$ will not revised berfore released</error>
		EMH_store_error_s1(EMH_severity_error, PLM_error,cName);
		return PLM_error;
	}

 
 return 0;

}
///////////////////////////////////////////////////////////////////////////////////////////////

// save code > save project > navigator > p2ashish >RC>Build configuration > build all
// Navigator > output > win 64 > lib > you will find P2method_customisation.lib and P2method_customisation.dll files

//outside TC we get these files in >TCROOT>bmide>workspace > 14000.0.0>p2custom >output >winx64>lib> check the date and time

/////////////////////////////////////////////////////////////////////////////////////////////
// cold deployment of project
// BMIDE>Generate software package
//////////////////////////////////////////////////////////////////////////////////////////


//auto genarated code for hxx

//@<COPYRIGHT>@
//==================================================
//Copyright $2025.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/* 
 * @file 
 *
 *   This file contains the declaration for the Extension T2check_in_process
 *
 */
 
#ifndef T2CHECK_IN_PROCESS_HXX
#define T2CHECK_IN_PROCESS_HXX
#include <tccore/method.h>
#include <T2method_customization/libt2method_customization_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif
                 
extern T2METHOD_CUSTOMIZATION_API int T2check_in_process(METHOD_message_t* msg, va_list args);
                 
#ifdef __cplusplus
                   }
#endif
                
#include <T2method_customization/libt2method_customization_undef.h>
                
#endif  // T2CHECK_IN_PROCESS_HXX
/////////////////////////////////////////////////////////////////

//auto genarated code for cxx

//@<COPYRIGHT>@
//==================================================
//Copyright $2025.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/* 
 * @file 
 *
 *   This file contains the implementation for the Extension T2check_in_process
 *
 */
#include <T2method_customization/T2check_in_process.hxx>

int T2check_in_process( METHOD_message_t * /*msg*/, va_list /*args*/ )
{
	using namespa
 
 return 0;

}
////////////////////////////////////////////////////////////////////////////////////////

// after mpdifcation 
//@<COPYRIGHT>@
//==================================================
//Copyright $2025.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/* 
 * @file 
 *
 *   This file contains the declaration for the Extension T2check_in_process
 *
 */
 
#ifndef T2CHECK_IN_PROCESS_HXX
#define T2CHECK_IN_PROCESS_HXX
#include <tccore/method.h>

#include<iostream>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>

#include <T2method_customization/libt2method_customization_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif

#define PLM_error (EMH_USER_error_base +5)
                 
extern T2METHOD_CUSTOMIZATION_API int T2check_in_process(METHOD_message_t* msg, va_list args);
                 
#ifdef __cplusplus
                   }
#endif
                
#include <T2method_customization/libt2method_customization_undef.h>
                
#endif  // T2CHECK_IN_PROCESS_HXX






////////////////////////////////////////
//@<COPYRIGHT>@
//==================================================
//Copyright $2025.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/* 
 * @file 
 *
 *   This file contains the implementation for the Extension T2check_in_process
 *
 */
#include <T2method_customization/T2check_in_process.hxx>

int T2check_in_process( METHOD_message_t * msg, va_list /*args*/ )
{
	using namespace std;
	TC_write_syslog("\n\n Custom DLL load Successful");
	int iFail=ITK_ok;
	tag_t source_rev=NULLTAG;
	char *cValue=NULL;
	char *cObjName=NULL;

	source_rev = msg ->object_tag;

	iFail=AOM_UIF_ask_value(source_rev,"process_stage_list",&cValue);
	iFail=AOM_UIF_ask_value(source_rev,"object_name",&cObjName);
	if(tc_strlen(cValue)>0)
	{
		EMH_store_error_s1(EMH_severity_error,PLM_error,cObjName);
		return PLM_error;
	}
	else
	{
		TC_write_syslog("\n\n Revise Successful");
	}
 
 return iFail;

}


