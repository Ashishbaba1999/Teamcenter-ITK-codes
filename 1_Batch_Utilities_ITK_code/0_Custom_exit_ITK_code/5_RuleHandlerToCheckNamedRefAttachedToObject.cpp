
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/custom.h>
#include<epm/epm.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/workspaceobject.h> 
#include<tccore/aom.h>
#include<tccore/grm.h>
#include<pom/pom/pom.h>
#include<ae/datasettype.h>
#include<ae/dataset.h>

#define DLLAPI _declspec (dllexport)
#define CountError (EMH_USER_error_base + 8)
#define TypeError (EMH_USER_error_base + 9)


extern DLLAPI int Handlers_Ashish_register_callbacks();
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args);

EPM_decision_t check_named_ref(EPM_rule_message_t);

//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Header.h"


extern DLLAPI int Handlers_Ashish_register_callbacks() {
	CUSTOM_register_exit("Handlers_Ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks1);
	CUSTOM_register_exit("Handlers_Ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks2);
	return 0;
}
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args) {
	METHOD_id_t method_id;
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered Success after Login for Handlers \n\n");
	TC_write_syslog("\n\n DLL Registered Success after Login for Handlers \n\n");
	EPM_register_rule_handler("EPM_check_named_ref", "Custom Rule Handler to check named ref", (EPM_rule_handler_t)check_named_ref);
	return 0;
}

EPM_decision_t check_named_ref(EPM_rule_message_t msg) {

	EPM_decision_t decision = EPM_nogo;

	int i = 0;
	int j = 0;
	int k = 0;
	int iTargetCount = 0;
	int iSecObjCount = 0;
	int iNamedRefCount = 0;


	char* cClassName = NULL;
	char* cDatasetType = NULL;
	char* cTargetObjName = NULL;
	char* cSecObjName = NULL;

	tag_t tTask = NULLTAG;
	tag_t tRootTask = NULLTAG;
	tag_t* tTargetList = NULLTAG;
	tag_t tClassID = NULLTAG;
	tag_t tRelationType = NULLTAG;
	tag_t* tSecObjList = NULLTAG;
	tag_t* tNamedRefList = NULLTAG;


	tTask = msg.task;

	EPM_ask_root_task(tTask, &tRootTask);
	if (tRootTask != NULLTAG)
	{
		EPM_ask_attachments(tRootTask, EPM_target_attachment, &iTargetCount, &tTargetList);
		if (iTargetCount > 0)
		{
			for (j = 0; j < iTargetCount; j++)
			{
				POM_class_of_instance(tTargetList[j], &tClassID);
				POM_name_of_class(tClassID, &cClassName);
				if (tc_strcmp(cClassName, "ItemRevision") == 0)
				{
					GRM_find_relation_type("IMAN_specification", &tRelationType);
					GRM_list_secondary_objects_only(tTargetList[j], tRelationType, &iSecObjCount, &tSecObjList);
					if (iSecObjCount > 0)
					{
						for (i = 0; i < iSecObjCount; i++)
						{
							WSOM_ask_object_type2(tSecObjList[i], &cDatasetType);
							if (tc_strcmp(cDatasetType, "PDF") == 0)
							{
								k++;
								AE_ask_dataset_named_refs(tSecObjList[i], &iNamedRefCount, &tNamedRefList);
								if (iNamedRefCount == 1)
								{
									decision = EPM_go;
								}
								else
								{
									AOM_UIF_ask_value(tTargetList[j], "object_name", &cSecObjName);
									EMH_store_error_s1(EMH_severity_error, CountError, cSecObjName);
									// TCROOT/lang/testserver/en_US/ue_errors.xml--->write custom error as 
									// <error id="8">Required number of Named References are not attached on %1$</error>		
								}
							}

						}
						if (k == 0)
						{
							AOM_UIF_ask_value(tTargetList[j], "object_name", &cTargetObjName);
							EMH_store_error_s1(EMH_severity_error, TypeError, cTargetObjName);
							// TCROOT/lang/testserver/en_US/ue_errors.xml--->write custom error as 
							// <error id="9">Perticular type of Dataset is not attached on %1$</error>
						}
					}
					else
					{
						AOM_UIF_ask_value(tTargetList[j], "object_name", &cTargetObjName);
						EMH_store_error_s1(EMH_severity_error, TypeError, cTargetObjName);
					}
				}

			}
		}

	}
	if (tTargetList) {
		MEM_free(tTargetList);
	}
	if (tSecObjList) {
		MEM_free(tSecObjList);
	}
	if (tNamedRefList) {
		MEM_free(tNamedRefList);
	}



	return decision;
}
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args) {
	*decision = ALL_CUSTOMIZATIONS;
	//printf("\n\n DLL Registered for Logout Sucess \n\n");
	TC_write_syslog("\n\n DLL Registered for Logout Success \n\n");
	return 0;
}
