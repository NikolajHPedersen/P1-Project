#include <stdlib.h>
#include <stdio.h>

struct patient{
    int patient_id;
    char first_name[32];
    char last_name[32];
    int age;
    char factors[][32];

};
//65521
void append_entry(char file_name[], char message[]);
void remove_entry(char file_name[], int line);

