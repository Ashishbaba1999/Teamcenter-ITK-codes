// BMIDE extention for method custominsation (pre-condition) to restrict IR from revise if it is released.

 
#ifndef P2CHECK_RELEASE_STATUS_HXX
#define P2CHECK_RELEASE_STATUS_HXX
#include <tccore/method.h>
#include<tccore/custom.h>
#include <iostream>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/workspaceobject.h> 
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
	char* cValue = NULL;
	char* cName = NULL;
	tag_t tSource_rev = msg->object_tag;
	AOM_UIF_ask_value(tSource_rev, "release_status_list", &cValue);
	WSOM_ask_name2(tSource_rev, &cName); 
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




