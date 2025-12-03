// ============================================================================
// Purpose   : To update ECN custom property on Item Revision during revise.
// Logic     : When Item Revision is revised (001,002,003...), update property
//             ECN_ID as follows:
//             - Rev 001 ? "0"
//             - Rev 002 ? "1"
//             - Rev 003 ? "2"
//             - Rev 004 ? "3"
//             - Rev 005 ? "A"
//             - Rev 006 ? "B"
//             - Rev 007 ? "C"
//             and so on.
// Author    : Ashish (Custom Exit Example)
// ============================================================================

//============================================================================
// ITK Customization Example
// Requirement: Set ECN attribute based on Item Revision sequence
//============================================================================

#include <stdio.h>
#include <tc/tc_startup.h>
#include <tcinit/tcinit.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
#include <tccore/aom.h>
#include <tccore/item.h>
#include <tc/emh.h>

// Export DLL functions
#define DLLAPI _declspec(dllexport)

// Function declarations
extern DLLAPI int Custom_exit_Ashish_register_callbacks();
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args);
extern DLLAPI int IRMF_after_revision_create(METHOD_message_t* msg, va_list args);

//============================================================================
// Register DLL Callbacks
//============================================================================
extern DLLAPI int Custom_exit_Ashish_register_callbacks() {
    // Register for login
    CUSTOM_register_exit("Custom_exit_Ashish", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks1);
    // Register for logout
    CUSTOM_register_exit("Custom_exit_Ashish", "USER_exit_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks2);
    return ITK_ok;
}

//============================================================================
// DLL Loaded After Login
//============================================================================
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args) {
    METHOD_id_t method_id;

    *decision = ALL_CUSTOMIZATIONS;
    printf("\n\n DLL Registered Successfully after Login \n\n");
    TC_write_syslog("\n\n DLL Registered Successfully after Login \n\n");

    // Hook into ItemRevision creation (after revision is created)
    METHOD_find_method("ItemRevision", "ITEM_create_rev", &method_id);
    METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)IRMF_after_revision_create, NULLTAG);

    return ITK_ok;
}

//============================================================================
// DLL Registered for Logout
//============================================================================
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args) {
    *decision = ALL_CUSTOMIZATIONS;
    printf("\n\n DLL Registered for Logout Success \n\n");
    TC_write_syslog("\n\n DLL Registered for Logout Success \n\n");
    return ITK_ok;
}

//============================================================================
// Post Action: After Item Revision is Created
//============================================================================
extern DLLAPI int IRMF_after_revision_create(METHOD_message_t* msg, va_list args) {
    tag_t item = NULLTAG;
    tag_t new_rev = NULLTAG;
    char* rev_id = NULL;

    // From ITEM_create_rev arguments ? get item and revision
    item = va_arg(args, tag_t);     // Item tag
    new_rev = va_arg(args, tag_t);  // Newly created Revision tag

    // Get the Revision ID (like "001", "002", "005", etc.)
    AOM_ask_value_string(new_rev, "item_revision_id", &rev_id);

    // Convert Revision ID (string) into integer
    int rev_num = atoi(rev_id);

    // Final ECN value buffer (string)
    // We keep it length 10 for safety (can hold "AA", "AB", etc. later if extended)
    char ecn_value[10] = "";

    if(rev_num >= 1 && rev_num <= 4) {
        // For first 4 revisions use numeric values
        // Example:
        //   rev_id = "001" ? rev_num = 1 ? ecn_value = "0"
        //   rev_id = "002" ? rev_num = 2 ? ecn_value = "1"
        sprintf(ecn_value, "%d", rev_num - 1);
    } else {
        // From 5th revision onwards ? use alphabets
        // Formula:
        //   letter_index = rev_num - 5
        //   rev_num = 5 ? letter_index = 0 ? 'A'
        //   rev_num = 6 ? letter_index = 1 ? 'B'
        //   rev_num = 7 ? letter_index = 2 ? 'C'
        //   ...
        //   rev_num = 30 ? letter_index = 25 ? 'Z'
        int letter_index = rev_num - 5;

        // Generate the alphabet using ASCII math
        // 'A' + 0 = 'A', 'A' + 1 = 'B', 'A' + 2 = 'C'
        sprintf(ecn_value, "%c", 'A' + letter_index);
    }

    // Lock revision before updating
    AOM_lock(new_rev);

    // Set the custom property "ecn_id" with the computed value
    // NOTE: Replace "ecn_id" with your actual custom property name in BMIDE
    AOM_set_value_string(new_rev, "ecn_id", ecn_value);

    // Save revision after update
    AOM_save_without_extensions(new_rev);

    // Unlock revision
    AOM_unlock(new_rev);

    // Free allocated memory
    if(rev_id) MEM_free(rev_id);

    return ITK_ok;
}

