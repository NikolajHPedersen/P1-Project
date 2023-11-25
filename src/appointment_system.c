#include "appointment_system.h"
#include "file_reader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int patient_creation(int const *CPR){
    int internal_CPR;
    char first_name[32];
    char last_name[32];


    if(CPR == NULL)
        input(" %d",&internal_CPR,"Enter patient CPR: \n");
    else
        internal_CPR = *CPR;

    input(" %s",&first_name,"Enter patient first name: \n");
    input(" %s",&last_name,"Enter patient last name: \n");

    unsigned short id = internal_CPR % 65521;
    char buffer[64];
    sprintf(buffer,"id: %d,first_name: %s,last_name: %s",id,first_name, last_name);

    //append_entry("test_db.txt",buffer);
    insert_entry("test_db.txt",buffer,1);

    return 0;
}

void input(char *format_string,void *ptr, char *printed_string){
    printf("%s",printed_string);
    scanf(format_string,ptr);
}
