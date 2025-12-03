//BMIDE Extention for custom rule Handler to check the Named Refrence attach to Business object

/*

1)set/check relese (teamcenter 14.2)
>Extentions>code generation> relase

2)create DLL Library (T2Handler2_Customization)
>Extentions>code generation> libraries

3)create Extention defination (T2EPM_attach_NamedRefrence)
>Extensions>Rule>Extention>Create new Extention defination

Name-T2EPM_check_NamedRefrence
Availibilty-ADD
Bussiness Object Name* - Session
Business object or Proerty - OType 
Operation Name - BMF_SESSION_register_epm_Handlers
Extention Point - Base Action

4) Attach custom extention to Bussiness Object
T2EPM_check_NamedRefrence to Session

5)genrate code > modify > buit > deploy

*/

// .hxx file

#include<iostream>
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
#define CountError (EMH_USER_error_base + 8)
#define TypeError (EMH_USER_error_base + 9)

EPM_decision_t check_named_ref(EPM_rule_message_t);

// .cxx file

int T2EPM_check_NamedRefrence( METHOD_message_t *msg, va_list /*args*/)     // default
{
	using namesspace std;
	
	int iFail= ITK_ok;
	tag_t obj=NULLTAG;
	obj=msg->object_tag;
	EPM_register_action_handler("EPM_check_named_ref", "Custom Rule Handler to check named ref", (EPM_rule_handler_t)check_named_ref);
	TC_write_syslog("\n\n DLL Registered Success after Login for Handlers \n\n");
	
	return ifail;	
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
								}
							}
						}
						if (k == 0)
						{
							AOM_UIF_ask_value(tTargetList[j], "object_name", &cTargetObjName);
							EMH_store_error_s1(EMH_severity_error, TypeError, cTargetObjName);
							
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
	if (tTargetList)MEM_free(tTargetList);
	if (tSecObjList) MEM_free(tSecObjList);
	
	if (tNamedRefList) MEM_free(tNamedRefList);



	return decision;
}
