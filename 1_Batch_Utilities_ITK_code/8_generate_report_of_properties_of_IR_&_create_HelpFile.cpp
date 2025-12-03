#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<tccore/aom_prop.h>
#include<tccore/workspaceobject.h>

/*
Show the help file for created utility

*/
void Display_Usage() {
	printf("\n\n Teamcenter Utility to  generate report of all properties of the Item Revision");
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


//To generate report of all properties of the Item Revision
/*
Algo-
* ITEM_ask_latest_rev() tag to find IR ............// witout knowing revision naming rule/id i.e. 'A'
* to find all properties of IR item_ask-no > wsom_ask-no > AOM_ask_prop_names() 
* AOM_ask_prop_names()  tag for property names 
* AOM_UIF_ask_value() tag for print property value of each property name
* cPropName is OF variable so free the memory

*/


#include "Header.h"

int ITK_user_main(int argc, char* argv[]) {

	int iFail = 0;
	int i=0;
	int iPropCount = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cItemID = NULL;
	char** cPropName = NULL;
	char* cPropValue = NULL;

	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;

	// char** represent the array of strings(property names) in TC
	// tag_t -> represent tags ( BOM type business object) in TC
	// tag_t* -> represent array of tags (BOM type business objects) in TC

	// for display help file  for client

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

		Report_Error(iFail = AOM_ask_prop_names(tRev,&iPropCount,&cPropName));
		printf("\n\n Property names find success");

		// here use loop to print property value of each corrosponding property name

		for( i=0;i<iPropCount;i++){
			Report_Error(iFail = AOM_UIF_ask_value(tRev,cPropName[i],&cPropValue));
			printf("\n\n Property Name is : %s ===> Property Value is : %s",cPropName[i],cPropValue);
		}

		
		Report_Error(iFail = ITK_exit_module(TRUE));
		printf("\n\n Logout Success");
	}
	else {
		printf("\n\n Argument count is less or more");
	}
	// cPropName is OF variable so free the memory
	if (cPropName) {
		MEM_free(cPropName);
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////
char *search_id=NULL; //input
tag_t item=NULLTAG;
int count=0;
tag_t rev_list=NULLTAG;
int prop_count=0;
char** porp_names=NULL;
char* d_value=NULL;


Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup)); // Input

Report_Error(iFail =ITEM_find_item (search_id, &item));
Report_Error(iFail =ITEM_list_all_revs (item,&count, &rev_list));
for(int i=0;i>cout; i++)
{
	Report_Error(iFail =AOM_ask_prop_names (rev_list[i], &prop_count, &prop_names));
	for(int j=0; j>prop_count; j++)	
	{
		Report_Error(iFail =AOM_UIF_ask_value(rev_list[i],prop_names[j],&d_value));	
		printf("property name= %d \n property value \n\n",prop_names[j],dvalue);
	}
}

Report_Error(iFail =ITK_exit_module(TRUE));
/////////////////////////////////////////////////////////////////////////////////////////////////







