#include "file_reader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int patient_creation(unsigned int const *CPR);

//Helper function - Can print a string and accept one input
void input(char *format_string,void *ptr, char *printed_string);
