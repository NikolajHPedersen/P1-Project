#include "schedule_parser.h"

void assign_appointments_new_patients(char new_patient_db[], char schedule_file[], date_t date){
    int new_patients;
    patient_t *patients = read_new_patients(new_patient_db, &new_patients);
    if(patients == NULL){
        printf("No new patients to assign appointments to.");
        return;
    }
    sort_patients_by_hwg(patients, new_patients);
    for(int i = 0;i < new_patients;i++){
        assign_appointment(patients[i],date,schedule_file);
    }
    free(patients);
    remove(new_patient_db);
}


patient_t *read_new_patients(char file_name[],int *new_patients){
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL){
        printf("No new Patients");
        return NULL;
    }
    char buffer[20] = "";

    int lines_in_file = count_lines_in_file(fp);
    *new_patients = lines_in_file;

    rewind(fp);

    unsigned int *new_patients_cpr = malloc(sizeof(unsigned int)*lines_in_file);

    for(int i = 0;i < lines_in_file;i++){
        fgets(buffer,12,fp);
        new_patients_cpr[i] = strtoul(buffer,NULL,10);
    }
    fclose(fp);

    patient_t *patients = malloc(sizeof(patient_t)*lines_in_file);

    for(int i = 0;i < lines_in_file;i++){
        patients[i] = serialize_patient(find_and_read_patient_line_binary("test_db.txt",new_patients_cpr[i]));
    }

    return patients;
}

int count_lines_in_file(FILE *fp){
    char buffer[15];
    char *check_ptr;
    int count = 0;

    while(1){
        check_ptr = fgets(buffer,15,fp);

        if(check_ptr == NULL){
            return count;
        }
        count++;
    }
}


void assign_appointment(patient_t patient, date_t date,char file_name[]){

    date_t appointment_date = assign_date(patient,file_name,date);

    char id[10];
    date_to_id(appointment_date, id);
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

                sprintf(c_line,"## %s",current_id);
                rewind(fp);

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

    date_t c_date = valid_dates[index - 1];

    int end_loop = 0;


    for(int j = range[1] + 1;j <= 2*DAYS_IN_SCHEDULE;j++){
        rewind(fp);

        date_to_id(c_date,current_id);

        sprintf(c_line, "## %s",current_id);
        c_date =  add_day(c_date);

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

        for(int i = 1;i <= APPOINTMENTS_PER_DAY;i++){
            fgets(current_line,100,fp);
            substring(current_line,cpr,15,10);
            if(strcmp(cpr,"0") == 0){
                substring(current_line,result_id,3,6);
                end_loop = 1;
                break;
            }
        }
        if(end_loop != 0){
            break;
        }

    }

    if(strcmp(result_id," ")!= 0){
        fclose(fp);
        return id_to_date(result_id);
    }

    return next_day;
}

patient_t serialize_patient(char str[]){
    patient_t new_patient;
    sscanf(str,"id: %u,first_name: %[^,],last_name: %[^,],HWG: %c,app: %u",
           &new_patient.patient_id,
           new_patient.first_name,
           new_patient.last_name,
           &new_patient.HWG,
           &new_patient.appointments);

    return new_patient;
}

void assign_appointment_to_patient(char file_name[],patient_t patient, char block_id[]){
    int appointments_per_day = 60*WORKING_DAY/APPOINTMENT_DURATION;
    char current_line[100];
    char cpr[100];
    long id = (long)atoi(block_id);

    char patient_cpr[100];
    sprintf(patient_cpr, "%010u",patient.patient_id);

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

void sort_patients_by_hwg(patient_t *patients, int len){
    qsort(patients, len, sizeof(patient_t),patient_cmp);
}

int patient_cmp(const void * a, const void * b){
    patient_t *p1 = (patient_t *)a, *p2 = (patient_t *)b;
    return (p1->HWG) - (p2->HWG);
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