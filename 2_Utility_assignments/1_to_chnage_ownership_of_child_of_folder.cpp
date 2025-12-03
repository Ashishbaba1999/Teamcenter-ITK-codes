// Chnage the ownership of first level childs of the folder to last level of heirarchy

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/workspaceobject.h>
#include<sa/user.h>
#include<fclasses/tc_string.h>
#include<tc/folder.h>
#include<tccore/aom.h>

void Display_Usage() {
	printf("\n\n Teamcenter Utility to Import Named Referance to Dataset");
	printf("\n\n -u=\t Login User ID");
	printf("\n\n -p=\t Login User Password");
	printf("\n\n -g=\t Login User Group");
	printf("\n\n -h\t to display the detailed information of utility");
	printf("\n\n -help\t to display the detailed information of utility");
	exit(0);
}

int Report_Error (int iFail)
{
	char* cError=NULL;	
	if(iFail!=ITK_ok){
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);		
	}
}
////////////////////////////////////////////////////////////////////////
#include "Header.h"

int ITK_user_main(int argc, char* argv[]){
	
	int iFail=0;
	int iCount=0;
	int iObjectCount=0;
    char* cError=NULL;
	char* cUserID=NULL;
	char* cPassword=NULL;
	char* cGroup=NULL;
	char* cFolderName=NULL;
	char* cChildName=NULL;
	
    tag_t tUser = NULLTAG;
    tag_t tNewUser = NULLTAG;
    tag_t tGroup = NULLTAG;
	WSO_search_criteria_t tCriteria;
	tag_t* tSearchList = NULLTAG;
	tag_t* tObjectList = NULLTAG;

	if (ITK_ask_cli_argument("-h")||ITK_ask_cli_argument("-help")) {
		Display_Usage();
	}
		
    if(argc==4){
    	cUserID=ITK_ask_cli_argument("-u=");
    	cPassword=ITK_ask_cli_argument("-p=");
    	cGroup=ITK_ask_cli_argument("-g=");
    	
    	Report_Error(iFail=ITK_init_module(cUserID,cPassword,cGroup));
    	printf("\n\n Login Success");

        Report_Error(iFail= SA_find_user2("infodba",&tUser));
		Report_Error(iFail= WSOM_clear_search_criteria(&tCriteria));
        

		tc_strcpy(tCriteria.class_name,"Folder");
		tc_strcpy(tCriteria.name,"testFL");
        tCriteria.owner = tUser;



		Report_Error(iFail= WSOM_search(tCriteria,&iCount,&tSearchList));
        if(iCount>0){
        for(int j=0;j<iCount;j++){
			Report_Error(iFail=FL_ask_references (tSearchList[j],FL_fsc_by_name, &iObjectCount, &tObjectList )); //to find contents of the folder
            Report_Error(iFail=SA_find_user2("AdminV",&tNewUser));
            Report_Error(iFail= SA_find_group("Engineering",&tGroup));
            if(iObjectCount>0){
                printf("\n\n no of child parts:%d",iObjectCount);
			for(int i=0;i<iObjectCount;i++){
                Report_Error(iFail = AOM_refresh(tObjectList[i],TRUE));
                Report_Error(iFail= AOM_set_ownership(tObjectList[i],tNewUser,tGroup));
                Report_Error(iFail=AOM_save_with_extensions(tObjectList[i]));
                Report_Error(iFail = AOM_refresh(tObjectList[i],FALSE));
			}
        }else{
            printf("\n\n Folder is empty "); 
        }
        
		}
        }

    	Report_Error(iFail=ITK_exit_module(TRUE));
    	printf("\n\n Logout Success");   	
	}
	else{
		printf("\n\n Argument count is less or more");
	}
	if(tSearchList){
		MEM_free(tSearchList);
	}
	if(tObjectList){
		MEM_free(tObjectList);
	}
	return 0;
}

/*
Report_Error(iFail= ());
printf("\n\n  "); 
*/
///////////////////////////////////////////////////////////////////////////////////////////
Header:

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tc/tc_util.h>
#include<fclasses/tc_string.h>

#include<tccore/item.h>
#include<ae/dataset.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<sa/user.h>
#include<sa/group.h>
#include<tc/folder.h>
#include<ae/datasettype.h>
#include<tccore/grm.h>
#include<tccore/workspaceobject.h>


int Report_Error(int iFail) {
	char* cError = NULL;
	if (iFail != ITK_ok) {
		EMH_ask_error_text(iFail, &cError);
		printf("\n%s", cError);
		exit(0);
	}
	return 0;
}

Code:
#include "Header.h"

int ITK_user_main(int argc, char* argv[]) {
    int iFail = 0;
    int iCount = 0;
    int iObjCount = 0;

    char* cUser = NULL;
    char* cPass = NULL;
    char* cGroup = NULL;
    char* cFolderName = NULL;
    char* cValue = NULL;

    tag_t tUser = NULLTAG;
    tag_t tNewUser = NULLTAG;
    tag_t tNewGroup = NULLTAG;
    tag_t* cList = NULL;
    tag_t* tObjects = NULL;

    WSO_search_criteria_t tCriteria;

    cUser = ITK_ask_cli_argument("-u=");
    cPass = ITK_ask_cli_argument("-p=");
    cGroup = ITK_ask_cli_argument("-g=");

    if (cUser != NULL && cPass != NULL && cGroup != NULL) {
        Report_Error(ITK_init_module(cUser, cPass, cGroup));
        printf("\n ITK login successful");

        Report_Error(SA_find_user2(cUser, &tUser));
        printf("\n User tag found");

        Report_Error(WSOM_clear_search_criteria(&tCriteria));
        tc_strcpy(tCriteria.class_name, "Folder");
        tc_strcpy(tCriteria.name, "CR7");
        tCriteria.owner = tUser;

        Report_Error(SA_find_user2("shaik", &tNewUser));
        Report_Error(SA_find_group("dba", &tNewGroup));

        Report_Error(WSOM_search(tCriteria, &iCount, &cList));
        if (iCount > 0) {
            for (int i = 0; i < iCount; i++)
            {
                Report_Error(WSOM_ask_name2(cList[i], &cFolderName));
                printf("\n Folder name is: %s", cFolderName);
                Report_Error(FL_ask_references(cList[i], FL_fsc_by_name, &iObjCount, &tObjects));
                if (iObjCount>0) {
                    for (int j = 0; j < iObjCount; j++)
                    {
                        Report_Error(AOM_refresh(tObjects[j], TRUE));
                        Report_Error(AOM_set_ownership(tObjects[j], tNewUser, tNewGroup));
                        Report_Error(AOM_save_with_extensions(tObjects[j]));
                        Report_Error(AOM_refresh(tObjects[j], FALSE));
                        Report_Error(WSOM_ask_name2(tObjects[j], &cValue));
                        printf("\n Owner has been changed for content : %s", cValue);
                    }
                }
                else {
                    printf("\n No content found in folder");
                }
            }
            MEM_free(tObjects);
            MEM_free(cList);
        }
        else {
            printf("\n No folder found");
        }

        Report_Error(ITK_exit_module(ITK_ok));
        printf("\n");
    }
    else {
        printf("\n Please enter proper credentials");
    }
    return 0;
}









