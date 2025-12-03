// find the contents of "task to prform" folder
// export containts of tasks to perform folder from my worklist folder into csv file.

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/workspaceobject.h>
#include<sa/user.h>
#include<fclasses/tc_string.h>
#include<tc/folder.h>

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


///////////////////////////////////////////////////////////////////////////////////////

#include "Header.h"

int ITK_user_main(int argc, char* argv[]) {

	int iFail = 0;
	int iCount = 0;
	int i = 0;
	int j = 0;
	int iObjectCount = 0;

	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cID = NULL;
	char* cFolderName = NULL;
	char* cChildName = NULL;

	tag_t tUser = NULLTAG;
	tag_t* tSearchList = NULLTAG;
	tag_t* tObjectList = NULLTAG;
	WSO_search_criteria_t tCriteria;

	if (argc == 5) {
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGroup = ITK_ask_cli_argument("-g=");
		cID = ITK_ask_cli_argument("-id=");

		Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
		printf("\n\n Login Success");

		Report_Error(iFail = WSOM_clear_search_criteria(&tCriteria));

		Report_Error(iFail = SA_find_user2(cID, &tUser));

		tc_strcpy(tCriteria.class_name, "Tasks To Perform");
		tc_strcpy(tCriteria.name, "*");
		tCriteria.owner = tUser;

		Report_Error(iFail = WSOM_search(tCriteria, &iCount, &tSearchList));
		printf("\n\n Search Success");
		for (i = 0; i < iCount; i++)
		{
			Report_Error(iFail = FL_ask_references(tSearchList[i], FL_fsc_by_name, &iObjectCount, &tObjectList)); //to find contents of the folder
			for (j = 0; j < iObjectCount; j++) {
				Report_Error(iFail = WSOM_ask_name2(tObjectList[j], &cChildName));
				printf("\n\n Folder child name is %s", cChildName);
			}
		}

		Report_Error(iFail = ITK_exit_module(TRUE));
		printf("\n\n Logout Success");
	}
	else {
		printf("\n\n Argument count is less or more");
	}
	return 0;
}




