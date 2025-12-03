#include<stdlib.h>
#include<tcinit/tcinit.h>
#include<qry/qry.h>
#include<tccore/project.h>
#include<bom/bom.h>
#include<tccore/tctype.h>

typedef struct {
	char* cItemID;
	char* cItemType;
	char* cItemName;
}input_t;

FILE* file;

int Report_Error(int iFail)
{
	char* cError = NULL;
	if (iFail != ITK_ok)
	{
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////
#include "Header.h"

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	int row_count = 0;

	tag_t tItemType = NULLTAG;
	tag_t tItemCreateInput = NULLTAG;
	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;

	input_t record;

	char buf[1024];
	const char delim[2] = ",";
	char* Token = NULL;
	char* cError = NULL;

	FILE* fp = TC_fopen("C:\\Users\\AdminV\\Desktop\\output.csv", "r");

	if (!fp)
	{
		printf("Can not open file \n ");
		exit(0);
	}
	else
	{
		Report_Error(ITK_init_module("infodba", "infodba", "dba"));
		printf("\n Login success \n");
		while (fgets(buf, 1024, fp))
		{
			row_count++;
			if (row_count == 1)
			{
				continue;
			}
			record.cItemID = strtok_s(buf, delim, &Token);
			record.cItemType = strtok_s(NULL, delim, &Token);
			record.cItemName = strtok_s(NULL, delim, &Token);
			Report_Error(ITEM_find_item(record.cItemID, &tItem));

			if (tItem == NULLTAG)
			{
				Report_Error(ITEM_create_item(record.cItemID, record.cItemName, record.cItemType, "", &tItem, &tRev));
				printf("\n\n ITEM create Success");

				Report_Error(ITEM_save_item(tItem));
				printf("\n\n ITEM Save Success");

			}
			else
			{
				printf("\n\n Item with ID %s is alredy Exist ", record.cItemID);
			}
		}
	}
	fclose(fp);
	return 0;
}
