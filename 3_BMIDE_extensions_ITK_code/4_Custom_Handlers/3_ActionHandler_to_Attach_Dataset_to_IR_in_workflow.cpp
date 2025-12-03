// write a Action Handler using BMIDE extention .. as attach datatset to IR after review task.

/*

1)set/check relese (teamcenter 14.2)
>Extentions>code generation> relase

2)create DLL Library (T2Handler_Customization)
>Extentions>code generation> libraries

>Extensions>Rule>Extention>Create new Extention defination

Name-T2EPM_assign_dataset
Availibilty-ADD
Bussiness Object Name* - Session
Business object or Proerty - OType 
Operation Name - BMF_SESSION_register_epm_Handlers
Extention Point - Base Action

4) Attach custom extention to Bussiness Object
T2EPM_assign_dataset to Session

5)genrate code > modify > buit > deploy

*/

// in.hxx

#include<iostream>
#include<epm/epm.h>
#include<tccore/aom.h>
#include<tccore/grm.h>
#include<pom/pom/pom.h>
#include<ae/datasettype.h>
#include<ae/dataset.h>
#include<tccore/project.h>


#define DLLAPI _declspec (dllexport)
extern DLLAPI int assign_dataset_to_object(EPM_action_message_t msg); 



// in .cxx

int P2EPM_assign_dataset( METHOD_message_t *msg, va_list /*args*/)     // default
{
	using namesspace std;
	
	int iFail= ITK_ok;
	tag_t obj=NULLTAG;
	obj=msg->object_tag;
	EPM_register_action_handler("EPM_assign_dataset", "Custom Action Handler to assign dataset", (EPM_action_handler_t)assign_dataset_to_object);
	TC_write_syslog("\n\n DLL Registered Success after Login for Handlers \n\n");
	
	return ifail;
	
}

extern DLLAPI int assign_dataset_to_object(EPM_action_message_t msg)
{
	using namespace std;
	
	int iCount = 0;
	int iNo_of_Arguments = 0;
	int i = 0;
	int j = 0;

	char* cArgName = NULL;
	char* cArgValue = NULL;
	char* cClassName = NULL;
	char* cDatasetType = NULL;
	char* cdatasetname = NULL;
	char* cRelationNmae = NULL;
	
	tag_t tRootTask = NULLTAG;
	tag_t* tAttachments = NULLTAG;
	tag_t tClassId = NULLTAG;
	tag_t tdatasetType = NULLTAG;
	tag_t tDataset = NULLTAG;
	tag_t tRealationType = NULLTAG;
	tag_t tRelation = NULLTAG;
	
	EPM_ask_root_task(msg.task,&tRoottask);
	if(tRootTask!=NULLTAG)
	{
		EPM_ask_attachments(tRoottask,EPM_target_attachment,&iCount,&tAttachments);      // if it is ItemRevision or Not
		if(tAttachemnts!=NULLTAG && iCount>0)
		{
			iNo_of_Arguments = TC_number_of_Arguments(msg.arguments);                    // get no of arguments
			
			for( j=0;j < iNo_of_Arguments ; j++)
			{
				ITK_ask_argument_named_Value(TC_next_argument(msg.arguments),&cArgName,&cArgValue);     // check arguments, i.e. dataset type , dataset Name, relation Name
				
				if(tc_strcmp(cArgName,"dataset_type")==0)
				{
					cDatasetTpye=(char*)MEM_alloc(100);
					tc_strcpy(cDatasetType,"");
					tc_strcpy(cdatasetType,cArgValue);
				}
				else(tc_strcmp(cArgName,"dataset_name")==0)
				{
					cDatasetName=(char*)MEM_alloc(100);
					tc_strcpy(cDatasetName,"");
					tc_strcpy(cdatasetName,cArgValue);
				}
				else(tc_strcmp(cArgName,"relation_name")==0)
				{
					cRelationName=(char*)MEM_alloc(100);
					tc_strcpy(cRelationName,"");
					tc_strcpy(cRelationName,cArgValue);
				}
				
			}
			AE_find_datasettype2 (cDatasetType, &tdatasetType);
	        AE_create_dataset_with_id (tdatasetType,cDatasetName,"Text", "","",&tDataset);
	        AE_save_myself(new_dataset));
	        GRM_find_relation_type (cRelationName, &tRealationType);
	        
	        for(i=0;i<iCount;i++)
	        {
	        	POM_class_of_instance(tAttachments[i],&tClassId);
	        	POM_name_of_class(tClassId,&cClassName)
	        	if(tc_strcmp(cClassName,"ItemRevision")==0)
	        	{
	        		GRM_create_relation (tAttachments[i],tDataset,tRealationType,NULLTAG,&tRelation);
	                GRM_save_relation (tRelation);
	        		
				}
	        	
			}
	        
		}
	}
	if (tAttachments) MEM_free(tAttachments);

}





