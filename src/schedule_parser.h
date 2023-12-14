#include "file_reader.h"
#include "schedule_lib.h"

#include <stdlib.h>
#include <stdio.h>

#define APPOINTMENTS_PER_DAY (60*WORKING_DAY/APPOINTMENT_DURATION)

#define PATIENT_DB "test_db.txt"

void assign_appointments_new_patients(char new_patient_db[], char schedule_file[], date_t date);

void assign_appointment(patient_t patient, date_t date,char file_name[]);

date_t assign_date(patient_t patient, char file_name[], date_t next_day);

void set_range_end(int *range_end,char HWG);

void go_to_line(int line,FILE *fp,char *current_line);

date_t future_date(date_t date, int time_skip);

void assign_appointment_to_patient(char file_name[],patient_t patient, char block_id[]);

void string_helper(char appointment_id[],char cpr[],char dest[]);

int find_block_id(FILE *fp, long block_id);

int patient_cmp(const void * a, const void * b);

void sort_patients_by_hwg(patient_t *patients, int len);

patient_t serialize_patient(char *str);

int count_lines_in_file(FILE *fp);

patient_t *read_new_patients(char file_name[],int *new_patients);