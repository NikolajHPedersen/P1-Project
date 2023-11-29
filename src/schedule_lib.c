#include "schedule_lib.h"


#define APPOINTMENT_DURARTION 30

/* Outline of schedule format:
 * block id: date_t in year-month-day
 * Beginning of block: --
 * End of Block: /-
 * appointment id: date_t + time e.g. an appointment 25/09/23 12:00 gets the id 2309251200
 *
 * appointment format:
 * Empty appointment: 2309251200 - 0
 * Taken appointment: 2309251200 - xxxxxxxxxx (cpr)
 * general format: id - cpr (0 if empty)
 *
 * example:
 * -- 230925
 *    2309250800-xxxxxxxxxx:xray
 *    2309250830-xxxxxxxxxx:mri
 *    2309250900-0
 * /-
 */



void create_schedule(char file_name[]){
    time_t current_time;
    struct tm *info;
    time(&current_time);
    info = localtime(&current_time);

    char *start_id = get_date_id(info);
    date_t start_date = id_to_date(start_id);
    start_date.weekday = (weekday_e)get_weekday(info);



    printf("%s",start_id);

}

char *get_date_id(struct tm *date){
    char id[10];
    char format_string[] = "%y%m%d";
    strftime(id, 10,format_string,date);

    return strdup(id);
}

date_t id_to_date(char id[]){
    date_t new_date;

    new_date.year = atoi(substring(id,0,1));
    new_date.month = atoi(substring(id,2,3));
    new_date.day = atoi(substring(id,4,5));

    return new_date;
}
int get_weekday(struct tm *time){
    char buffer[1];
    strftime(buffer,sizeof(int),"%w",time);
    return atoi(buffer);
}


char *substring(char str[],int start,int end){
    char buffer[end - start];

    for(int i = start;i <= end;i++){
        strcat(buffer,&str[i]);
    }
    return strdup(buffer);
}

