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
Teamcenter Utility to Import Named Referance to Dataset

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
  AE_import_named_ref() tag
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

	        //Report_Error(iFail = AOM_lock(tSecObjects[i]));
			//printf("\n\n Datasets checkout success");

			Report_Error(iFail = AOM_refresh(tSecObjects[i],TRUE));
			printf("\n\n Datasets refresh success");

			Report_Error(iFail = AE_import_named_ref(tSecObjects[i],"PDF_Referance","C:\\Users\\AdminV\\Desktop\\import.txt",NULL,SS_BINARY));//shift+ctrl+right click> copy as path
			printf("\n\n named referance file import success");

			//Report_Error(iFail = AOM_unlock(tSecObjects[i]));
			//printf("\n\n Datasets checkin success");

			//Report_Error(iFail = AOM_refresh(tSecObjects[i],FALSE));
			//printf("\n\n Datasets refresh success");

			Report_Error(iFail = AE_save_myself(tSecObjects[i]));    // ae_save_myself() is doing checkin and save both
			printf("\n\n Datasets save success");
			

			}
			else if(tc_strcmp(cSecObjType,"Text")==0){

			//Report_Error(iFail = AOM_lock(tSecObjects[i]));
			//printf("\n\n Datasets checkout success");

			Report_Error(iFail = AOM_refresh(tSecObjects[i],TRUE));
			printf("\n\n Datasets refresh success");

			Report_Error(iFail = AE_import_named_ref(tSecObjects[i],"Text","C:\\Users\\AdminV\\Desktop\\import.txt",NULL,SS_TEXT));
			printf("\n\n named referance file import success");

			//Report_Error(iFail = AOM_unlock(tSecObjects[i]));
			//printf("\n\n Datasets checkin success");

			//Report_Error(iFail = AOM_refresh(tSecObjects[i],FALSE));
			//printf("\n\n Datasets refresh success");

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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<tccore/grm.h>
#include<string.h>
#include<fclasses/tc_string.h>
#include<tccore/workspaceobject.h>
#include<tccore/aom.h>
#include<ae/dataset.h>
#include<ae/ae.h>
#ifndef SS_BINARY
#define SS_BINARY 1
#endif
#ifndef SS_TEXT
#define SS_TEXT 2
#endif 


int Report_Error(int iFail)
{
	char* cError = NULL;
	if (iFail != ITK_ok) {
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);
	}
}


#include "Header.h"

int ITK_user_main(int argc, char* argv[]) {

	int iFail = 0;
	int iCount = 0;
	int i = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cID = NULL;
	char* cObjType = NULL;

	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tRelationType = NULLTAG;
	tag_t* tSecObjects = NULLTAG;

	if (argc == 5) {
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGroup = ITK_ask_cli_argument("-g=");
		cID = ITK_ask_cli_argument("-id=");

		Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
		printf("\n\n Login Success");

		Report_Error(iFail = ITEM_find_item(cID, &tItem));
		Report_Error(iFail = ITEM_ask_latest_rev(tItem, &tRev));
		Report_Error(iFail = GRM_find_relation_type("IMAN_specification", tRelationType));
		Report_Error(iFail = GRM_list_secondary_objects_only(tRev, tRelationType, &iCount, &tSecObjects));
		for (i = 0; i < iCount; i++)
		{
			Report_Error(iFail = WSOM_ask_object_type2(tSecObjects[i], &cObjType));
			if (tc_strcmp(cObjType, "PDF") == 0)
			{
				Report_Error(iFail = AOM_refresh(tSecObjects[i], TRUE));
				Report_Error(iFail = AE_import_named_ref(tSecObjects[i], "PDF_Referance", "C:\\Users\\AdminV\\Desktop\\fff.pdf", NULL,SS_BINARY));
				Report_Error(iFail = AE_save_myself(tSecObjects[i]));
			}
			else if (tc_strcmp(cObjType, "Text") == 0)
			{
				Report_Error(iFail = AOM_refresh(tSecObjects[i], TRUE));
				Report_Error(iFail = AE_import_named_ref(tSecObjects[i], "Text", "C:\\Users\\AdminV\\Desktop\\Harish.txt", NULL,SS_TEXT));
				Report_Error(iFail = AE_save_myself(tSecObjects[i]));
			}
		}
		
		Report_Error(iFail = ITK_exit_module(TRUE));
		printf("\n\n Logout Success");
	}
	else {
		printf("\n\n Argument count is less or more");
	}
	if (tSecObjects) {
		MEM_free(tSecObjects);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* search_id = NULL;
tag_t item = NULLTAG;
int cout= NULL;
tag_t* rev_list=NULLTAG;
tag_t relation_type=NULLTAG;
int Seccount=0;
tag_t *secondary_objects=NULLTAG;
char* objectType= NULL;


Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));

Report_Error(iFail =ITEM_find_item (search_id, &item));
Report_Error(iFail =ITEM_list_all_revs (item,&count, &rev_list));
for(int i=0;i>cout; i++)
{
	Report_Error(iFail = GRM_find_relation_type ("IMAN_specification", &relation_type));
	Report_Error(iFail = GRM_list_secondary_objects_only (rev_list[i], relation_type, &Seccount,&secondary_objects));
	for(int j=0;j>Seccount;j++)
	{
		Report_Error(iFail = WSOM_ask_object_type2 (secondary_objects[j], &object_type));
		if(tc_strsmp(object_type,"PDF")==0)
		{
			Report_Error(iFail = (AOM_refresh(object_type,TRUE));
			Report_Error(iFail = AE_import_named_ref (secondary_objects[j],"PDF_reference", "C:\\Users\\AdminV\\Desktop\\fff.pdf", NULL,SS_BINARY) );
			Report_Error(iFail = AE_save_myself (secondary_objects[j]));

			
			
		}
		if(tc_strsmp(object_type,"Text")==0)
		{
			Report_Error(iFail = (AOM_refresh(object_type,TRUE));
			Report_Error(iFail = AE_import_named_ref (secondary_objects[j],"Text", "C:\\Users\\AdminV\\Desktop\\sss.text", NULL,SS_TEXT) );
			Report_Error(iFail = AE_save_myself (secondary_objects[j]));
			
		}
	}
	
	
	
}

Report_Error(iFail = GRM_list_secondary_objects_only (rev_list[i], relation_type, &Seccount,&secondary_objects));
Report_Error(iFail = WSOM_ask_object_type2 (secondary_objects[j], &object_type));
Report_Error(iFail = (AOM_refresh(object_type,TRUE));
Report_Error(iFail = AE_import_named_ref (secondary_objects[j],"PDF_reference", "C:\\Users\\AdminV\\Desktop\\fff.pdf", NULL,SS_BINARY) );
Report_Error(iFail = AE_save_myself (tag_t dataset));




































