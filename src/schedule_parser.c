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

    date_t valid_dates[range[1] - range[0] + 1];

    int iter = 0, index = 0, dest;
    char temp_id[100] = " ";
    char iter_line[100] = " ";
    char *check_ptr;

    while(iter <= range[1] - range[0]){
        dest = -1;
        //current_day = future_date(next_day,range[0] + iter);
        current_day = add_day(current_day);

        date_to_id(current_day,temp_id);

        //sprintf(iter_line,"## %s",temp_id);
        rewind(fp);
        while(dest == -1){
            check_ptr = fgets(current_line,100,fp);
            if(check_ptr == NULL){
                break;
            }
            else if(strstr(current_line,temp_id) == 0){
                valid_dates[index] = current_day;

                index++;
                dest = 0;
            }
        }
        iter++;
    }

    char current_id[100] = " ";
    char c_line[100] = " ";

    char *n_check_pointer;
    int end = 0;

    if(index != 0){
        for(int i = 1;i <= APPOINTMENTS_PER_DAY;i++){
            for(int j = 0; j <= index;j++){
                date_to_id(valid_dates[j],current_id);
                printf("%s ",current_id);

                sprintf(c_line,"## %s",current_id);
                rewind(fp);
                printf("%s ", c_line);
                int k = 0;
                while(k == 0){
                    n_check_pointer = fgets(current_line,100,fp);
                    if(strstr(current_line,c_line) != NULL){
                        k = 1;
                    }
                    if(n_check_pointer == NULL){
                        k = 1;
                    }
                }
                if(n_check_pointer == NULL){
                    continue;
                }
                for(int t = 1;t < i;t++){
                    fgets(current_line,100,fp);
                }
                fgets(current_line,100,fp);
                substring(current_line,cpr,15,10);
                if(strcmp(cpr,"0") == 0){
                    substring(current_line,result_id,3,6);
                    end = 1;
                    break;
                }
            }
            if(end != 0){
                break;
            }
        }
    }


    if(strcmp(result_id," ")!= 0){
        fclose(fp);
        return id_to_date(result_id);
    }

    int it = range[1] + 1;
    date_t c_date = add_day(valid_dates[index]);


    for(int j = range[1] + 1;j <= DAYS_IN_SCHEDULE;j++){
        rewind(fp);

        date_to_id(c_date,current_id);
        sprintf(c_line, "## %s",current_id);

        int k = 0;
        while(k == 0){
            n_check_pointer = fgets(current_line,100,fp);
            if(strstr(current_line,c_line) != NULL){
                k = 1;
            }
            if(n_check_pointer == NULL){
                k = 1;
            }
        }

        for(int i = 1;i < APPOINTMENTS_PER_DAY;i++){

        }
        add_day(c_date);
        it++;
    }



    return next_day;
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