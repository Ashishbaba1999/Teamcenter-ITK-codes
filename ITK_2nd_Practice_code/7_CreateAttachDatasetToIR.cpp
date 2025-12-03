//2)create and attach dataset to specific IR

//Usecase - Utility to attach a Compliance or Regulatory Attachments(dataset) to all revisions of an item.

#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<ae/datasettype.h>
#include<ae/dataset.h>
#include<tccore/grm.h>



int Report_Error(int iFail)
{
	char* cError = NULL;
	if (iFail != ITK_ok)
	{		
		EMH_ask_error_text(iFail, &cError);
		printf("\n Error is : %s", cError);
		if (cError)MEM_free(cError);
		exit(0);
	}
}

///////////////////////////////////////////////////////

#include "Header.h"

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	int iCount = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cID = NULL;
	

	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tFolder = NULLTAG;
	tag_t* tRev_list = NULLTAG;
	tag_t tDatasetType = NULLTAG;
	tag_t tDataset = NULLTAG;
	tag_t tRelationType = NULLTAG;
	tag_t tRelation = NULLTAG;

	if (argc == 5)
	{
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGroup = ITK_ask_cli_argument("-g=");
		cID = ITK_ask_cli_argument("-id=");
		
		
		Report_Error(iFail = ITK_init_module(cUserID,cPassword,cGroup));
		printf("\n Login Success");

		Report_Error(iFail = ITEM_find_item(cID,&tItem));
		printf("\n Item Found");

		Report_Error(iFail = ITEM_list_all_revs(tItem,&iCount,&tRev_list));
		for (int i = 0; i < iCount; i++)
		{
			Report_Error(iFail = AE_find_datasettype2("PDF", &tDatasetType));
			Report_Error(iFail = AE_create_dataset_with_id(tDatasetType, "ASML_pdf", "", "", "", &tDataset));
			Report_Error(iFail = AE_save_myself(tDataset));

			Report_Error(iFail = GRM_find_relation_type("IMAN_specification", &tRelationType));
			Report_Error(iFail = GRM_create_relation(tRev_list[i], tDataset, tRelationType, NULLTAG, &tRelation));
			Report_Error(iFail = GRM_save_relation(tRelation));
		}


		Report_Error(iFail = ITK_exit_module(TRUE));
		printf("\n Logout Success");


		
	}
	else
	{
		printf("Argument Count is less or more");
	}

	return 0;

}



