#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tc/emh.h>
#include<stdlib.h>

void Display_Usage() {
	printf("\n\n Teamcenter Utility to Import Named Referance to Dataset");
	printf("\n\n -u=\t Login User ID");
	printf("\n\n -p=\t Login User Password");
	printf("\n\n -g=\t Login User Group");
	printf("\n\n -h\t to display the detailed information of utility");
	printf("\n\n -help\t to display the detailed information of utility");
	exit(0);
}

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

////////////////////////////////////////////////////////////////////////////////


#include "Header.h"

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	
	if (ITK_ask_cli_argument("-h") || ITK_ask_cli_argument("-help")
	{
		Display_Usage();
	}

	if (argc == 4)
	{
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGroup = ITK_ask_cli_argument("-g=");
		
		Report_Error(iFail = ITK_init_module(cUserID,cPassword,cGroup));
		printf("\n Login Success");

		Report_Error(iFail = ITK_exit_module(TRUE));
		printf("\n Logout Success");


		
	}
	else
	{
		printf("Argument Count is less or more");
	}

	return 0;

}



