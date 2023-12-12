#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_reader.h"

#define APPOINTMENT_DURATION 30
#define DAYS_IN_SCHEDULE 60
#define WORKING_DAY 8
#define START_TIME 8

struct patient_t{
    unsigned int patient_id;
    char first_name[32];
    char last_name[32];
    int age;
    char HWG;
    unsigned int appointments;
};
typedef struct patient_t patient_t;

struct date_t{
    int year;
    int month;
    int day;
    int weekday;
};
typedef struct date_t date_t;

enum weekday_e {sunday, monday, tuesday, wednesday, thursday, friday, saturday};
typedef enum weekday_e weekday_e;

void create_empty_schedule(char file_name[]);

date_t add_day(date_t date);

void append_line_schedule(FILE *fp,char *message);

void get_date_id(struct tm *date, char *str);

date_t id_to_date(char id[]);

void date_to_id(date_t date, char output[]);

//void add_day(date_t *date);

int get_weekday(struct tm *time);

void substring(char const src[],char dest[] ,int start,int len);

int contained_in(int arr[], int len, int value);

int is_leap_year(int year);

void add_block(FILE *fp, char *id);