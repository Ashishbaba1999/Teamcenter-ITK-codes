#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tc/emh.h>
#include<stdlib.h>


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


#include "Header.h"

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;

	Report_Error(iFail = ITK_auto_login());
	printf("\n Login Success");


	Report_Error(iFail = ITK_exit_module(TRUE));
	printf("\n Logout Success");
	

	return 0;
}
