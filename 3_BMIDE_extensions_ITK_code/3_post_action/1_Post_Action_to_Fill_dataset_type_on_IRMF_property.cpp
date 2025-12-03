// To fill the dataset type on the IRMF property aftre attachmnet of dataset on the Item Revision.

// Using BMIDE extensions

// hxx 





























































#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/workspaceobject.h> 
#include<tccore/aom.h>
#include<tccore/grm.h>

#define DLLAPI _declspec (dllexport)

extern DLLAPI int Custom_exit_Ashish_register_callbacks();
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args);
extern DLLAPI int IRMF_after_dataset_attach(METHOD_message_t* msg, va_list args);


///////////////////////////////////////////////////////////////////////////////////////

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

    // here we creating relation between two bussiness objects between item revision and dataset by attaching
    // dataset to IR .. here "IMAN_specification" is object type
    // to find msg for dataset  TCROOT>include>grm>search as grm_msg.h>RC>edit with notepad++> "GRM_create"
    METHOD_find_method("IMAN_specification", "GRM_create", &method_id);
	METHOD_add_action(method_id,METHOD_post_action_type,(METHOD_function_t)IRMF_after_dataset_attach, NULLTAG);
	
	return 0;
}
extern DLLAPI int IRMF_after_dataset_attach(METHOD_message_t* msg, va_list args){
    int iSecObj = 0;
    int i = 0;

    char* cType = NULL;

    tag_t tPrimary = NULLTAG;
    tag_t tSecondary = NULLTAG;
    tag_t tRelation = NULLTAG;
    tag_t* tSecObj = NULLTAG;

    tPrimary = va_arg(args, tag_t);
    tSecondary = va_arg(args, tag_t);

    GRM_find_relation_type("IMAN_master_form_rev",&tRelation);
    GRM_list_secondary_objects_only(tPrimary,tRelation,&iSecObj,&tSecObj);
    
    WSOM_ask_object_type(tSecondary,&cType);

    for(i=0;i<iSecObj;i++){
        AOM_lock(tSecObj[i]);
        AOM_UIF_set_value(tSecObj[i],"user_data_1",cType);
        AOM_save_without_extensions(tSecObj[i]);
        AOM_unlock(tSecObj[i]);
    }
    if(tSecObj){
        MEM_free(tSecObj);
    }
    return 0;
}
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args) {
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered for Logout Sucess \n\n");
	TC_write_syslog("\n\n DLL Registered for Logout Success \n\n");
	return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




