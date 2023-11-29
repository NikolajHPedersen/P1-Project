#include <time.h>
#include <string.h>

#include "file_reader.h"

struct patient{
    int patient_id;
    char first_name[32];
    char last_name[32];
    int age;
    char factors[][32];
};

struct date{
    int year;
    int month;
    int day;
};
typedef struct date date;

struct appointment_t{
    long id;
    long patient_cpr;
    int duration_min;
    char description[];
};
typedef struct appointment_t appointment_t;

struct block_t{
    int block_size;
    appointment_t appointments[];
};
typedef struct block_t block_t;

char *get_date_id(struct tm *date);


void create_schedule(char file_name[]);