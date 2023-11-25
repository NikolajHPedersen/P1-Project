#include <stdio.h>
#include <stdlib.h>

#include "appointment_system.h"
#include "file_reader.h"

//This is the file containing the main function.
//Most things should be implemented in other files

int main(void){
    char menu_items[][32] = {
                        "Appointment creation",
                        "Patient creation",
                        "Rebook/delete appointment",
                        "Exit"};

    int user_input;
    int loop = 1;
    while(loop == 1){
        printf("Which submenu do you wish to access?\n");
        printf("%s[0], %s[1], %s[2], %s[3]\n",
               menu_items[0], menu_items[1], menu_items[2], menu_items[3]);

        scanf("%d",&user_input);

        switch (user_input) {
            case 0:
                break;
            case 1:
                patient_creation(NULL);
                break;
            case 2:
                break;
            case 3:
                loop = 0;
                break;
            default:
                printf("Invalid Input");
                break;
        }
    }

    return 0;
}