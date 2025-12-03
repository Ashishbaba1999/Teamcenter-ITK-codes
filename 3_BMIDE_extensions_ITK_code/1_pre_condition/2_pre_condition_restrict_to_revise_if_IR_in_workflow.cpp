// write a code as do not revise the IR if it is in workflow. (precondition)

/*
1. Update BMIDE.bat file
2. set envirnment Variables
3  restart machine
4  set path in Bmide_generatecode 
5  edit project properties
//////////////////////////////////

1)set/check relese (teamcenter 14.2)
>Extentions>code generation> relase

2)create DLL Library (T2Method_Customization)
>Extentions>code generation> libraries

3)create Extention defination (T2Check_in_process)
>Extensions>Rule>Extention>Create new Extention defination

Name-T2Check_in_process
Availibilty-ADD
Bussiness Object Name* - ItemRevision
Business object or Proerty - OType 
Operation Name - Item_copy_rev
Extention Point - pre Condition

4) Attach custom extention to Bussiness Object
T2Check_in_process to ItemRevision

5)genrate code > modify > buit > deploy
*/
////////////////////////////////////////////////////////////////////////////////////////

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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

