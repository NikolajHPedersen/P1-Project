#include <stdlib.h>
#include <stdio.h>

struct patient{
    int patient_id;
    char first_name[32];
    char last_name[32];
    int age;
    char factors[][32];

};
//
void append_entry(char file_name[], char message[]);
void insert_entry(char file_name[], char message[], int line_number);
void remove_entry(char file_name[], int line);



