// to register custom DLL registration 

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/custom.h>

#define DLLAPI _declspec (dllexport)

extern DLLAPI int Custom_exit_Ashish_register_callbacks();
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args);

///////////////////////////////////////////////////////////////////////////////////////////

#include "Header.h"

extern DLLAPI int Custom_exit_Ashish_register_callbacks() {
	CUSTOM_register_exit("Custom_exit_Ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks1);
	CUSTOM_register_exit("Custom_exit_Ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks2);
	return 0;
}
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args) {
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered Success after Login \n\n");
	TC_write_syslog("\n\n DLL Registered Success \n\n");
	return 0;
}
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args) {
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered for Logout Sucess \n\n");
	TC_write_syslog("\n\n DLL Registered for Logout Success \n\n");
	return 0;
}

// Clean -> Build -> Copy Path -> search in RUN 
// copy -> Custom_exit_Ashish.dll  paste-> TCROOT/bin
// copy -> Custom_exit_Ashish.lib  paste-> TCROOT/lib
// to set name of DLL in TC => Edit -> options -> search -> Search on Keywords-> TC_customization_liabraries
// click -> edit -> Custom_exit_Ashish  -> + <- click -> click -> save ------> restart TC

// to find syslog file -> go to -> Help -> about -> Log file -> copy path -> Run -> check in that file as 
//-> DLL registerd success.


///////////////////////////////////////////////////////
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/custom.h>

#define DLLAPI _declspec (dllexport)

extern DLLAPI int Custom_exit_ashish_register_callbacks();
extern DLLAPI int CUSTOM_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int CUSTOM_execute_callbacks2(int* decision, va_list args);






#include "Header.h"

extern DLLAPI int Custom_exit_ashish_register_callbacks()
{
	CUSTOM_register_exit("Custom_exit_ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)CUSTOM_execute_callbacks1);
	CUSTOM_register_exit("Custom_exit_ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)CUSTOM_execute_callbacks2);
	return 0;
}

extern DLLAPI int CUSTOM_execute_callbacks1(int* decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL register success after Login \n\n");
	TC_write_syslog("\n\n DLL register success after Login \n\n");

	return 0;

}
extern DLLAPI int CUSTOM_execute_callbacks2(int* decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL register success after Logout \n\n");
	TC_write_syslog("\n\n DLL register success after Logout \n\n");
	return 0;
}
