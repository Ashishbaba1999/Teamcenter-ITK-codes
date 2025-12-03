#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/grm.h>
#include<tccore/item.h>
#include<tccore/workspaceobject.h>

int Report_Error(iFail) {
	char* cError = NULL;
	if (iFail != ITK_ok) {
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);
	}
}

//To generate report of secondary objects attached to IR
/*
Algo-
* item_find_rev() tag to find IR
* to find all list of secondary objects attached to IR
* GRM_find_relation_type() tag to find relation type
* GRM_list_secondary_objects_only() tag to find datasets/secObjects
* to print all the datasets/secObjects
* first find type/name of datasets/secObjects
* here use loop to print secObjects name
* inside loop use 
*  WSOM_ask_name2() tag to find name of each dataset/secObjects
*  tSecObject is OF variable so free the memory

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
	char* cSecObjName = NULL;

	//tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tRelationType = NULLTAG;
	tag_t* tSecObjects = NULLTAG;


	// tag_t -> represent BOM type business object TC
	// tag_t* -> represent array of BOM type business objects in TC

	if (argc == 5) {
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGroup = ITK_ask_cli_argument("-g=");
		cItemID = ITK_ask_cli_argument("-id=");

		Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
		printf("\n\n Login Success");

		//Report_Error(iFail = ITEM_find_item(cItemID, &tItem));
		//printf("\n\n Item find Success");
		Report_Error(iFail = ITEM_find_rev(cItemID, "A", &tRev));
		printf("\n\n IR find Success");

        //* to find all list of secondary objects attached to IR
        //* GRM_find_relation_type() tag to find relation type
        //* GRM_list_secondary_objects_only() tag to find datasets/secObjects

		Report_Error(iFail = GRM_find_relation_type("IMAN_specification",&tRelationType));
		printf("\n\n Relation Type find Success");


		Report_Error(iFail = GRM_list_secondary_objects_only(tRev,tRelationType,&iSecObjCount,&tSecObjects));
		printf("\n\n Datasets find success");

        //to print all the datasets/secObjects
        // first find type/name of datasets/secObjects
        // here use loop to print secObjects name
        //inside loop use 
        //WSOM_ask_name2() tag to find name of each dataset/secObjects

		for( i=0;i<iSecObjCount;i++){
			Report_Error(iFail = WSOM_ask_name2(tSecObjects[i], &cSecObjName));
			printf("\n\n Datasets Name is %s",cSecObjName);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


char *cname=NULL;
tag_t *secondary_objects=NULLTAG;
int SecCount= 0;
tag_t *relation_type=NULLTAG;
tag_t *rev_list=NULLTAG;
int count=0;
tag_t item=NULLTAG;
char* search_id=NULL;  // Input


Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup)); // Input

Report_Error(iFail=ITEM_find_item (search_id, &item));
Report_Error(iFail=ITEM_list_all_revs (item, &count, &rev_list));
for(int i=0;i>count;i++)
{
	Report_Error(iFail=GRM_find_relation_type ("IMAN_specification", &relation_type));
	Report_Error(iFail=GRM_list_secondary_objects_only (rev_list[i], tag_t relation_type, &SecCount, &secondary_objects));
	for(int j=0;j>SecCount;j++)
	{
		Report_Error(iFail=WSOM_ask_name2 (tag_t secondary_objects[j], &cname));		
		printf("Dataset is %s",cname);
	}
}

Report_Error(iFail = ITK_exit_module(TRUE));





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




