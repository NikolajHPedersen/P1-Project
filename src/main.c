#include <stdio.h>
#include <stdlib.h>

#include "appointment_system.h"
#include "file_reader.h"
//#include "schedule_parser.h"

//This is the file containing the main function.
//Most things should be implemented in other files

int main(void){
    sort_cpr_database("test_db.txt");
    char menu_items[][32] = {
                        "Appointment creation",
                        "Patient creation",
                        "Book Appointment",
                        "Show appointments",
                        "Search patient journal",
                        "Exit"};

    int user_input;
    int loop = 1;

    unsigned int cpr;

    while(loop == 1){
        printf("Which submenu do you wish to access?\n");
        printf("%s[0], %s[1], %s[2], %s[3], %s[4], %s[5]\n",
               menu_items[0], menu_items[1], menu_items[2], menu_items[3], menu_items[4], menu_items[5]);

        scanf("%d",&user_input);

        switch (user_input) {
            case 0:
                //create_empty_schedule("test_schedule.txt");

                printf("Please insert the the date for the appointment (dd/mm/yy): ");
                int day;
                int month;
                int year;
                scanf("%d/%d/%d", &day, &month, &year);

                date_t date = {
                        year,
                        month,
                        day
                };

                assign_appointments_new_patients("new_patients.txt","test_schedule.txt",date);

                break;
            case 1:
                patient_creation(NULL);
                break;
            case 2:
                book_appointment("test_db.txt");
                break;
            case 3:
                show_schedule("test_schedule.txt");

                break;
            case 4:
                printf("Enter patient CPR-Number: ");
                scanf("%10u", &cpr);

                printf("\n Result: %s\n", find_and_read_patient_line_binary("test_db.txt", cpr));
                break;
            case 5:
                loop = 0;
                break;
            default:
                printf("Invalid Input");
                break;
        }
    }
}