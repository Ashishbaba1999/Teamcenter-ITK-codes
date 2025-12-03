#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/grm.h>
#include<tccore/item.h>
#include<tccore/workspaceobject.h>
#include<string.h>
#include<fclasses/tc_string.h>
#include<ae/dataset.h>
#include<tccore/aom.h>

void Display_Usage() {
	printf("\n\n Teamcenter Utility to Import Named Referance to Dataset");
	printf("\n\n -u=\t Login User ID");
	printf("\n\n -p=\t Login User Password");
	printf("\n\n -g=\t Login User Group");
	printf("\n\n -id=\t Item ID");
	printf("\n\n -h\t to display the detailed information of utility");
	printf("\n\n -help\t to display the detailed information of utility");
	exit(0);
}

int Report_Error(iFail) {
	char* cError = NULL;
	if (iFail != ITK_ok) {
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);
	}
}

////////////////////////////////////////////////////////////////////////
/*
Teamcenter Utility to Export Named Referance from Dataset

Algo-
*  ITEM_ask_latest_rev() tag to find IR
* to find all list of secondary objects attached to IR
* GRM_find_relation_type() tag to find relation type
* GRM_list_secondary_objects_only() tag to find datasets/secObjects
* to print all the datasets/secObjects
* here use loop to print secObjects name
* inside loop use 
* WSOM_ask_object_type2() tag to find type of each dataset
* use if(tc_strcmp(cSecObjType,"PDF")==0) {
  AOM_refresh() tag
  AE_export_named_ref() tag
  AE_save_myself() tag
}
* tSecObject is OF variable so free the memory

*/



#include "Header.h"

int ITK_user_main(int argc, char* argv[]) {

	int iFail = 0;
	int i=0;
	int iSecObjCount = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cItemID = NULL;
	char* cSecObjType = NULL;

	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tRelationType = NULLTAG;
	tag_t* tSecObjects = NULLTAG;


	// tag_t -> represent BOM type business object TC
	// tag_t* -> represent array of BOM type business objects in TC

    if (ITK_ask_cli_argument("-h")||ITK_ask_cli_argument("-help")) {
		Display_Usage();
	}

	if (argc == 5) {
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGroup = ITK_ask_cli_argument("-g=");
		cItemID = ITK_ask_cli_argument("-id=");

		Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
		printf("\n\n Login Success");

		Report_Error(iFail = ITEM_find_item(cItemID, &tItem));
		printf("\n\n Item find Success");
		Report_Error(iFail = ITEM_ask_latest_rev(tItem, &tRev));
		printf("\n\n IR find Success");

		Report_Error(iFail = GRM_find_relation_type("IMAN_specification",&tRelationType));
		printf("\n\n Relation Type find Success");


		Report_Error(iFail = GRM_list_secondary_objects_only(tRev,tRelationType,&iSecObjCount,&tSecObjects));
		printf("\n\n Datasets find success");


		for( i=0;i<iSecObjCount;i++){
			Report_Error(iFail = WSOM_ask_object_type2(tSecObjects[i], &cSecObjType));
			printf("\n\n Datasets Type is %s",cSecObjType);

			if(tc_strcmp(cSecObjType,"PDF")==0){

			Report_Error(iFail = AOM_refresh(tSecObjects[i],TRUE));
			printf("\n\n Datasets refresh success");

            Report_Error(iFail = AE_export_named_ref(tSecObjects[i],"PDF_Referance","path with dobule slash..."));
            printf("\n\n named referance file export success");

			Report_Error(iFail = AE_save_myself(tSecObjects[i]));    // ae_save_myself() is doing checkin and save both
			printf("\n\n Datasets save success");
			

			}

			else if(tc_strcmp(cSecObjType,"Text")==0){

			Report_Error(iFail = AOM_refresh(tSecObjects[i],TRUE));
			printf("\n\n Datasets refresh success");
            
            Report_Error(iFail = AE_export_named_ref(tSecObjects[i],"Text","path with dobule slash..."));
            printf("\n\n named referance file export success");

			Report_Error(iFail = AE_save_myself(tSecObjects[i]));
			printf("\n\n Datasets save success");

			}
		}

		
		Report_Error(iFail = ITK_exit_module(TRUE));
		printf("\n\n Logout Success");
	}
	else {
		printf("\n\n Argument count is less or more");
	}
	// tSecObject is OF variable so free the memory
	if (tSecObjects) {
		MEM_free(tSecObjects);
	}
	return 0;
}




