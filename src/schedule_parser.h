#include "file_reader.h"
#include "schedule_lib.h"

#include <stdlib.h>
#include <stdio.h>

#define APPOINTMENTS_PER_DAY (60*WORKING_DAY/APPOINTMENT_DURATION)

void assign_appointment(patient_t patient,char file_name[]);

date_t assign_date(patient_t patient, char file_name[], date_t next_day);

void go_to_line(int line,FILE *fp,char *current_line);

date_t future_date(date_t date, int time_skip);

void assign_appointment_to_patient(char file_name[],patient_t patient, char block_id[]);

void string_helper(char appointment_id[],char cpr[],char dest[]);

int find_block_id(FILE *fp, long block_id);