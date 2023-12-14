#include "appointment_system.h"

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

void show_schedule(char file_name[]){
    date_t date;
    char block_id[20] = "";
    char current_line[100];
    char appointments[16][50];

    char res_id[20];

    int res;
    int index = 0;

    printf("For which date do you wish to view the schedule (dd/mm/yy):");
    scanf("%d/%d/%d",&date.day,&date.month,&date.year);

    FILE *fp = fopen(file_name,"r");
    date_to_id(date,block_id);

    char *check_ptr = find_block_id(fp,block_id);

    if(check_ptr == NULL){
        printf("The given date is weekend. No appointments during weekends");
        return;
    }


    for(int i = 1;i <= APPOINTMENTS_PER_DAY;i++){
        res = read_cpr(fp,res_id,current_line);

        if(res == 0){
            strcpy(appointments[index],current_line);
            index++;
        } else if(res == 1){
            break;
        }
    }

    if(index == 0){
        printf("No appointments on %d/%d/%d found\n",date.day,date.month,date.year);
        return;
    }

    printf("%d appointments on %d/%d/%d \n",index,date.day,date.month,date.year);
    for(int i = 0;i < index;i++){
        printf("%s",appointments[i]);
    }

}