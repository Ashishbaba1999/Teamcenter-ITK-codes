#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/workspaceobject.h>
#include<sa/user.h>
#include<fclasses/tc_string.h>


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
    char* cError=NULL;
	char* cUserID=NULL;
	char* cPassword=NULL;
	char* cGroup=NULL;
	char* cItemName=NULL;
	char* cItemID=NULL;
    
    
	tag_t tUser = NULLTAG;
	WSO_search_criteria_t tCriteria;
	tag_t* tSearchList = NULLTAG;

	if (ITK_ask_cli_argument("-h")||ITK_ask_cli_argument("-help")) {
		Display_Usage();
	}
		
    if(argc==4){
    	cUserID=ITK_ask_cli_argument("-u=");
    	cPassword=ITK_ask_cli_argument("-p=");
    	cGroup=ITK_ask_cli_argument("-g=");
    	
    	Report_Error(iFail=ITK_init_module(cUserID,cPassword,cGroup));
    	printf("\n\n Login Success");

		Report_Error(iFail= WSOM_clear_search_criteria(&tCriteria));

		Report_Error(iFail= SA_find_user2("infodba",&tUser));
        
		tc_strcpy(tCriteria.class_name,"Item");
		tc_strcpy(tCriteria.name,"*");
		tCriteria.owner = tUser;

		Report_Error(iFail= WSOM_search(tCriteria,&iCount,&tSearchList));
		if(iCount>0){
			for(int i=0;i<iCount;i++){
				Report_Error(iFail=WSOM_ask_name2(tSearchList[i],&cItemName));
				Report_Error(iFail=WSOM_ask_id_string(tSearchList[i],&cItemID));
				printf("\n\n Item ID is %s and Item name is %s",cItemID,cItemName);
			}

		}
    	Report_Error(iFail=ITK_exit_module(TRUE));
    	printf("\n\n Logout Success");   	
	}
	else
	{
		printf("\n\n Argument count is less or more");
	}
	if(tSearchList)
	{
		MEM_free(tSearchList);
	}
	return 0;
}

/*
Report_Error(iFail= ());
printf("\n\n  "); 
*/



