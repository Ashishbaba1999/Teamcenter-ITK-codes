#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tc/emh.h>



#include "Header.h"

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char* cError = NULL;

	iFail=ITK_init_module("infodba", "infodba", "dba");
	if (iFail = ITK_ok)
	{
		printf("\n Login Success");
	}
	else
	{
		EMH_ask_error_text(iFail, &cError);
		printf("\n Error is : %s",cError);
		if (cError)MEM_free(cError);
	}
	

	return 0;
}


