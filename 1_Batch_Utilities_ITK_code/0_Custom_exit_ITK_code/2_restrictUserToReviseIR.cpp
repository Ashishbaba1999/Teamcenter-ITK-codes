//To restirct the user to Revise the ItemRevision if it is not released and display custom error message.

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/workspaceobject.h> //WSOM_ask_name2()

#define DLLAPI _declspec (dllexport)
#define PLM_error (EMH_USER_error_base + 2)
//#define PLM_error (919002)

extern DLLAPI int Custom_exit_Ashish_register_callbacks();
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args);
extern DLLAPI int check_release_status(METHOD_message_t* msg, va_list args);





///////////////////////////////////////////////////////////////////////////////////////////

#include "Header.h"

extern DLLAPI int Custom_exit_Ashish_register_callbacks() {
	CUSTOM_register_exit("Custom_exit_Ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks1);
	CUSTOM_register_exit("Custom_exit_Ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks2);
	return 0;
}
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args) {
	METHOD_id_t method_id;
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered Success after Login \n\n");
	TC_write_syslog("\n\n DLL Registered Success \n\n");
	METHOD_find_method("ItemRevision", "ITEM_copy_rev", &method_id);
	METHOD_add_pre_condition(method_id, (METHOD_function_t)check_release_status, NULLTAG);
	return 0;
}
extern DLLAPI int check_release_status(METHOD_message_t* msg, va_list args)
{
	tag_t tSource_rev = NULLTAG;
	char* cValue = NULL;
	char* cName = NULL;
	tSource_rev = va_arg(args, tag_t);
	AOM_UIF_ask_value(tSource_rev, "release_status_list", &cValue);
	WSOM_ask_name2(tSource_rev, &cName);           // for printing object name in error message
	if (tc_strlen(cValue) > 0)
	{
		TC_write_syslog("\n\n DLL Revised Success \n\n");
	}
	else
	{
		// TCROOT/lang/testserver/en_US/ue_errors.xml--->write custom error as 
		// <error id="2">Custom exit : Item revision will not revised berfore released</error>
		//EMH_store_error(EMH_severity_error, PLM_error);
		// TCROOT/lang/testserver/en_US/ue_errors.xml--->write custom error as 
		// <error id="2">Custom exit : %1$ will not revised berfore released</error>
		EMH_store_error_s1(EMH_severity_error, PLM_error,cName);
		return PLM_error;
	}

	return 0;
}
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args) {
	*decision = ALL_CUSTOMIZATIONS;
	//printf("\n\n DLL Registered for Logout Sucess \n\n");
	TC_write_syslog("\n\n DLL Registered for Logout Success \n\n");
	return 0;
}


/////////////////////////////////////////////////////////////////////////








#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>

#define DLLAPI _declspec (dllexport)
#define PLM_error (EMH_USER_error_base + 2)

extern DLLAPI int Custom_exit_ashish_register_callbacks();
extern DLLAPI int CUSTOM_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int CUSTOM_execute_callbacks2(int* decision, va_list args);

extern DLLAPI int check_release_status(METHOD_message_t msg, va_list args);





#include "Header.h"

extern DLLAPI int Custom_exit_ashish_register_callbacks()
{
	CUSTOM_register_exit("Custom_exit_ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)CUSTOM_execute_callbacks1);
	CUSTOM_register_exit("Custom_exit_ashish", "USER_exit_module", (CUSTOM_EXIT_ftn_t)CUSTOM_execute_callbacks2);
	return 0;
}

extern DLLAPI int CUSTOM_execute_callbacks1(int* decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL register success after Login \n\n");
	TC_write_syslog("\n\n DLL register success after Login \n\n");

	METHOD_id_t method_id;

	METHOD_find_method("ItemRevision", "ITEM_copy_rev", &method_id);
	METHOD_add_pre_condition(method_id,(METHOD_function_t) check_release_status, NULLTAG);


	return 0;

}
extern DLLAPI int check_release_status(METHOD_message_t msg, va_list args)
{
	tag_t tSource_rev = NULLTAG;
	tSource_rev = va_arg(args, tag_t);
	char* cValue = NULL;
	AOM_UIF_ask_value(tSource_rev, "release_status_list", &cValue);
	if (tc_strlen(cValue) > 0)
	{
		TC_write_syslog("\n\n Reivse Success\n\n");
	}
	else
	{
		EMH_store_error(EMH_severity_error, PLM_error);
		return PLM_error;
	}

	return 0;
}
extern DLLAPI int CUSTOM_execute_callbacks2(int* decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL register success after Logout \n\n");
	TC_write_syslog("\n\n DLL register success after Logout \n\n");
	return 0;
}
