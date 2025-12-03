// BMIDE Extention to create custom action Handler to assign project to IR in workflow

/*

1)set/check relese (teamcenter 14.2)
>Extentions>code generation> relase

2)create DLL Library (T2Handler1_Customization)
>Extentions>code generation> libraries

3)create Extention defination (T2EPM_assign_Project)
>Extensions>Rule>Extention>Create new Extention defination

Name-T2EPM_assign_Project
Availibilty-ADD
Bussiness Object Name* - Session
Business object or Proerty - OType 
Operation Name - BMF_SESSION_register_epm_Handlers
Extention Point - Base Action

4) Attach custom extention to Bussiness Object
T2EPM_assign_Project to Session

5)genrate code > modify > buit > deploy

*/

// .hxx

#include<iostream>
#include<epm/epm.h>
#include<tccore/aom.h>
#include<tccore/grm.h>
#include<pom/pom/pom.h>
#include<ae/datasettype.h>
#include<ae/dataset.h>
#include<tccore/project.h>

#define DLLAPI _declspec (dllexport)
extern DLLAPI int assign_dataset_to_Project(EPM_action_message_t msg); 

// .cxx


int T2assign_status( METHOD_message_t * msg, va_list /*args*/ )


{
	using namespace std;

		int iFail= ITK_ok;
		tag_t obj=NULLTAG;
		obj=msg->object_tag;
		EPM_register_action_handler("EPM_assign_Project", "Custom Action Handler to assign Project", (EPM_action_handler_t)assign_dataset_to_Project);
		TC_write_syslog("\n\n DLL Registered Success after Login for Handlers \n\n");

		return iFail;

}
int assign_dataset_to_Project(EPM_action_message_t msg)
{
	int iTargetCount = 0;
	int iNO_arguments = 0;
	int i = 0;
	int j = 0;

	char* cArgName = NULL;
	char* cArgValue = NULL;
	char* cProjectID = NULL;
	char* cClassName = NULL;

	tag_t tTask = NULLTAG;
	tag_t tRootTask = NULLTAG;
	tag_t* tTargetList = NULLTAG;
	tag_t tClassID = NULLTAG;
	tag_t tProject = NULLTAG;

	EPM_ask_root_task(tTask, &tRootTask);
	if (tRootTask != NULLTAG)
	{
		EPM_ask_attachments(tRootTask, EPM_target_attachment, &iTargetCount, &tTargetList);
		if (iTargetCount > 0)
		{
			iNO_arguments = TC_number_of_arguments(msg.arguments);
			for (i = 0; i < iNO_arguments; i++)
			{
				ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgName, &cArgValue);
				if (tc_strcmp(cArgName, "project_id") == 0)
				{
					cProjectID = (char*)MEM_alloc(100);
					tc_strcpy(cProjectID, "");
					tc_strcpy(cProjectID, cArgValue);
				}
			}
			for (j = 0; j < iTargetCount; j++)
			{
				POM_class_of_instance(tTargetList[j], &tClassID);
				POM_name_of_class(tClassID, &cClassName);
				if (tc_strcmp(cClassName, "ItemRevision") == 0)
				{
					PROJ_find(cProjectID, &tProject);
					if (tProject != NULLTAG)
					{
						PROJ_assign_objects(1, &tProject, 1, &tTargetList[j]);
						//printf("\n\n Project Assigned to Object");
					}
				}
			}
		}
	}
	if (tTargetList)MEM_free(tTargetList);

}


