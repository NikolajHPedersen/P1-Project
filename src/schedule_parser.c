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
    printf("%s\n",id);
    assign_appointment_to_patient(file_name, patient, id);
}

date_t assign_date(patient_t patient, char file_name[], date_t next_day){
    int range[2];

    char next_day_id[10] = " ";
    char result_id[10] = " ";

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

    go_to_line(line,fp,current_line);

    char cpr[15];

    for(int i = 1;i <= APPOINTMENTS_PER_DAY;i++){
        fgets(current_line,100,fp);
        substring(current_line,cpr,15,10);
        if(strcmp(cpr,"0") == 0){
            substring(current_line,result_id,3,6);
            break;
        }
    }
    if(strcmp(result_id," ") != 0){
        return id_to_date(result_id);
    }
    switch(patient.HWG){
        case 'A':
            range[0] = 1;
            range[1] = 2;
            break;
        case 'B':
            range[0] = 3;
            range[1] = 9;
            break;
        case 'C':
            range[0] = 10;
            range[1] = 29;
            break;
        default:
            printf("No assigned HWG");
            range[0] = 31;
            range[1] = 60;
    }

    date_t current_day = next_day;

    for(int i = 1;i <= APPOINTMENTS_PER_DAY;i++){
        for(int j = range[0];j <= range[1];j++){
            current_day = future_date(current_day,j);
            printf("date: %02d/%02d/%02d ",current_day.weekday,current_day.month,current_day.year);
            date_to_id(current_day, cpr);
            id = (long)atoi(cpr);
            rewind(fp);
            line = find_block_id(fp,id);
            if(line == -1){
                continue;
            }

            fgets(current_line,100,fp);
            //printf("%s",current_line);
            substring(current_line,cpr,15,10);

            if(strcmp(cpr,"0") == 0){
                substring(current_line,result_id,3,6);
                break;
            }
            current_day = next_day;
        }
    }
    fclose(fp);
    if(strcmp(result_id," ")!= 0){
        return id_to_date(result_id);
    }


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

int find_block_id(FILE *fp, long block_id){
    char cpr_str[10];
    sprintf(cpr_str, "%ld", block_id);

    char buffer[100] = "";

    char *check_ptr;

    int count = 1;

    for(;;count++){
        check_ptr = fgets(buffer,100,fp);
        if(check_ptr == NULL){
            return -1;
        }
        if(strstr(buffer, cpr_str)){
            break;
        }
    }

    return count;
}