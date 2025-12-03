#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>

in Report_Error(iFail){
	char* cError=NULL;
	if(iFail!=ITK_ok){
		EMH_ask_error_text(iFail,&cError);
		printf("\n\n Error is : %s",cError);
		MEM_free(cError);
		exit(0);
	}
}

//////////////////////////////////////////////////////////
#include "header.h"

int ITK_user_main(int argc, char* argv[]){
	
	int iFail=0;
	char* cError=NULL;
	char* cUserID=NULL;
	char* cPassword=NULL;
	char* cGroup=NULL;
	char* cItemID=NULL;
	char* cItemName=NULL;
	tag_t tItem=NULLTAG;
	tag_t tRev=NULLTAG;
	
	if(argc==6){
		cUserID=ITK_ask_cli_argument("-u=");
		cPassword=ITK_ask_cli_argument("-p=");
		cGroup=ITK_ask_cli_argument("-g=");
		cItemID=ITK_ask_cli_argument("-id=");
		cItemName=ITK_ask_cli_argument("-name=");
		
		Report_Error(iFail=ITK_init_module(cUserID,cPassword,cGroup));
		printf("\n\n login Success");
		
		Report_Error(iFail=ITEM_create_item(cItemID,cItemName,"Item","",&tItem,&tRev));
		printf("\n\n Item created ");
		
		Report_Error(iFail=ITEM_save_item(tItem));
		printf("\n\n Item Saved ");
		
		Report_Error(iFail=ITK_exit_module(TRUE));
		printf("\n\n logout Success");
		
		
	}else{
		printf("\n\n Argument count is less or more");
	}
	return 0;
}
