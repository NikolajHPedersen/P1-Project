#include "schedule_lib.h"



/* Outline of schedule format:
 * block id: date_t in year-month-day
 * Beginning of block: ##
 * End of Block: /-
 * appointment id: date_t + time e.g. an appointment 25/09/23 12:00 gets the id 2309251200
 *
 * appointment format:
 * Empty appointment: 2309251200 - 0
 * Taken appointment: 2309251200 - xxxxxxxxxx (cpr)
 * general format: id - cpr (0 if empty)
 *
 * example:
 * ## 230925
 *    2309250800-xxxxxxxxxx:xray
 *    2309250830-xxxxxxxxxx:mri
 *    2309250900-0
 * //
 */

void create_empty_schedule(char file_name[]){
    //get the current time
    time_t current_time;
    struct tm *info;
    time(&current_time);
    info = localtime(&current_time);

    //Create an id based on that time information
    char *start_id = get_date_id(info);

    printf("\n%s\n",start_id);

    //Create a new date struct from the id
    date_t start_date = id_to_date(start_id);

    printf("\n%d/%d/%d\n",start_date.year,start_date.month,start_date.day);

    printf("\n%s\n",start_id);

    start_date.weekday = (weekday_e)get_weekday(info);

    date_t current_day = id_to_date(start_id);
    current_day.weekday = (weekday_e)get_weekday(info);

    date_t next_day = id_to_date(start_id);
    next_day.weekday = (weekday_e)get_weekday(info);

    char current_id[11];

    FILE *fp = fopen(file_name,"a");
    if(fp == NULL){
        printf("Something went wrong");
        exit(EXIT_FAILURE);
    }

    for(int i = 0;i < DAYS_IN_SCHEDULE;i++){
        current_day = next_day;
        if(next_day.weekday == sunday || next_day.weekday == saturday){
            printf("Got this far! 3");
            append_line_schedule(fp, "++ Weekend");
            next_day = add_day(current_day);
        } else{
            date_to_id(next_day, current_id);

            add_block(fp,current_id);
            next_day = add_day(current_day);

        }
    }
    fclose(fp);
}

void add_block(FILE *fp, char *id){
    char *buffer = calloc(25,sizeof(char));
    if(buffer == NULL){
        printf("something went wrong!");
        exit(EXIT_FAILURE);
    }

    sprintf(buffer,"## %s",id);

    append_line_schedule(fp,buffer);

}
void append_line_schedule(FILE *fp,char *message){
    fprintf(fp,"%s\n",message);
}


//takes a struct tm and turns it into an id
char *get_date_id(struct tm *date){
    char id[10];
    char format_string[] = "%y%m%d";
    strftime(id, 10,format_string,date);

    return strdup(id);
}

//Adds a day to the date struct it is given
/*
void add_day(date_t *date){
    date->day++;
    int thirty_one[] = {1, 3,5,7,8,10,12};
    int thirty[] = {4,6,9,11};

    if(date->day == 31 && contained_in(thirty, 4, date->month)){
        date->day = 1;
        date->month++;
    }
    else if(date->day == 32 && contained_in(thirty_one, 7, date->month)){
        date->day = 1;
        date->month++;
        if(date->month == 13){
            date->month = 1;
            date->year++;
        }
    }
    else if(date->month == 2 && is_leap_year(date->year+2000) == 0 && date->day == 29){
        date->day = 1;
        date->month++;
    }
    else if(date->month == 2 && is_leap_year(date->year+2000) == 1 && date->day == 30){
        date->day = 1;
        date->month++;
    }

    date->weekday = (date->weekday + 1) % 7;
}
*/

date_t add_day(date_t date){
    int days_in_month[12] = {31, 28, 31,30,31,30,31,31,30,31,30,31};

    date.day = date.day + 1;
    if(date.month == 2 && date.day == 29){
        if(is_leap_year(date.year + 2000) == 1){
            days_in_month[1] = 29;
        }
    }

    if(date.day > days_in_month[date.month - 1]){
        date.day = 1;
        date.month = date.month + 1;
        if(date.month > 12){
            date.month = 1;
            date.year = date.year + 1;
        }
    }
    date.weekday = (date.weekday + 1) % 7;

    return date;
}



//returns 1 if leap year else returns 0;
int is_leap_year(int year){
    if(year % 400 == 0){
        return 1;
    } else if(year % 100 == 0){
        return 0;
    } else if(year % 4 == 0){
        return 1;
    } else{
        return 0;
    }
}

date_t id_to_date(char id[]){
    date_t new_date;
    char buffer[3];
    substring(id, buffer,0,2);
    new_date.year = atoi(buffer);
    substring(id, buffer,2,2);
    new_date.month = atoi(buffer);
    substring(id, buffer,4,2);
    new_date.day = atoi(buffer);

    return new_date;
}

void date_to_id(date_t date, char output[]){
    char buffer[6];
    sprintf(buffer,"%02d%02d%02d",date.year,date.month,date.day);
    strcpy(output,buffer);
}

int get_weekday(struct tm *time){
    char buffer[1];
    strftime(buffer,sizeof(int),"%w",time);
    return atoi(buffer);
}

void substring(char src[],char dest[] ,int start,int len){
    char buffer[1000];
    int i = 0;
    for(;i < len;i++){
        buffer[i] = src[i + start];
    }
    buffer[i + 1] = '\0';
    strcpy(dest, buffer);

}

/***
 *
 * @param arr the array that is searched for value in
 * @param len the length of the array
 * @param value the value to check for in array
 * @return returns 1 if value is in arr; 0 if not
 */
int contained_in(int arr[], int len, int value){
    for(int i = 0;i < len;i++){
        if(arr[i] == value)
            return 1;
    }
    return 0;
}