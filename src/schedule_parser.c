#include "schedule_parser.h"
//Adds new patients to the schedule.
//It takes three parameters, the name of the .txt file storing new patients,
//the name of the .txt file storing the schedule and the date
void assign_appointments_new_patients(char new_patient_db[], char schedule_file[], date_t date){
    //int storing the number of new patients. This number is also the number of lines in the text file containing new patients
    int new_patients;

    //The new patients are loaded into an array
    //read_new_patients uses malloc, therefore it needs to freed at the end
    patient_t *patients = read_new_patients(new_patient_db, &new_patients);

    //read_new_patients returns NULL when no new patients have been added
    //Therefore there are no patients to assign times to
    if(patients == NULL){
        printf("No new patients to assign appointments to.\n");
        return;
    }

    //The patients are sorted based on their HWG group with the order being A, B and then C
    sort_patients_by_hwg(patients, new_patients);

    //The patients are then assigned a time in the schedule
    for(int i = 0;i < new_patients;i++){
        assign_appointment(patients[i],date,schedule_file);
    }
    //the memory allocated for patients is freed
    free(patients);

    //Since the file storing new_patients is just an empty txt file it can be removed
    remove(new_patient_db);
}

//This function reads the CPR numbers stored in the file containing new patients, stores them in an array and
//then finds the corresponding patient in the patient database and adding them to an array. At the end the array is returned
//The function takes two parameters the name of file were new patients are stored and int * which is used as output parameter
//to find the number of new patients
patient_t *read_new_patients(char file_name[],int *new_patients){
    //Open file in read mode
    FILE *fp = fopen(file_name, "r");
    //If the file does not exist return a NULL pointer
    if(fp == NULL){
        return NULL;
    }
    //This buffer stores the string read from fgets()
    char buffer[20] = "";

    //We count the number of lines in the file. This i equivalent to the number of new patients
    *new_patients = count_lines_in_file(fp);

    //Rewind the file pointer to the start of the file, so we can begin reading from it
    rewind(fp);

    //Allocate memory for the cpr numbers we are to read
    unsigned int *new_patients_cpr = malloc(sizeof(unsigned int) * *new_patients);

    //Read the lines using fgets() and convert them to unsigned int
    for(int i = 0;i < *new_patients;i++){
        fgets(buffer,12,fp);
        new_patients_cpr[i] = strtoul(buffer,NULL,10);
    }

    //Close the file since we are done reading from it
    fclose(fp);

    //Allocate memory for the patients array
    patient_t *patients = malloc(sizeof(patient_t) * *new_patients);

    //Find serialize each patient from the new patient with the information stored in the patient DB
    for(int i = 0;i < *new_patients;i++){
        patients[i] = serialize_patient(find_and_read_patient_line_binary(PATIENT_DB,new_patients_cpr[i]));
    }

    free(new_patients_cpr);
    return patients;
}

//Count the lines in a file takes a FILE pointer as parameter
int count_lines_in_file(FILE *fp){
    char buffer[15];
    char *check_ptr;
    //Stores line count
    int count = 0;

    //fgets returns NULL when it reaches the end of the file
    //therefore we can get a line count by iterating until check_ptr == NULL and return count
    while(1){
        check_ptr = fgets(buffer,15,fp);

        if(check_ptr == NULL){
            return count;
        }
        count++;
    }
}

//This function assigns a date to a patient from a certain start date
//The parameters it takes are a patient struct, the date from which it should look for free times
//and the name of the file containing the schedule
void assign_appointment(patient_t patient, date_t date,char file_name[]){
    //The assign_date function figures out which date to give to the patient
    date_t appointment_date = assign_date(patient,file_name,date);

    //Convert the date to a char, which is the id of the block in which the patients time will be assigned
    char id[10];
    date_to_id(appointment_date, id);

    //This function changes the txt file containing the schedule
    assign_appointment_to_patient(file_name, patient, id);
}

