
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct process
{
  int process_id;
  char action;
  int page;
  int virtualpage; //??
  int physicalpage; //??
};

void getMemoryList(struct process process_list[], int *num_of_actions);

int main(){
    struct process process_list[100];
    int num_of_actions;
  
    getMemoryList(process_list, &num_of_actions);

    for(int i = 0; i < num_of_actions; i++)
    {
          printf("PROCESS #%i: process_id: %i action: %c page: %i\n", 
              i + 1, process_list[i].process_id, process_list[i].action, process_list[i].page);
    }
}

void getMemoryList(struct process process_list[], int *num_of_actions)
{
    FILE *fp;

    printf("Opening memory.dat file.\n");
    fp = fopen("memory.dat", "r");
    if(fp == NULL)
    {
        perror("Error while opening file.\n");
        // exit(EXIT_FAILURE);
    }

    char line[1000];
    int process_id, page, action;
    int n, index = 0;

    while(fgets(line, 1000, fp) != NULL)
    {
        n = sscanf(line, "%i %lc %i\n", &process_id, &action, &page);
        if(n == 2)
        {
            process_list[index].process_id = process_id;
            process_list[index].action = action;
            process_list[index].page = 0;
        }
        else
        {
            process_list[index].process_id = process_id;
            process_list[index].action = action;
            process_list[index].page = page;
        }
        index++;
    }
    fclose(fp);

    *num_of_actions = index;
}
