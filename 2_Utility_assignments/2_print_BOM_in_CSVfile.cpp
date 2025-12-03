


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


////////////////////////////////////////////////////////////////////////////////////////
//to print Bom with children

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

////////////////////////////////////////////////////////////////////////////////////////////

#include "Header.h"

int ITK_user_main(int argc, char* argv[]) {
    char* cUser = ITK_ask_cli_argument("-u=");
    char* cPass = ITK_ask_cli_argument("-p=");
    char* cGroup = ITK_ask_cli_argument("-g=");
    char* cItem = ITK_ask_cli_argument("-item=");

    int iRevCount = 0;
    int iNumOfChild = 0;
    char* cObjectName = NULL;
    char* cItemId = NULL;
    tag_t tItem = NULLTAG;
    tag_t* tRevList = NULLTAG;
    tag_t tWindow = NULLTAG;
    tag_t tBomLine = NULLTAG;
    tag_t* tBomChildren = NULLTAG;

    fp = TC_fopen("SH_BOM_Report.syslog", "w");
        
    
    if (cUser && cPass && cGroup) {
        if (fp != NULL) {
            TC_fprintf(fp, "Object Name, Item Id, Revision ID, Assembly Level, Parent Assembly, Owner, Group");

            if (cItem != NULL) {
                Report_Error(ITK_init_module(cUser, cPass, cGroup));
                printf("\n Login successful");

                Report_Error(ITEM_find_item(cItem, &tItem));
                if (tItem != NULLTAG) {
                    printf("\n Item found successful");

                    Report_Error(ITEM_list_all_revs(tItem, &iRevCount, &tRevList));
                    if (tRevList != NULLTAG) {
                        printf("\n Revision list found successful");

                        for (int i = 0; i < iRevCount; i++) {
                            Report_Error(BOM_create_window(&tWindow));
                            Report_Error(BOM_set_window_top_line(tWindow, tItem, tRevList[i], NULLTAG, &tBomLine));

                            Report_Error(AOM_UIF_ask_value(tBomLine, "bl_rev_object_name", &cObjectName));
                            Report_Error(AOM_UIF_ask_value(tBomLine, "bl_item_item_id", &cItemId));

                            TC_fprintf(fp, "\n%s,%s", cObjectName, cItemId);

                            printf("\n %s,%s", cObjectName, cItemId);
                            Report_Error(BOM_line_ask_all_child_lines(tBomLine, &iNumOfChild, &tBomChildren));
                            if (iNumOfChild > 0) {
                                bom_sub_child(tBomChildren, iNumOfChild);
                            }
                            BOM_close_window(tWindow);
                        }
                        if (tRevList) {
                            MEM_free(tRevList);
                        }
                    }
                }
                ITK_exit_module(ITK_ok);
                printf("\n Logout successfull");
            }
            fflush(fp);
            fclose(fp);
        }
        else {
            printf("\n File not created");
            exit(0);
        }
    }
    else {
        printf("\n Enter the credentials properly");
    }
    return 0;
}
// recursion for child of next level
int bom_sub_child(tag_t* tBomChildren, int iNumOfChild) {
    int iFail = ITK_ok;
    int iNumOfSubChild = 0;
    char* cObjectName = NULL;
    char* cItemId = NULL;
    tag_t* tBomLine = NULLTAG;
    tag_t* tSubChildren = NULLTAG;
    for (int j = 0; j < iNumOfChild; j++) {
        Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_object_name", &cObjectName));
        Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_item_item_id", &cItemId));

        TC_fprintf(fp, "\n%s,%s", cObjectName, cItemId);

        BOM_line_ask_all_child_lines(tBomChildren[j], &iNumOfSubChild, &tSubChildren);
        if (iNumOfSubChild > 0) {
            bom_sub_child(tSubChildren, iNumOfSubChild); // recursion will end to the last level of child.
        }
    }
    return iFail;
}



