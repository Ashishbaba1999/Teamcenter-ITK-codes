// Chnage the ownership of first level childs of the folder

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








