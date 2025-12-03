// assign project to target business objects through workflow process.


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
#include<tccore/project.h>

#define DLLAPI _declspec (dllexport)
#define CountError (EMH_USER_error_base + 8)
#define TypeError (EMH_USER_error_base + 9)


extern DLLAPI int ActHandler_Ashish_register_callbacks();
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args);

extern DLLAPI int assign_project_to_object(EPM_action_message_t); 

//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Header.h"


extern DLLAPI int ActHandler_Ashish_register_callbacks() {
	CUSTOM_register_exit("ActHandler_Ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks1);
	CUSTOM_register_exit("ActHandler_Ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks2);
	return 0;
}
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args) {
	METHOD_id_t method_id;
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered Success after Login for Handlers \n\n");
	TC_write_syslog("\n\n DLL Registered Success after Login for Handlers \n\n");
	// register Action handler using ...EPM_register_Action_handler
	EPM_register_action_handler("EPM_assign_project", "Custom Action Handler to assign project", (EPM_action_handler_t)assign_project_to_object);
	return 0;
}
extern DLLAPI int assign_project_to_object(EPM_action_message_t msg)
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

	tTask = msg.task;

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
						printf("\n\n Project Assigned to Object");
					}
				}
			}
		}
	}
	if (tTargetList) {
		MEM_free(tTargetList);
	}




	return 0;
}
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args) {
	*decision = ALL_CUSTOMIZATIONS;
	//printf("\n\n DLL Registered for Logout Sucess \n\n");
	TC_write_syslog("\n\n DLL Registered for Logout Success \n\n");
	return 0;
}
