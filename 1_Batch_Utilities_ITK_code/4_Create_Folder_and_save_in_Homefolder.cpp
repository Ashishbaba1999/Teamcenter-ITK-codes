// create folder and save in Home folder.

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tc/folder.h>
#include<tccore/aom.h>
#include<sa/user.h>
#include<tc/folder.h>


int Report_Error(iFail){
	char* cError=NULL;
	if(iFail!=ITK_ok){
		EMH_ask_error_text(iFail,&cError);
		printf("\n\n Error is: %s",cError);
		MEM_free(cError);
		exit(0); 
	}
}


/////////////////////////////////////////////////////////////////////////
#include "header.h"

ITK_user_main(int argc, char* argv[]){
	int iFail=0;
	char* cError=NULL;
	char* cUserID=NULL;
	char* cPassword=NULL;
	char* cGroup=NULL;
	char* cFolder=NULL;
	
	tag_t tFolder=NULLTAG;
	tag_t tUser=NULLTAG;
	tag_t tHomeFolder=NULLTAG;
	
	if(argc==5){
		cUserID=ITK_ask_cli_argument("-u=");
		cPassword=ITK_ask_cli_argument("-p=");
		cGroup=ITK_ask_cli_argument("-g=");
		cFolder=ITK_ask_cli_argument("-folder=");
		
		Report_Error(iFail=ITK_init_module(cUserID,cPassword,cGroup));
		printf("\n\n Login Success ");
		Report_Error(iFail=FL_create2(cFolder,"",&tFolder));
		printf("\n\n Folder Created");
		Report_Error(iFail=AOM_save_with_extensions(tFolder));
		printf("\n\n Folder Saved");
		Report_Error(iFail=SA_find_user2(cUserID,&tUser));
		printf("\n\n User found");
		Report_Error(iFail=SA_ask_user_home_folder(tUser,&tHomeFolder));
		printf("\n\n Users home folder found");
		Report_Error(iFail=FL_insert(tHomeFolder,tFolder,999));
		printf("\n\n Folder inserted into Home Folder");
	
		printf("\n\n Folder saved in Home Folder Saved");
		Report_Error(iFail=ITK_exit_module(TRUE));
		printf("\n\n Logout Success ");		
		
	}else{
		printf("\n\n Argument count is less or more");
	}
	return 0;
}
