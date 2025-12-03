// 1)create Item and save it in Home folder

#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<sa/user.h>
#include<tc/folder.h>
#include<tccore/aom.h>


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


///////////////////////////////////////////////////
#include "Header.h"

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cID = NULL;
	char* cName = NULL;

	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tFolder = NULLTAG;

	if (argc == 6)
	{
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGroup = ITK_ask_cli_argument("-g=");
		cID = ITK_ask_cli_argument("-id=");
		cName = ITK_ask_cli_argument("-name=");
		
		Report_Error(iFail = ITK_init_module(cUserID,cPassword,cGroup));
		printf("\n Login Success");

		Report_Error(iFail = ITEM_create_item(cID, cName, "", "", &tItem, &tRev));
		printf("\n Item created");

		Report_Error(iFail = ITEM_save_item(tItem));
		printf("\n Item saved");

		Report_Error(iFail = SA_find_user2(cUserID, &tUser));
		printf("\n User found");

		Report_Error(iFail = SA_ask_user_home_folder(tUser, &tFolder));
		printf("\n Folder Found");

		Report_Error(iFail = FL_insert(tFolder, tItem, 999));
		printf("\n Item inserted to Folder");

		Report_Error(iFail = AOM_save_with_extensions(tFolder));
		printf("\n Folder saved");

		Report_Error(iFail = ITK_exit_module(TRUE));
		printf("\n Logout Success");


		
	}
	else
	{
		printf("Argument Count is less or more");
	}

	return 0;

}



