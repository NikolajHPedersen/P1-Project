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

    fprintf(fp,"%s\n",message);
    fclose(fp);
}

void insert_entry(char file_name[], char message[], int line_number){
    FILE *fp = fopen(file_name,"r");
    FILE *temp = fopen("temp.txt","w");

    char current_line[100];
    for(int i = 0;i < line_number;i++){
        fgets(current_line,100,fp);
        fputs(current_line,temp);
    }
    fprintf(temp,"%s\n",message);
    do{

        fgets(current_line,100,fp);
        if(feof(fp))
            break;
        fputs(current_line,temp);


    }while(!feof(fp));
    fclose(fp);
    fclose(temp);

}

void remove_entry(char file_name[], int line){

}
