#include "appointment_system.h"
#include "file_reader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int patient_creation(unsigned int const *CPR){
    unsigned int internal_CPR;
    char first_name[32] = "";
    char last_name[32] = "";


    if(CPR == NULL)
        input(" %lld",&internal_CPR,"Enter patient CPR: \n");
    else
        internal_CPR = *CPR;

    input(" %s",&first_name,"Enter patient first name: \n");
    input(" %s",&last_name,"Enter patient last name: \n");

    char buffer[64];
    sprintf(buffer,"id: %010u,first_name: %s,last_name: %s",internal_CPR,first_name, last_name);

    append_entry("test_db.txt",buffer);
    //insert_entry("test_db.txt",buffer,2);

    return 0;
}

void input(char *format_string,void *ptr, char *printed_string){
    printf("%s",printed_string);
    scanf(format_string,ptr);
}


