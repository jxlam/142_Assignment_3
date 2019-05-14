#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ------------- E N U M S --------------
typedef enum
{
    READY = 0,
    KILLED = 1
} STATUS;


// ------------- S T R U C T S --------------
struct command
{
    int         command_id;
    char        action;
    int         page;
};

struct process
{
    int         process_id;
    int         virtual_addr;
    int         physical_addr;
    int         dirty; // should also have access bit (see if it's read but not written)
    int         access;
    STATUS      status;
};


// ------------- C O M M A N D   F U N C S --------------
void getCommandList(struct command command_list[], int *num_of_commands);


// ------------- M E M O R Y   A L G O S --------------
void FIFO_MEM(struct command command_list[], int *num_of_commands);
void LRU_MEM(struct command command_list[], int *num_of_commands);
void RANDOM_MEM(struct command command_list[], int *num_of_commands);

// ------------- A C T I O N S --------------
int executeAction(struct command command, struct process* process_list, int* physical_space, int* swap_space, int* process_index, int* swap_index, int* physical_index);
int createProcess(struct process* process_list, int* process_index, int process_id);
void terminateProcess();
void allocateProcess();
void readProcess();
void writeProcess();
void freeProcess();

// ------------- P R I N T   F U N C S--------------
void printCommand(struct command Command);
void printProcess(struct process Process);


int main(){
    struct command command_list[100];
    int num_of_commands;
  
    /* populate command list and get number of commands */
    getCommandList(command_list, &num_of_commands);

    for(int i = 0; i < num_of_commands; i++)
    {
            printCommand(command_list[i]);
    }

    /* REMEMBER TO CLEAN AFTER EACH ALGO */
    FIFO_MEM(command_list, &num_of_commands);
    LRU_MEM(command_list, &num_of_commands);
    RANDOM_MEM(command_list, &num_of_commands);
}



// FUNCTION: command command_list[]
//     - each command is read from file "memory.dat"
//     - values from line are stored in command struct
//     - each command is 1 index
//
//     EX)         memory.dat
//                     100 C  
//                     100 A 1
//
//     RESULT)     command_list[]
//
//                     command_list[0]
//                         command_id  = 100
//                         action      = 'C'
//                         page        = 0
//                     command_list[1]
//                         command_id  = 100
//                         action      = 'A'
//                         page        = 1
void getCommandList(struct command command_list[], int *num_of_commands)
{
    FILE *fp;

    /* OPEN FILE TO READ COMMANDS */
    printf("Opening memory.dat file.\n");
    fp = fopen("memory.dat", "r");
    if(fp == NULL)
    {
        perror("Error while opening file.\n");
    }

    char line[1000];
    int command_id, page, action;
    int n, index = 0;

    while(fgets(line, 1000, fp) != NULL)
    {
        n = sscanf(line, "%i %lc %i\n", &command_id, &action, &page);

        if(n > 0)
        {
            command_list[index].command_id = command_id;
            command_list[index].action = action;
            command_list[index].page = page;
            if(n == 2)
            {
                command_list[index].page = 0;
            }
        }
        index++;
    }
    fclose(fp);

    *num_of_commands = index;
}

void FIFO_MEM(struct command command_list[], int *num_of_commands)
{
    printf("\n\t\t\t\tFIFO MEM\n");
    printf("-----------------------------------------------------------------------------------\n");

    struct process process_list[100];
    int physical_space[20];
    int swap_space[1000];

    int process_index = 0;
    int physical_index = 0;
    int swap_index = 0;

    for(int i=0; i < *num_of_commands; i++)
    {
        // Execute command command
        printf("COMMAND #%i: ", i + 1);
        printCommand(command_list[i]);

        if(executeAction(command_list[i], process_list, physical_space, swap_space, &process_index, &swap_index, &physical_index) == 0)
        {
            printf("COMMAND FAILED\n");
        }
    }
}

void LRU_MEM(struct command command_list[], int *num_of_commands)
{
    printf("\n\t\t\t\tLRU MEM\n");
    printf("-----------------------------------------------------------------------------------\n");

}

void RANDOM_MEM(struct command command_list[], int *num_of_commands)
{
    printf("\n\t\t\t\tRANDOM MEM\n");
    printf("-----------------------------------------------------------------------------------\n");

}

int executeAction(struct command command, struct process* process_list, int* physical_space, int* swap_space, int* process_index, int* swap_index, int* physical_index)
{
    int result = 1;
    switch(command.action)
    {
        case 'C':   
            createProcess(process_list, process_index, command.command_id);
            printProcess(process_list[(*process_index) - 1]);
            break;
        case 'T':   
            terminateProcess();
            break;
        case 'A':   
            allocateProcess();
            break;
        case 'R':   
            readProcess();
            break;
        case 'W':   
            writeProcess();
            break;
        case 'F':   
            freeProcess();
            break;
        default:    
            result = 0;
            break;
    }
}

int createProcess(struct process* process_list, int* process_index, int process_id)
{
    printf("createProcess\n");
    struct process createdProcess;

    createdProcess.process_id       = process_id;
    createdProcess.virtual_addr     = -1;
    createdProcess.physical_addr    = -1;
    createdProcess.dirty            = 0;
    createdProcess.status           = READY;

    printProcess(createdProcess);

    process_list[(*process_index)++] = createdProcess;

    return 1;
}
void terminateProcess()
{
    printf("terminateProcess\n");
}
void allocateProcess()
{
    printf("allocateProcess\n");
}
void readProcess()
{
    printf("readProcess\n");
}
void writeProcess()
{
    printf("writeProcess\n");
}
void freeProcess()
{
    printf("freeProcess\n");
}

void printCommand(struct command Command)
{
      printf("command_id: %i action: %c page: %i\n", 
              Command.command_id, Command.action, Command.page);
}

void printProcess(struct process Process)
{
    printf("process_id: %i virtual_addr: %i physical_addr: %i dirty: %i status: %i\n", 
              Process.process_id, Process.virtual_addr, Process.physical_addr, Process.dirty, Process.status);
}