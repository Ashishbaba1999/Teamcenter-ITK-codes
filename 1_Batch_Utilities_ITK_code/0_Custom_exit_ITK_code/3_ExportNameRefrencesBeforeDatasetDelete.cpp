// To Export all the Named Referances to local drive before delete operation on datatset

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
//extern DLLAPI int check_release_status(METHOD_message_t* msg, va_list args);
extern DLLAPI int export_before_delete(METHOD_message_t* msg, va_list args);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Header.h"
// To Export all the Named Referances to local drive before delete operation on datatset

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

    // to find msg for dataset  TCROOT>include>ae>search as dataset_msg.h>RC>edit with notepad++> "AE_delete_dataset"
	METHOD_find_method("Dataset", "AE_delete_dataset", &method_id);
	METHOD_add_action(method_id,METHOD_pre_action_type,(METHOD_function_t)export_before_delete, NULLTAG);
	return 0;
}
extern DLLAPI int export_before_delete(METHOD_message_t* msg, va_list args)
{
	int j = 0;
	int k = 0;
	int iRefCount = 0;
	int iDatasetTypeRefListCount = 0;

	char* cdatasettypeName = NULL;
	char** cDatasetTypeRefNameeList = NULL;
	char* cFileName = NULL;
	char originalpath[100] = "C:\\data\\";

	tag_t tDataset = msg->object_tag;
	//tag_t tDataset = va_arg(args, tag_t);
	tag_t tdatasetType = NULLTAG;
	tag_t* tRefList = NULLTAG;

	WSOM_ask_object_type2(tDataset, &cdatasettypeName);
	AE_find_datasettype2(cdatasettypeName, &tdatasetType);
	AE_ask_datasettype_refs(tdatasetType, &iDatasetTypeRefListCount, &cDatasetTypeRefNameeList);
	for (j = 0; j < iDatasetTypeRefListCount; j++)
	{
		printf("\n\n Ref List Name is %s", cDatasetTypeRefNameeList[j]);
		AE_ask_all_dataset_named_refs2(tDataset, cDatasetTypeRefNameeList[j], &iRefCount, &tRefList);
		for (k = 0; k < iRefCount; k++)
		{
			printf("\n\n File Path is %s", originalpath);
			TC_write_syslog("\n\n File Path is %s", originalpath);
			IMF_ask_original_file_name2(tRefList[k], &cFileName);
			tc_strcat(originalpath, cFileName);
			printf("\n\n Updated File Path is %s", originalpath);
			TC_write_syslog("\n\n Updated File Path is %s", originalpath);
			IMF_export_file(tRefList[k], originalpath);
			//Report_error();
			tc_strcpy(originalpath, "C:\\data\\");
			printf("\n\n File Path is %s", originalpath);
			TC_write_syslog("\n\n File Path is %s", originalpath);
		}
	}


	return 0;
}
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args) {
	*decision = ALL_CUSTOMIZATIONS;
	//printf("\n\n DLL Registered for Logout Sucess \n\n");
	TC_write_syslog("\n\n DLL Registered for Logout Success \n\n");
	return 0;
}