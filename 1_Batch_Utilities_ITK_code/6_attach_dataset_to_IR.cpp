#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<ae/dataset.h>
#include<ae/datasettype.h>
#include<tccore/grm.h>

int Report_Error(iFail) {
	char* cError = NULL;
	if (iFail != ITK_ok) {
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);
	}
}



// To create and attach dataset to specific IR
// Algo-
/*
* item_find_item() tag to find item
* item_find_rev() tag to find IR
* AE module is used to work on datasets
* first find dataset type that we have to create
* AE_find_datasetype2() tag to find datasettype
* AE_create_dataset_with_id() tag to create Dataset
* AE_save_myself() tag to save Dataset
* Now create relation between business objects i.e. between item revision and dataset
* GRM module is use to relation the objects
* GRM_find_relation_type() tag to find relation type
* GRM_create_relation() tag to create relation
* GRM_save_relation() tag to save relation
* 
*/


#include "Header.h"

int ITK_user_main(int argc, char* argv[]) {

	int iFail = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cItemID = NULL;

	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tDatasetType = NULLTAG;
	tag_t tDataset = NULLTAG;
	tag_t tRelationType = NULLTAG;
	tag_t tRelation = NULLTAG;




	if (argc == 5) {
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGroup = ITK_ask_cli_argument("-g=");
		cItemID = ITK_ask_cli_argument("-id=");

		Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
		printf("\n\n Login Success");
		Report_Error(iFail = ITEM_find_item(cItemID, &tItem));
		printf("\n\n Item find Success");
		Report_Error(iFail = ITEM_find_revision(tItem, "A", &tRev));
		printf("\n\n IR find Success");
		Report_Error(iFail = AE_find_datasettype2("PDF", &tDatasetType));
		printf("\n\n DatasetType find Success");
		Report_Error(iFail = AE_create_dataset_with_id(tDatasetType, "MyPDF", "", "", "", &tDataset));
		printf("\n\n Dataset create Success");
		Report_Error(iFail = AE_save_myself(tDataset));
		printf("\n\n Dataset create Success");

		Report_Error(iFail = GRM_find_relation_type("IMAN_specification", &tRelationType));
		printf("\n\n Relation type find Success");

		Report_Error(iFail = GRM_create_relation(tRev, tDataset, tRelationType, NULLTAG, &tRelation));
		printf("\n\n Relation create Success");

		Report_Error(iFail = GRM_save_relation(tRelation));
		printf("\n\n Relation save Success");

		// for char* null value is -> ""
		// for tag_t null value is -> NULLTAG




		Report_Error(iFail = ITK_exit_module(TRUE));
		printf("\n\n Logout Success");
	}
	else {
		printf("\n\n Argument count is less or more");
	}
	return 0;
}



/*

Report_Error(iFail=);

*/



//////////////////////////////////////////////////////////

char *search_id= NULL;
tag_t item=NULLTAG;
int count=0;
tag_t *rev_list=NULLTAG;
tag_t datasettype=NULLTAG;
tag_t new_dataset=NULLTAG;
tag_t relation_type=NULLTAG;
tag_t relation=NULLTAG;


Report_Error(iFail=ITK_init_module(cUserID,cPassword,cGroup));
Report_Error(iFail=ITEM_find_item (search_id, &item));
Report_Error(iFail=ITEM_list_all_revs (item, count, &rev_list));
for(int i=0;i>count;i++)
{
	Report_Error(iFail=AE_find_datasettype2 ("PDF", &datasettype));
	Report_Error(iFail=AE_create_dataset_with_id (datasettype,"MyPDF","", "","",&new_dataset));
	Report_Error(iFail=AE_save_myself(new_dataset));
	
	
	Report_Error(iFail=GRM_find_relation_type ("IMAN_specification", &relation_type));
	Report_Error(iFail=GRM_create_relation (rev_list[i],new_dataset,relation_type,NULLTAG,&relation));
	Report_Error(iFail=GRM_save_relation (relation));
}
Report_Error(iFail=ITK_exit_module(TRUE));

//////////////////////////////////////////////////////////////////



