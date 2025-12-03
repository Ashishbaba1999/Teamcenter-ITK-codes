// create Item and save it in Home folder

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<sa/user.h>
#include<tc/folder.h>
#include<tccore/aom.h>


int Report_Error(int iFail) {
	char* cError = NULL;
	if (iFail != ITK_ok) {
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);
	}
}


///////////////////////////////////////////////////////////////////////////


#include "Header.h"
int ITK_user_main(int argc, char* argv[]) {
	int iFail = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cItemID = NULL;
	char* cItemName = NULL;
	tag_t tItem = NULLTAG;
	tag_t trev = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tHomeFolder = NULLTAG;

	if (argc == 6) {
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGroup = ITK_ask_cli_argument("-g=");
		cItemID = ITK_ask_cli_argument("-id=");
		cItemName = ITK_ask_cli_argument("-name=");

		//login
		Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
		printf("\n\n Login Success");
		
		//create and save  Item
		Report_Error(iFail = ITEM_create_item(cItemID, cItemName, "Item", "", &tItem, &trev));
		printf("\n\n Item  created");
		Report_Error(iFail = ITEM_save_item(tItem));
		printf("\n\n Item saved");

		//find user_tag 
		Report_Error(iFail = SA_find_user2(cUserID, &tUser));
		printf("\n\n User Found");

		//find/ask user's Home folder
		Report_Error(iFail = SA_ask_user_home_folder(tUser, &tHomeFolder));
		printf("\n\n User's Home Folder found");

		// add/insert item to Home folder
		Report_Error(iFail = FL_insert(tHomeFolder, tItem, 999));
		printf("\n\n Item inserted to Home Folder");

		// save Home Folder 
		Report_Error(iFail = AOM_save_with_extensions(tHomeFolder));
		printf("\n\n Home folder saved");

		// Logout
		Report_Error(iFail = ITK_exit_module(TRUE));
		printf("\n\n Logout Success");

	}
	else {
		printf("\n\n Argument count is less or more");
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
Report_Error(iFail = ITEM_create_item(cItemID, cItemName, "Item", "", &tItem, &trev));
Report_Error(iFail = ITEM_save_item(tItem));
Report_Error(iFail = SA_find_user2(cUserID, &tUser));
Report_Error(iFail = SA_ask_user_home_folder(tUser, &tHomeFolder));
Report_Error(iFail = FL_insert(tHomeFolder, tItem, 999));
Report_Error(iFail = AOM_save_with_extensions(tHomeFolder));
Report_Error(iFail = ITK_exit_module(TRUE));

/////////////////////////////////////////////////////////////////////////////////////////////////


