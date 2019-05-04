#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef enum
{
    READY,
    KILLED
} STATUS;

struct process
{
    int         process_id;
    char        action;
    int         page;
    int         virtual_addr;
    int         physical_addr;
    int         dirty;
    STATUS      status;
};

void getMemoryList(struct process process_list[], int *num_of_actions);

void FIFO_MEM(struct process process_list[], int *num_of_actions);
void LRU_MEM(struct process process_list[], int *num_of_actions);
void RANDOM_MEM(struct process process_list[], int *num_of_actions);

int chooseAction(char action);
void createPage();
void terminatePage();
void allocatePage();
void readPage();
void writePage();
void freePage();


int main(){
    struct process process_list[100];
    int num_of_actions;
  
    getMemoryList(process_list, &num_of_actions);

    // for(int i = 0; i < num_of_actions; i++)
    // {
    //       printf("PROCESS #%i: process_id: %i action: %c page: %i\n", 
    //           i + 1, process_list[i].process_id, process_list[i].action, process_list[i].page);
    // }
    FIFO_MEM(process_list, &num_of_actions);
}

void getMemoryList(struct process process_list[], int *num_of_actions)
{
    FILE *fp;

    printf("Opening memory.dat file.\n");
    fp = fopen("memory.dat", "r");
    if(fp == NULL)
    {
        perror("Error while opening file.\n");
    }

    char line[1000];
    int process_id, page, action;
    int n, index = 0;

    while(fgets(line, 1000, fp) != NULL)
    {
        n = sscanf(line, "%i %lc %i\n", &process_id, &action, &page);

        if(n > 0)
        {
            process_list[index].process_id = process_id;
            process_list[index].action = action;
            process_list[index].page = page;
            process_list[index].virtual_addr = -1;
            process_list[index].physical_addr = -1;
            process_list[index].dirty = 0;
            process_list[index].status = READY;
            if(n == 2)
            {
                process_list[index].page = 0;
            }
        }
        index++;
    }
    fclose(fp);

    *num_of_actions = index;
}

void FIFO_MEM(struct process process_list[], int *num_of_actions)
{
    int physical_space[20];
    int swap_space[1000];

    int physical_index;
    int swap_index;

    for(int i=0; i < *num_of_actions; i++)
    {
        // Execute process command
        printf("PROCESS #%i: process_id: %i action: %c page: %i\n", 
              i + 1, process_list[i].process_id, process_list[i].action, process_list[i].page);
        if(chooseAction(process_list[i].action) == 0)
        {
            printf("PROCESS COMMAND FAILED\n");
        }
    }
}

int chooseAction(char action)
{
    int result = 1;
    switch(action)
    {
        case 'C':   
            createPage();
            break;
        case 'T':   
            terminatePage();
            break;
        case 'A':   
            allocatePage();
            break;
        case 'R':   
            readPage();
            break;
        case 'W':   
            writePage();
            break;
        case 'F':   
            freePage();
            break;
        default:    
            result = 0;
            break;
    }
}

void createPage()
{
    printf("createPage\n");
}
void terminatePage()
{
    printf("terminatePage\n");
}
void allocatePage()
{
    printf("allocatePage\n");
}
void readPage()
{
    printf("readPage\n");
}
void writePage()
{
    printf("writePage\n");
}
void freePage()
{
    printf("freePage\n");
}