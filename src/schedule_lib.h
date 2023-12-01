#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_reader.h"

#define APPOINTMENT_DURARTION 30
#define DAYS_IN_SCHEDULE 30
#define WORKING_DAY 8

struct patient{
    unsigned int patient_id;
    char first_name[32];
    char last_name[32];
    int age;
    char factors[][32];
};

struct date_t{
    int year;
    int month;
    int day;
    int weekday;
};
typedef struct date_t date_t;

enum weekday_e {sunday, monday, tuesday, wednesday, thursday, friday, saturday};
typedef enum weekday_e weekday_e;

struct appointment_t{
    long id;
    long patient_cpr;
    int duration_min;
    char description[];
};
typedef struct appointment_t appointment_t;

struct block_t{
    int block_id;
    int block_size;

};
typedef struct block_t block_t;

struct subblock_t{
    unsigned int id;
    appointment_t appointments[];
};
typedef struct subblock_t subblock_t;

void create_empty_schedule(char file_name[]);

void add_day(date_t *date);

char *get_date_id(struct tm *date);

date_t id_to_date(char id[]);

void date_to_id(date_t date, char output[]);

int get_weekday(struct tm *time);

char *substring(char str[],int start,int end);

int contained_in(int arr[], int len, int value);

int is_leap_year(int year);

void add_block(char file_name[],char *id);