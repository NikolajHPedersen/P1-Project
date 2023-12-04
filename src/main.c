#include <stdio.h>
#include <stdlib.h>

#include "appointment_system.h"
#include "file_reader.h"
#include "schedule_parser.h"

//This is the file containing the main function.
//Most things should be implemented in other files

int main(void){
    char menu_items[][32] = {
                        "Appointment creation",
                        "Patient creation",
                        "Rebook/delete appointment",
                        "Show appointments",
                        "Exit"};

    int user_input;
    int loop = 1;

    while(loop == 1){
        printf("Which submenu do you wish to access?\n");
        printf("%s[0], %s[1], %s[2], %s[3], %s[4]\n",
               menu_items[0], menu_items[1], menu_items[2], menu_items[3], menu_items[4]);

        scanf("%d",&user_input);

        patient_t test;

        switch (user_input) {
            case 0:
                //create_empty_schedule("test_schedule.txt");

                assign_appointment(test, "test_schedule.txt");
                break;
            case 1:
                patient_creation(NULL);
                break;
            case 2:
                change_entry_v2("test_db.txt","This line was changed again!",1);
                printf("%s", read_entry("test_db.txt",6));
                printf("%d", find_entry_cpr("test_db.txt",32872));
                break;
            case 3:

                break;
            case 4:
                loop = 0;
                break;
            default:
                printf("Invalid Input");
                break;
        }
    }

    return 0;
}