//This function returns the date a given patient should be assigned an appointment on
//It takes three parameters the patient who needs an appointment, file containing the schedule
//and the date from which the appointments should be assigned
date_t assign_date(patient_t patient, char file_name[], date_t next_day){
    //The range of days that the patient can be assigned appointments
    int range[2];

    //Contains the id of the starting date
    char next_day_id[10] = " ";

    //The id that will be converted to the date that is returned
    char result_id[10] = " ";

    //the block id as a long
    long id;

    //Convert the date to a string and the string to a long
    date_to_id(next_day,next_day_id);
    id = (long)atoi(next_day_id);

    //look for the date in the schedule file and save the line number
    int line = find_entry_cpr(file_name,id);

    //If the date does not exist in the file (aka it is a weekend)
    //Then keep adding days until a weekday is reached
    //We do the same as above until a weekday is reached
    while(line == -1){
        next_day = add_day(next_day);
        date_to_id(next_day,next_day_id);
        id = (long)atoi(next_day_id);

        line = find_entry_cpr(file_name,id);
    }
    //Open the file
    FILE *fp = fopen(file_name,"r");

    //Buffer to store the line read from fgets
    char current_line[100];

    //Go to the line found in the while loop
    go_to_line(line,fp,current_line);

    //This string stores the cpr to be read as a substring from current_line
    char cpr[15];

    //Iterate through all appointments and look for an empty appointment
    for(int i = 1;i <= APPOINTMENTS_PER_DAY;i++){
        //read the current line
        fgets(current_line,100,fp);

        //take a substring of the current line. The start of the substring is character 15 because
        //in the format of the schedule that is where the CPR number of the assigned patients
        substring(current_line,cpr,15,10);

        //For empty appointments the CPR of the person is set to 0
        //If cpr is equivalent to 0 then the block id is saved in the result id string and the loop is broken
        if(strcmp(cpr,"0") == 0){
            //create substring containing the block id.
            //The block id always starts at the third character and has a length of 6
            substring(current_line,result_id,3,6);
            break;
        }
    }

    //Since result_id is initialized as a space. Check if it has changed. If it has convert result id to date and return it
    if(strcmp(result_id," ") != 0){
        return id_to_date(result_id);
    }

    //Set the range of valid days
    range[0] = 1;
    set_range_end(&range[1],patient.HWG);

    date_t current_day = next_day;

    //Fill the valid_dates array with all the days
    date_t valid_dates[range[1] - range[0] + 1];
    int iter = 0;
    while(iter <= range[1] - range[0]){
        current_day = add_day(current_day);
        valid_dates[iter] = current_day;
        iter++;
    }

    //
    char current_id[100] = " ";
    char c_line[100] = " ";

    char *n_check_pointer;
    int end = 0;

    for(int i = 1;i <= APPOINTMENTS_PER_DAY;i++){
        for(int j = 0; j <= iter;j++){
            date_to_id(valid_dates[j],current_id);

            rewind(fp);

            int k = 0;
            while(k == 0){
                n_check_pointer = fgets(current_line,100,fp);
                if(strstr(current_line,current_id) != NULL){
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

    if(strcmp(result_id," ")!= 0){
        fclose(fp);
        return id_to_date(result_id);
    }

    date_t c_date = valid_dates[iter - 1];

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

void set_range_end(int *range_end,char HWG){
    switch(HWG){
        case 'A':
            *range_end = 2;
            break;
        case 'B':
            *range_end = 9;
            break;
        case 'C':
            *range_end = 29;
            break;
        default:
            //printf("No assigned HWG");
            *range_end = 60;
            break;
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
//Sorts patients within an array by their HWG group using qsort
void sort_patients_by_hwg(patient_t *patients, int len){
    qsort(patients, len, sizeof(patient_t),patient_cmp);
}

//Comparison function used by qsort to sort patients
int patient_cmp(const void *a, const void *b){
    //Two patient pointers are created and the two void pointers are cast to the same type
    patient_t *p1 = (patient_t *)a, *p2 = (patient_t *)b;
    //The return value for the comparator function needs to follow
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