// ITEM creation using TCType API

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/tctype.h>

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

///////////////////////////////////////////////////////////////////////

#include "Header.h"

int ITK_user_main(int argc, char* argv[]){
	
	int iFail=0;
    char* cError=NULL;
	char* cUserID=NULL;
	char* cPassword=NULL;
	char* cGroup=NULL;
	
	tag_t input = NULLTAG;
	tag_t bo = NULLTAG;
	
		
    if(argc==4){
    	cUserID=ITK_ask_cli_argument("-u=");
    	cPassword=ITK_ask_cli_argument("-p=");
    	cGroup=ITK_ask_cli_argument("-g=");
    	
    	
    	
    	Report_Error(iFail=ITK_init_module(cUserID,cPassword,cGroup));
    	printf("\n\n Login Success");
    	
    	TCTYPE_ask_type();
    	
    	TCTYPE_construct_create_input();
    	
    	TCTYPE_create_object(input,&bo);
    	
    	
    	
    	Report_Error(iFail=ITK_exit_module(TRUE));
    	printf("\n\n Logout Success");   	
	}
	else{
		printf("\n\n Argument count is less or more");
	}
	return 0;
}


