#include "schedule_lib.h"


#define APPOINTMENT_DURARTION 30

/* Outline of schedule format:
 * block id: date in year-month-day
 * Beginning of block: --
 * End of Block: /-
 * appointment id: date + time e.g. an appointment 25/09/23 12:00 gets the id 2309251200
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

char *get_date_id(struct tm *date){
    char id[10];
    char format_string[] = "%y%m%d";
    strftime(id, 10,format_string,date);

    return strdup(id);
}

char *substring(){

}

date id_to_date(char id[]){
    date new_date;

    new_date.year = strtol()


    return new_date;
}

void create_schedule(char file_name[]){
    time_t current_time;
    struct tm *info;
    time(&current_time);
    info = localtime(&current_time);



    char *start_id = get_date_id(info);



    printf("%s",start_id);

}