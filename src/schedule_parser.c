#include "schedule_parser.h"

void assign_appointment(patient_t patient,char file_name[]){
    //FOR TESTING PURPOSES ONLY
    date_t test = {
        23,
        12,
        22
    };

    date_t appointment_date = assign_date(patient,file_name,test);

    char id[10];
    date_to_id(appointment_date, id);
    assign_appointment_to_patient(file_name, patient, id);
}

date_t assign_date(patient_t patient, char file_name[], date_t next_day){
    int range[2];

    char next_day_id[10] = " ";
    long id;

    date_to_id(next_day,next_day_id);
    id = (long)atoi(next_day_id);

    int line = find_entry_cpr(file_name,id);

    while(line == -1){
        next_day = add_day(next_day);
        date_to_id(next_day,next_day_id);
        id = (long)atoi(next_day_id);

        line = find_entry_cpr(file_name,id);
    }



    FILE *fp = fopen(file_name,"r");

    char current_line[100];
/*
    for(int i = 1;i <= line;i++){
        fgets(current_line,100,fp);
    }
*/
    go_to_line(line,fp,current_line);

    char cpr[15];

    for(int i = 1;i <= APPOINTMENTS_PER_DAY;i++){
        fgets(current_line,100,fp);
        substring(current_line,cpr,15,10);
        if(strcmp(cpr,"0") == 0){
            substring(current_line,next_day_id,3,6);
            break;
        }
    }
    if(strcmp(next_day_id," ") != 0){
        return id_to_date(next_day_id);
    }
    switch(patient.HWG){
        case 'A':
            range[0] = 2;
            range[1] = 3;
            break;
        case 'B':
            range[0] = 4;
            range[1] = 10;
            break;
        case 'C':
            range[0] = 11;
            range[1] = 30;
            break;
        default:
            printf("No assigned HWG");
            range[0] = 31;
            range[1] = 60;
    }

    date_t current_day = next_day;

    for(int i = 1;i <= APPOINTMENTS_PER_DAY;i++){
        for(int j = range[0];j < range[1];j++){
            current_day = future_date(current_day,j);

            date_to_id(current_day, cpr);
            id = (long)atoi(cpr);
            fclose(fp);
            line = find_entry_cpr(file_name,id);
            if(line == -1){
                continue;
            }
            fopen(file_name,"r");
            go_to_line(line + i,fp,current_line);

            fgets(current_line,100,fp);
            substring(current_line,cpr,15,10);

            if(strcmp(cpr,"0") == 0){
                substring(current_line,next_day_id,3,6);
                break;
            }
        }
    }
    fclose(fp);
    return id_to_date(next_day_id);
}



void assign_appointment_to_patient(char file_name[],patient_t patient, char block_id[]){
    int appointments_per_day = 60*WORKING_DAY/APPOINTMENT_DURATION;
    char current_line[100];
    char cpr[100];
    long id = (long)atoi(block_id);

    char patient_cpr[100];
    sprintf(patient_cpr, "%u",patient.patient_id);

    int insert_line = 0;

    int line = find_entry_cpr(file_name,id);

    FILE *fp = fopen(file_name, "r+");

    for(int i = 1;i <= line;i++){
        fgets(current_line,100,fp);
    }

    for(int i = 1;i <= appointments_per_day;i++){
        fgets(current_line,100,fp);
        substring(current_line,cpr,15,10);
        if(strcmp(cpr,"0") == 0){
            insert_line = i + line;
            break;
        }
    }
    fclose(fp);

    char line_to_insert[100];
    if(insert_line != 0){
        string_helper(current_line,patient_cpr,line_to_insert);
        change_entry(file_name,line_to_insert,insert_line - 1);
    }
}

date_t future_date(date_t date, int time_skip){
    for(int i = 1;i <= time_skip;i++){
        date = add_day(date);
    }
    return date;
}


void string_helper(char appointment_id[],char cpr[],char dest[]){
    char local[100];
    substring(appointment_id,local,0,15);
    sprintf(dest,"%s%s",local,cpr);
}

void go_to_line(int line,FILE *fp,char *current_line){
    for(int i = 1;i <= line;i++){
        fgets(current_line,100,fp);
    }
}