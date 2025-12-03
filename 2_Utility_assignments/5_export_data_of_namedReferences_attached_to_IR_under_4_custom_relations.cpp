/*write a code to check data of 4 relations under that relations files are attached on Item Revision .. export that data to csv file

Takes an Item Revision as input.

Checks for 4 relations under that Item Revision (example: IMAN_specification, IMAN_reference, IMAN_Rendering, IMAN_manifestation — or your custom relations).

For each relation, lists the attached datasets/files.

Exports the information to a CSV file.


// export_ir_relations_to_csv.c

Purpose: Export attached datasets/files under specific relations
         from an Item Revision to CSV.

Compile with ITK libs (similar to BOM utility).
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
ItemID,RevisionID,Relation,DatasetName,DatasetType,FileName
ABC123,A,IMAN_specification,SpecDoc1,MSWord,"SpecDoc1.docx"
ABC123,A,IMAN_reference,Drawing1,PDF,"Drawing1.pdf"
ABC123,A,IMAN_Rendering,Preview1,JPEG,"Preview1.jpg"

////////////////////////////////////////////////////////////////////////////////////////////////////////

// Successful code

#include <stdio.h>
#include <stdlib.h>
#include <tc/emh.h>
#include <tcinit/tcinit.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <tccore/grmtype.h>
#include <ae/dataset.h>
#include <sa/tcfile.h>
#include<tc/tc_util.h>

FILE* file;

int Report_Error(int iFail)
{
    char* cError = NULL;
    if (iFail != ITK_ok)
    {
        EMH_ask_error_text(iFail, &cError);
        printf("\n Error: %s\n", cError);
        MEM_free(cError);
        exit(1);
    }
    return iFail;
}












/////////////////////////////////////////////////////////////////
#include "Header.h"

int ITK_user_main(int argc, char* argv[])
{
    int iFail = ITK_ok;
    char* cUserID = NULL;
    char* cPassword = NULL;
    char* cGroup = NULL;
    char* cItem_id = NULL;
    tag_t tItem = NULLTAG;
    tag_t* tRevList = NULLTAG;
    tag_t tRev = NULLTAG;
    int iRevCount = 0;

    if (argc == 5)
    {
        cUserID = ITK_ask_cli_argument("-u=");
        cPassword = ITK_ask_cli_argument("-p=");
        cGroup = ITK_ask_cli_argument("-g=");
        cItem_id = ITK_ask_cli_argument("-id=");

        file = TC_fopen("IR_RELATION_EXPORT.csv", "w+");
        if (file == NULL)
        {
            printf("\n Cannot open CSV file\n");
            return 1;
        }

        // login
        Report_Error(ITK_init_module(cUserID, cPassword, cGroup));
        printf("\n Login Success\n");

        // find item
        Report_Error(ITEM_find_item(cItem_id, &tItem));
        if (tItem == NULLTAG)
        {
            printf("\n Item not found\n");
            return 1;
        }

        // list revisions
        Report_Error(ITEM_list_all_revs(tItem, &iRevCount, &tRevList));
        if (iRevCount == 0)
        {
            printf("\n No revisions found\n");
            return 0;
        }

        // CSV header
        TC_fprintf(file, "ItemID,RevisionID,Relation,DatasetName,DatasetType,FileName\n");
        

        // Define relations you want to check
        const char* relations[4] = { "IMAN_specification", "IMAN_reference", "IMAN_Rendering", "IMAN_manifestation" };

        for (int i = 0; i < iRevCount; i++)
        {
            tRev = tRevList[i];

            char* rev_id = NULL, * item_id = NULL;
            Report_Error(AOM_ask_value_string(tRev, "item_id", &item_id));
            Report_Error(AOM_ask_value_string(tRev, "item_revision_id", &rev_id));

            // loop through 4 relations
            for (int r = 0; r < 4; r++)
            {
                tag_t relType = NULLTAG;
                int nSecObjs = 0;
                tag_t* secObjs = NULL;

                Report_Error(GRM_find_relation_type(relations[r], &relType));
                if (relType == NULLTAG)
                    continue;

                Report_Error(GRM_list_secondary_objects_only(tRev, relType, &nSecObjs, &secObjs));
                if (nSecObjs > 0)
                {
                    for (int j = 0; j < nSecObjs; j++)
                    {
                        char* ds_name = NULL, * ds_type = NULL;
                        Report_Error(AOM_ask_value_string(secObjs[j], "object_name", &ds_name));
                        Report_Error(AOM_ask_value_string(secObjs[j], "object_type", &ds_type));

                        // list named references (files) of dataset
                        int nRefs = 0;
                        char** refNames = NULL;
                        Report_Error(AOM_ask_value_tags(secObjs[j], "ref_list", &nRefs, &refNames));

                        if (nRefs > 0)
                        {
                            for (int k = 0; k < nRefs; k++)
                            {
                                char* fileName = NULL;
                                tag_t dsFile = NULLTAG;

                                // Ask file from named reference
                                Report_Error(AOM_ask_value_tags(secObjs[j], refNames[k], &nRefs, &dsFile));

                                if (dsFile != NULLTAG)
                                {
                                    Report_Error(AOM_ask_value_string(dsFile, "original_file_name", &fileName));

                                    TC_fprintf(file, "%s,%s,%s,%s,%s,%s\n",
                                        item_id, rev_id, relations[r],
                                        ds_name, ds_type,
                                        fileName ? fileName : "");
                                        
                                    if (fileName) MEM_free(fileName);
                                }
                            }
                        }
                        else
                        {
                            TC_fprintf(file, "%s,%s,%s,%s,%s,%s\n",
                                item_id, rev_id, relations[r],
                                ds_name, ds_type, "");
                        }

                        if (ds_name) MEM_free(ds_name);
                        if (ds_type) MEM_free(ds_type);
                        if (refNames) MEM_free(refNames);
                    }
                }

                if (secObjs) MEM_free(secObjs);
            }

            if (item_id) MEM_free(item_id);
            if (rev_id) MEM_free(rev_id);
        }

        // cleanup
        if (tRevList) MEM_free(tRevList);
        fclose(file);

        Report_Error(ITK_exit_module(TRUE));
        printf("\n Logout Success\n");
    }
    else
    {
        printf("\n Usage: %s -u=<user> -p=<pw> -g=<group> -id=<item_id>\n", argv[0]);
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// write a code so that CSV heading is as below
// ItemID,RevisionID,IMAN_specification,IMAN_reference,IMAN_Rendering,IMAN_manifestation

#include <stdio.h>
#include <stdlib.h>
#include <tc/emh.h>
#include <tcinit/tcinit.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <tccore/grmtype.h>
#include <ae/dataset.h>
#include <sa/tcfile.h>
#include <tc/tc_util.h>
#include<fclasses/tc_string.h>

FILE* file;   // Global file pointer for CSV output

// Error handler function
int Report_Error(int iFail)
{
    char* cError = NULL;
    if (iFail != ITK_ok)   // If ITK call failed
    {
        EMH_ask_error_text(iFail, &cError);   // Get error text
        printf("\n Error: %s\n", cError);     // Print error
        MEM_free(cError);                     // Free allocated memory
        exit(1);                              // Exit program
    }
    return iFail;
}

/////////////////////////////////////////////////////////////////
#include "Header.h"

// Entry point of ITK utility
int ITK_user_main(int argc, char* argv[])
{
    char* cUserID = NULL;
    char* cPassword = NULL;
    char* cGroup = NULL;
    char* cItem_id = NULL;
    tag_t tItem = NULLTAG;
    tag_t* tRevList = NULLTAG;   // array of revision tags
    tag_t tRev = NULLTAG;
    int iRevCount = 0;

    // Expect exactly 4 arguments: user, password, group, item_id
    if (argc == 5)
    {
        // Get command line arguments
        cUserID = ITK_ask_cli_argument("-u=");
        cPassword = ITK_ask_cli_argument("-p=");
        cGroup = ITK_ask_cli_argument("-g=");
        cItem_id = ITK_ask_cli_argument("-id=");

        // Open CSV file for writing
        file = TC_fopen("IR_RELATION_EXPORT.csv", "w+");
        if (file == NULL)
        {
            printf("\n Cannot open CSV file\n");
            return 1;
        }

        // Login to Teamcenter
        Report_Error(ITK_init_module(cUserID, cPassword, cGroup));
        printf("\n Login Success\n");

        // Find the item by ItemID
        Report_Error(ITEM_find_item(cItem_id, &tItem));
        if (tItem == NULLTAG)
        {
            printf("\n Item not found\n");
            return 1;
        }

        // Get all revisions of the item
        Report_Error(ITEM_list_all_revs(tItem, &iRevCount, &tRevList));
        if (iRevCount == 0)
        {
            printf("\n No revisions found\n");
            return 0;
        }

        // Write CSV header
        TC_fprintf(file, "ItemID,RevisionID,IMAN_specification,IMAN_reference,IMAN_Rendering,IMAN_manifestation\n");

        // Relations to check
        const char* relations[4] = { "IMAN_specification", "IMAN_reference", "IMAN_Rendering", "IMAN_manifestation" };

        // Loop through each revision
        for (int i = 0; i < iRevCount; i++)
        {
            tRev = tRevList[i];

            // Get item_id and revision_id
            char* rev_id = NULL, * item_id = NULL;
            Report_Error(AOM_ask_value_string(tRev, "item_id", &item_id));
            Report_Error(AOM_ask_value_string(tRev, "item_revision_id", &rev_id));

            // Buffers to hold filenames for each relation
            char* relFiles[4] = { NULL, NULL, NULL, NULL };

            // Loop through 4 relations
            for (int r = 0; r < 4; r++)
            {
                tag_t relType = NULLTAG;
                int nSecObjs = 0;
                tag_t* secObjs = NULL;

                // Find relation type (e.g., IMAN_specification)
                Report_Error(GRM_find_relation_type(relations[r], &relType));
                if (relType == NULLTAG) continue;

                // List secondary objects (datasets) of this relation
                Report_Error(GRM_list_secondary_objects_only(tRev, relType, &nSecObjs, &secObjs));

                if (nSecObjs > 0)
                {
                    // Loop through each dataset in this relation
                    for (int j = 0; j < nSecObjs; j++)
                    {
                        int nRefs = 0;
                        char** refNames = NULL;

                        // Get all named references of dataset (e.g., PDF_Reference, UGPART)
                        Report_Error(AE_ask_dataset_named_refs(secObjs[j], &nRefs, &refNames));

                        if (nRefs > 0)
                        {
                            // Loop through each named reference
                            for (int k = 0; k < nRefs; k++)
                            {
                                tag_t dsFile = NULLTAG;
                                char* fileName = NULL;

                                // Get file tag for this named reference
                                Report_Error(AE_ask_dataset_named_ref(secObjs[j], refNames[k], &dsFile));
                                if (dsFile != NULLTAG)
                                {
                                    // Get actual OS filename
                                    Report_Error(AOM_ask_value_string(dsFile, "original_file_name", &fileName));

                                    if (relFiles[r] == NULL)
                                    {
                                        // If first file ? allocate new string
                                        relFiles[r] = (char*)MEM_alloc(strlen(fileName) + 1);
                                          tc_strcpy(relFiles[r], fileName);
                                    }
                                    else
                                    {
                                        // If already exists ? append with comma
                                        int newLen = strlen(relFiles[r]) + strlen(fileName) + 2;
                                        relFiles[r] = (char*)MEM_realloc(relFiles[r], newLen);
                                        tc_strcat(relFiles[r], ",");
                                        tc_strcat(relFiles[r], fileName);
                                    }

                                    if (fileName) MEM_free(fileName);
                                }
                            }
                        }
                        if (refNames) MEM_free(refNames);
                    }
                }

                // If no dataset or files found ? mark as NULL
                if (!relFiles[r])
                {
                    relFiles[r] = (char*)MEM_alloc(5);
                    tc_strcpy(relFiles[r], "NULL");
                }

                if (secObjs) MEM_free(secObjs);
            }

            // Write one row into CSV
            TC_fprintf(file, "%s,%s,%s,%s,%s,%s\n",
                item_id, rev_id,
                relFiles[0], relFiles[1], relFiles[2], relFiles[3]);

            // Free allocated memory
            for (int r = 0; r < 4; r++)
                if (relFiles[r]) MEM_free(relFiles[r]);

            if (item_id) MEM_free(item_id);
            if (rev_id) MEM_free(rev_id);
        }

        // cleanup
        if (tRevList) MEM_free(tRevList);
        fclose(file);

        Report_Error(ITK_exit_module(TRUE));
        printf("\n Logout Success\n");
    }
    else
    {
        printf("\n Usage: %s -u=<user> -p=<pw> -g=<group> -id=<item_id>\n", argv[0]);
    }
    return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// sample output

ItemID,RevisionID,IMAN_specification,IMAN_reference,IMAN_Rendering,IMAN_manifestation
TEST1234,A,drawing1.pdf,NULL,NULL,model1.prt,drawing1.prt
TEST1234,B,NULL,specB.txt,NULL,NULL

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
