#include "schedule_parser.h"

void assign_appointment(patient_t patient,char file_name[]){

    //FOR TESTING PURPOSES ONLY
    date_t test = {
        23,
        12,
        22
    };
    char id[10];
    date_to_id(test, id);
    assign_appointment_to_patient(file_name, patient, id);

}


void assign_appointment_to_patient(char file_name[],patient_t patient, char block_id[]){
    int appointments_per_day = 60*WORKING_DAY/APPOINTMENT_DURATION;
    char current_line[100];
    long id = (long)atoi(block_id);

    int line = find_entry_cpr(file_name,id);

    for(int i = 1;i <= appointments_per_day;i++){
         read_entry(file_name,line + i);


    }




}

