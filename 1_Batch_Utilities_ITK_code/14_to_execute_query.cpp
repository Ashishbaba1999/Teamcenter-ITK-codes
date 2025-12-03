// exicute query using single entry (Item name)

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<qry/qry.h>
#include<tccore/workspaceobject.h>
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

///////////////////////////////////////////////////////////////////////

#include "Header.h"

int ITK_user_main(int argc, char* argv[]){
	
	int iFail=0;
    int iEntriesCount=0;
    char* cError=NULL;
	char* cUserID=NULL;
	char* cPassword=NULL;
	char* cGroup=NULL;
    char** cEntries=NULL;
    char** cValues=NULL ;

    tag_t tQuery=NULLTAG;

    if (ITK_ask_cli_argument("-h")||ITK_ask_cli_argument("-help")) {
		Display_Usage();
	}
		
    if(argc==4){
    	cUserID=ITK_ask_cli_argument("-u=");
    	cPassword=ITK_ask_cli_argument("-p=");
    	cGroup=ITK_ask_cli_argument("-g=");
    	
    	Report_Error(iFail=ITK_init_module(cUserID,cPassword,cGroup));
    	printf("\n\n Login Success");

        Report_Error(iFail=QRY_find2("Item Name", &tQuery));

        if(tQuery! = NULLTAG)
        {
            printf("\n\n Query find Success"); 
            Report_Error(iFail=QRY_find_user_entries(tQuery,&iEntriesCount,&cEntries,&cValues));
            if(cEntries!=NULL)
            {
                printf("\n\n Query entries find Success");

                for(i=0;i< iEntriesCount; i++)
                {
                    if(tc_strcmp(cEntries[i],"Item Name")==0)
                    {
                        tc_strcpy(cvalues[i],"*");
                        Report_Error(iFail=QRY_execute (tQuary,));

                    }
                }
            }
        }

        

       

        Report_Error(iFail=WSOM_ask_name2());


    	
    	Report_Error(iFail=ITK_exit_module(TRUE));
    	printf("\n\n Logout Success");   	
	}
	else{
		printf("\n\n Argument count is less or more");
	}
	return 0;
}

/*
Report_Error(iFail= ());
printf("\n\n  "); 
*/



