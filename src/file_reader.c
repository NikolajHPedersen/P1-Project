#include "file_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append_entry(char file_name[], char message[]){
    FILE *fp = fopen(file_name,"a");
    if(fp == NULL){
        printf("Something went wrong");
        exit(EXIT_FAILURE);
    }
    printf("And here!");
    fprintf(fp,"%s\n",message);
    fclose(fp);
}

void remove_entry(char file_name[], int line){

}
