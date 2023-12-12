#include "appointment_system.h"
#include "file_reader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int patient_creation(unsigned int const *CPR){
    unsigned int internal_CPR;
    char first_name[32] = "";
    char last_name[32] = "";
    char HWG;

    if(CPR == NULL)
        input(" %lld",&internal_CPR,"Enter patient CPR: \n");
    else
        internal_CPR = *CPR;

    input(" %s",&first_name,"Enter patient first name: \n");
    input(" %s",&last_name,"Enter patient last name: \n");
    input(" %c",&HWG,"Enter patient HWG group: \n");

    char buffer[100];
    sprintf(buffer,"id: %010u,first_name: %s,last_name: %s,HWG: %c,app: 0",internal_CPR,first_name, last_name,HWG);
    append_entry("test_db.txt",buffer);


    char buffer_new_patient[20];
    sprintf(buffer_new_patient, "%010u",internal_CPR);
    append_entry("new_patients.txt",buffer_new_patient);

    sort_cpr_database("test_db.txt");

    return 0;
}

void input(char *format_string,void *ptr, char *printed_string){
    printf("%s",printed_string);
    scanf(format_string,ptr);
}
