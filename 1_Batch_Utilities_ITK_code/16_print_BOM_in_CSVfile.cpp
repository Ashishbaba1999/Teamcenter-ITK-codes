


// print bom structure in csv file

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<bom/bom.h>
#include<tccore/aom_prop.h>
#include<tc/tc_util.h>

FILE* file;

int Report_Error(int iFail)
{
	char* cError = NULL;
	if (iFail != ITK_ok) {
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);
	}
}

//////////////////////////////////////////////////////////////////////////
#include "Header.h"

int ITK_user_main(int argc, char* argv[])
{

	int iFail = 0;
	int iNumberOfchild = 0;
	int iRevCount = 0;
	int i = 0, j = 0, k = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGrp = NULL;
	char* cItem_id = NULL;

	char* cObject_Name = NULL;
	char* cItemID = NULL;
	char* cRevision_ID = NULL;
	char* cAssembly_Level = NULL;
	char* cParent_Assembly = NULL;
	char* cOwner = NULL;
	char* cGroup = NULL;

	tag_t tItem = NULLTAG;
	tag_t* tRevList = NULLTAG;
	tag_t tWindow = NULLTAG;
	tag_t tBomLine = NULLTAG;
	tag_t* tBomChildren = NULLTAG;
	tag_t tChild = NULLTAG;
	tag_t* tSubChilds = NULLTAG;

	if (argc == 5)
	{
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGrp = ITK_ask_cli_argument("-g=");
		cItem_id = ITK_ask_cli_argument("-id=");

		file = TC_fopen("BOM_REPORT.csv", "w+");
		if (file != NULL)
		{
			TC_fprintf(file, "Object Name, Item ID , Revision ID,Assembly Level,Parent Assembly, Owner, Group");
			
			if (cItem_id != NULL)
			{
				Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
				printf("\n\n Login Success");

				Report_Error(ITEM_find_item(cItem_id, &tItem));
				if (tItem != NULLTAG)
				{
					printf("\n\n Item found success");
					Report_Error(ITEM_list_all_revs(tItem, &iRevCount, &tRevList));
					if (tRevList != NULL)
					{
						printf("\n\n Item Revisons found success");
						for (i = 0; i < iRevCount; i++)
						{
							Report_Error(BOM_create_window(&tWindow));
							Report_Error(BOM_set_window_top_line(tWindow, tItem, tRevList[i], NULLTAG, &tBomLine));

							Report_Error(AOM_UIF_ask_value(tBomLine,"bl_rev_object_name",&cObject_Name));
							Report_Error(AOM_UIF_ask_value(tBomLine,"bl_item_id", &cItemID));
							Report_Error(AOM_UIF_ask_value(tBomLine,"bl_item_revision_id",&cRevision_ID));
							Report_Error(AOM_UIF_ask_value(tBomLine,"bl_level_starting_0", &cAssembly_Level));
							Report_Error(AOM_UIF_ask_value(tBomLine,"bl_formatted_parent_name",&cParent_Assembly));
							Report_Error(AOM_UIF_ask_value(tBomLine,"bl_rev_owning_user",&cOwner));
							Report_Error(AOM_UIF_ask_value(tBomLine,"bl_rev_ownning_group",&cGroup));

							TC_fprintf(file, "\n%s,%s,%s,%s,%s,%s,%s", cObject_Name, cItemID, cRevision_ID, cAssembly_Level, cParent_Assembly, cOwner, cGroup);

							Report_Error(BOM_line_ask_all_child_lines(tBomLine, &iNumberOfchild, &tBomChildren));
							if (iNumberOfchild > 0)
							{
								for(j=0; j< iNumberOfchild; j++)
								{
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_object_name", &cObject_Name));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_item_id", &cItemID));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_item_revision_id", &cRevision_ID));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_level_starting_0", &cAssembly_Level));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_formatted_parent_name", &cParent_Assembly));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_owning_user", &cOwner));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_ownning_group", &cGroup));

								TC_fprintf(file, "\n%s,%s,%s,%s,%s,%s,%s", cObject_Name, cItemID, cRevision_ID, cAssembly_Level, cParent_Assembly, cOwner, cGroup);
								}
							}

						}

					}
					if (tRevList)
					{
						MEM_free(tRevList);
					}
					if (tBomChildren)
					{
						MEM_free(tBomChildren);
					}
					BOM_close_window(tWindow);


				}

				Report_Error(iFail = ITK_exit_module(TRUE));
				printf("\n\n Logout Success");
			}
			fflush(file);
			fclose(file);
		}
		else
		{
			printf("\n\n File Not Found");
		}


	}
	else
	{
		printf("\n\n Argument count is less or more");
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




#include <stdio.h>
#include <stdlib.h>
#include <tc/tc_startup.h>
#include <tcinit/tcinit.h>
#include <tc/emh.h>
#include <tccore/item.h>
#include <tccore/aom_prop.h>
#include <bom/bom.h>
#include <tc/tc_util.h>

FILE* file;

int Report_Error(int iFail) {
    char* cError = NULL;
    if (iFail != ITK_ok) {
        EMH_ask_error_text(iFail, &cError);
        printf("\nError: %s\n", cError);
        MEM_free(cError);
        exit(1);
    }
    return iFail;
}


// Recursive function to print BOM
void print_bom_recursive(tag_t bom_line) {
    int iChildCount = 0;
    tag_t* tChildren = NULL;
    char* cObject_Name = NULL, * cItemID = NULL, * cRevision_ID = NULL;
    char* cAssembly_Level = NULL, * cParent_Assembly = NULL, * cOwner = NULL, * cGroup = NULL;

    // Print current line properties
    Report_Error(AOM_UIF_ask_value(bom_line, "bl_rev_object_name", &cObject_Name));
    Report_Error(AOM_UIF_ask_value(bom_line, "bl_item_id", &cItemID));
    Report_Error(AOM_UIF_ask_value(bom_line, "bl_item_revision_id", &cRevision_ID));
    Report_Error(AOM_UIF_ask_value(bom_line, "bl_level_starting_0", &cAssembly_Level));
    Report_Error(AOM_UIF_ask_value(bom_line, "bl_formatted_parent_name", &cParent_Assembly));
    Report_Error(AOM_UIF_ask_value(bom_line, "bl_rev_owning_user", &cOwner));
    Report_Error(AOM_UIF_ask_value(bom_line, "bl_rev_owning_group", &cGroup));

    TC_fprintf(file, "%s,%s,%s,%s,%s,%s,%s\n",
        cObject_Name, cItemID, cRevision_ID,
        cAssembly_Level, cParent_Assembly, cOwner, cGroup );

    MEM_free(cObject_Name);
    MEM_free(cItemID);
    MEM_free(cRevision_ID);
    MEM_free(cAssembly_Level);
    MEM_free(cParent_Assembly);
    MEM_free(cOwner);
    MEM_free(cGroup);

    // Now get children and recurse
    Report_Error(BOM_line_ask_all_child_lines(bom_line, &iChildCount, &tChildren));
    for (int i = 0; i < iChildCount; i++) {
        print_bom_recursive(tChildren[i]);
    }
    if (tChildren) MEM_free(tChildren);
}


#include "Header.h"


int ITK_user_main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("\nUsage: program -u=USER -p=PASS -g=GROUP -id=ITEMID\n");
        return 1;
    }

    char* cUserID = ITK_ask_cli_argument("-u=");
    char* cPassword = ITK_ask_cli_argument("-p=");
    char* cGrp = ITK_ask_cli_argument("-g=");
    char* cItem_id = ITK_ask_cli_argument("-id=");

    Report_Error(ITK_init_module(cUserID, cPassword, cGrp));
    printf("\nLogin Success\n");

    tag_t tItem = NULLTAG;
    Report_Error(ITEM_find_item(cItem_id, &tItem));

    int iRevCount = 0;
    tag_t* tRevList = NULL;
    Report_Error(ITEM_list_all_revs(tItem, &iRevCount, &tRevList));

    file = TC_fopen("BOM_REPORT.csv", "w+");
    TC_fprintf(file, "Object Name,Item ID,Revision ID,Assembly Level,Parent Assembly,Owner,Group\n");

    for (int i = 0; i < iRevCount; i++) {
        tag_t tWindow = NULLTAG, tBomLine = NULLTAG;
        Report_Error(BOM_create_window(&tWindow));
        Report_Error(BOM_set_window_top_line(tWindow, tItem, tRevList[i], NULLTAG, &tBomLine));

        // Recursive print starting from top line
        print_bom_recursive(tBomLine);

        BOM_close_window(tWindow);
    }

    if (tRevList) MEM_free(tRevList);
    fclose(file);

    Report_Error(ITK_exit_module(TRUE));
    printf("\nLogout Success\n");
    return ITK_ok;
}





