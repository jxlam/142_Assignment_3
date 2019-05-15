#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "swap.h"

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
    // LRU_MEM(command_list, &num_of_commands);
    // RANDOM_MEM(command_list, &num_of_commands);
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


void initPageTable(struct page pages[])
{
    for(int i = 0; i < 20; i++)
    {
        pages[i].process_id = -1;
        int dirty = 0;
        int access = 0;
    }
}

void initSwapSpace(struct page pages[])
{
    for(int i = 0; i < 1000; i++)
    {
        pages[i].process_id = -1;
        int dirty = 0;
        int access = 0;
    }
}


void initProcessList(struct process Process[])
{
    for(int i = 0; i < 100; i++)
    {
        Process[i].process_id       = -1;
        Process[i].status           = READY;

        for(int j = 0; j < 20; j++)
        {
            Process[i].page_table[j].virtual_addr     = -1;
            Process[i].page_table[j].physical_addr    = -1;
        }
    }
}

void movePageIntoSwap(int swap_index, struct page swap_space[], struct process Process[], struct page physical_space[])
{
    printf("MOVING %i INTO SWAP.\n", swap_index);
    struct page pageToSwap;
    int new_swap_index;

    pageToSwap = physical_space[swap_index];

    for(int i = 0; i < 1000; i++)
    {
        if(swap_space[i].process_id == -1)
        {
            swap_space[i] = pageToSwap;
            new_swap_index = i;
            break;
        }
    }

    for(int i = 0; i < 100; i++)
    {
        if(physical_space[swap_index].process_id == Process[i].process_id)
        {
            for(int j = 0; j < 20; j++)
            {
                if(swap_index == Process[i].page_table[j].physical_addr)
                {
                    Process[i].page_table[j].swap_index = new_swap_index;
                }
            }
        }
    }

    //freePage(&(physical_space[swap_index]));
    physical_space[swap_index].process_id = -1;
    physical_space[swap_index].dirty = 0;
    physical_space[swap_index].access = 0;
}




// ------------- M E M O R Y / S W A P   A L G O S --------------

void FIFO_MEM(struct command command_list[], int *num_of_commands)
{
    printHeader("FIFO_MEM");

    struct process process_list[100];
    struct page swap_space[1000];
    struct page physical_memory[20];

    int process_index = 0;
    int physical_index = 0;
    int swap_index = 0;

    int global_timer = 0;
    int page_to_swap = 0;

    initPageTable(physical_memory);
    initProcessList(process_list);
    initSwapSpace(swap_space);

    for(int i=0; i < *num_of_commands; i++)
    {
        // Execute command command
        printf("COMMAND #%i: ", i + 1);
        printCommand(command_list[i]);

        if(executeAction(command_list[i], process_list, physical_memory, swap_space, &process_index, &swap_index, &physical_index) == 0)
        {
            int oldest_timer = physical_memory[0].timer;
            int oldest_index = 0;
            for(int i = 0; i < 20; i++)
            {
                if(physical_memory[i].process_id != -1 & physical_memory[i].timer < oldest_timer)
                {
                    oldest_timer = physical_memory[i].timer;
                    oldest_index = i;
                }
            }
            movePageIntoSwap(oldest_index, swap_space, process_list, physical_memory);
            printPhysicalSpace(physical_memory);
            printSwapSpace(swap_space);
            printProcessList(process_list);

            executeAction(command_list[i], process_list, physical_memory, swap_space, &process_index, &swap_index, &physical_index);
            printPhysicalSpace(physical_memory);
            printSwapSpace(swap_space);
            printProcessList(process_list);
        }

        if(command_list[i].action == 'A')
        {
            printf("allocating setting timer\n");
            for(int k = 0; k < 100; k++)
            {
                if(process_list[k].process_id == command_list[i].command_id)
                {
                    for(int j = 0; j < 20; j++)
                    {
                        if(process_list[k].page_table[j].virtual_addr == command_list[i].page)
                        {
                            printf("FINALLY setting timer\n");
                            int index = process_list[k].page_table[j].physical_addr;
                            physical_memory[index].timer = global_timer++;
                        }
                    }
                }
            }

            printProcessList(process_list);
        }
    }

    printf("\n\nFINAL PROCESS LIST\n");
    printProcessList(process_list);

    printf("\n\nFINAL PHYSICAL PAGE\n");
    printPhysicalSpace(physical_memory);
}

void LRU_MEM(struct command command_list[], int *num_of_commands)
{
    printHeader("LRU_MEM");

    struct process process_list[100];
    struct page swap_space[1000];
    struct page physical_memory[20];

    int process_index = 0;
    int physical_index = 0;
    int swap_index = 0;

    int global_timer = 0;
    int page_to_swap = 0;

    int frequency = 0;

    initPageTable(physical_memory);
    initProcessList(process_list);
    initSwapSpace(swap_space);

    for(int i=0; i < *num_of_commands; i++)
    {
        // Execute command command
        printf("COMMAND #%i: ", i + 1);
        printCommand(command_list[i]);

        if(executeAction(command_list[i], process_list, physical_memory, swap_space, &process_index, &swap_index, &physical_index) == 0)
        {
            int oldest_timer = physical_memory[0].timer;
            int oldest_index = 0;
            for(int i = 0; i < 20; i++)
            {
                if(physical_memory[i].process_id != -1 & physical_memory[i].timer < oldest_timer)
                {
                    oldest_timer = physical_memory[i].timer;
                    oldest_index = i;
                }
            }
            movePageIntoSwap(oldest_index, swap_space, process_list, physical_memory);
            printPhysicalSpace(physical_memory);
            printSwapSpace(swap_space);
            printProcessList(process_list);

            executeAction(command_list[i], process_list, physical_memory, swap_space, &process_index, &swap_index, &physical_index);
            printPhysicalSpace(physical_memory);
            printSwapSpace(swap_space);
            printProcessList(process_list);
        }

        if(command_list[i].action == 'C')
        {
            for(int k = 0; k < 100; k++)
            {
                if(process_list[k].process_id == command_list[i].command_id)
                {
                    for(int j = 0; j < 20; j++)
                    {
                        if(process_list[k].page_table[j].virtual_addr == command_list[i].page)
                        {
                            int index = process_list[k].page_table[j].physical_addr;
                            physical_memory[index].frequency = 0;
                        }
                    }
                }
            }

            printProcessList(process_list);
        }
    }

    printf("\n\nFINAL PROCESS LIST\n");
    printProcessList(process_list);

    printf("\n\nFINAL PHYSICAL PAGE\n");
    printPhysicalSpace(physical_memory);
}

void RANDOM_MEM(struct command command_list[], int *num_of_commands)
{
    printHeader("RANDOM_MEM");

    struct process process_list[100];
    struct page swap_space[1000];
    struct page physical_memory[20];

    int process_index = 0;
    int physical_index = 0;
    int swap_index = 0;

    initPageTable(physical_memory);
    initProcessList(process_list);
    initSwapSpace(swap_space);

    for(int i=0; i < *num_of_commands; i++)
    {
        // Execute command command
        printf("COMMAND #%i: ", i + 1);
        printCommand(command_list[i]);

        if(executeAction(command_list[i], process_list, physical_memory, swap_space, &process_index, &swap_index, &physical_index) == 0)
        {
            int random = rand() % 20;
            printf("SWAP: %i\n", random);
            movePageIntoSwap(random, swap_space, process_list, physical_memory);
            printPhysicalSpace(physical_memory);
            printSwapSpace(swap_space);
            printProcessList(process_list);

            printf("POST SWAP\n");
            executeAction(command_list[i], process_list, physical_memory, swap_space, &process_index, &swap_index, &physical_index);
            printPhysicalSpace(physical_memory);
            printSwapSpace(swap_space);
            printProcessList(process_list);
        }
    }
}





// ------------- A C T I O N S --------------

int executeAction(struct command command, struct process process_list[], struct page physical_space[], struct page swap_space[], int* process_index, int* swap_index, int* physical_index)
{
    int result = 1;
    switch(command.action)
    {
        case 'C':   
            createProcess(process_list, process_index, command.command_id);
            // printProcess(process_list[(*process_index) - 1]);
            printProcessList(process_list);
            break;
        case 'T':   
            terminateProcess(command, process_list, physical_space);
            printPhysicalSpace(physical_space);
            break;
        case 'A':   
            result = allocateProcess(command, process_list, physical_space);
            printPhysicalSpace(physical_space);
            printProcessList(process_list);
            break;
        case 'R':   
            readProcess(command, process_list, physical_space);
            printPhysicalSpace(physical_space);
            break;
        case 'W':   
            writeProcess(command, process_list, physical_space);
            printPhysicalSpace(physical_space);
            break;
        case 'F':   
            freeProcess(command, process_list, physical_space);
            break;
        default:    
            result = 0;
            break;
    }
    return result;
}

int createProcess(struct process process_list[], int* process_index, int process_id)
{
    printf("CREATE PROCESS\n");
    struct process createdProcess;

    createdProcess.process_id       = process_id;
    createdProcess.status           = READY;
    for(int i = 0; i < 20; i++)                         // Initializes page table
    {
        createdProcess.page_table[i].virtual_addr   = -1;
        createdProcess.page_table[i].physical_addr  = -1;
        createdProcess.page_table[i].swap_index     = -1;
    }

    process_list[(*process_index)++] = createdProcess;

    return 1;
}

void terminateProcess(struct command command, struct process process_list[], struct page physical_space[])
{
    printf("TERMINATE PROCESS\n");
    freeProcess(command, process_list, physical_space);
    for(int i = 0; i < 100; i++)
    {
        if(process_list[i].process_id == command.command_id)
        {
            process_list[i].status = KILLED;
        }
    }
}

int allocateProcess(struct command command, struct process process_list[], struct page physical_space[])
{
    int result = 1;
    int physical_index = -1;
    printf("ALLOCATE PROCESS\n");
    for(int i = 0; i < 20; i++)
    {
        if(physical_space[i].process_id == -1)
        {
            physical_index = i;
            physical_space[i].process_id = command.command_id;
            break;
        }
    }

    if(physical_index >= 0)
    {
        for(int i = 0; i < 100; i++)
        {
            if(command.command_id == process_list[i].process_id)
            {
                for(int j = 0; j < 20; j++)
                {
                    if(process_list[i].page_table[j].virtual_addr == -1)
                    {
                        // printf("storing VA and PA: process %i, page_table %i, VA %i, PA %i\n", 
                        //         process_list[i].process_id, j, command.page, physical_index);
                        process_list[i].page_table[j].virtual_addr  = command.page;
                        process_list[i].page_table[j].physical_addr = physical_index;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        result = 0;
    }
    return result;
}
void readProcess(struct command command, struct process process_list[], struct page physical_space[])
{
    int physical_index;
    printf("READ PROCESS\n");
    for(int i = 0; i < 100; i++)
    {
        if(process_list[i].process_id == command.command_id)
        {
            for(int j = 0; j < 20; j++)
            {
                if(process_list[i].page_table[j].virtual_addr == command.page)
                {
                    physical_index = process_list[i].page_table[j].physical_addr;
                    physical_space[physical_index].access = 1;
                }
            }
        }
    }
}

void writeProcess(struct command command, struct process process_list[], struct page physical_space[])
{
    int physical_index;
    printf("WRITE PROCESS\n");

    for(int i = 0; i < 100; i++)
    {
        if(process_list[i].process_id == command.command_id)
        {
            for(int j = 0; j < 20; j++)
            {
                if(process_list[i].page_table[j].virtual_addr == command.page)
                {
                    physical_index = process_list[i].page_table[j].physical_addr;
                    physical_space[physical_index].dirty = 1;
                }
            }
        }
    }
}
void freeProcess(struct command command, struct process process_list[], struct page physical_space[])
{
    int physical_index;
    printf("FREE PROCESS\n");
    for(int i = 0; i < 100; i++)
    {
        if(process_list[i].process_id == command.command_id)
        {
            for(int j = 0; j < 20; j++)
            {
                if(process_list[i].page_table[j].virtual_addr != -1)
                {
                    physical_index = process_list[i].page_table[j].physical_addr;
                    freePage(&physical_space[physical_index]);           // frees physical page
                    process_list[i].page_table[j].virtual_addr  = -1;       // clears process page table
                    process_list[i].page_table[j].physical_addr = -1;
                }
            }
        }
    }
}


void freePage(struct page* Page)
{
    (*Page).process_id     = -1;
    (*Page).dirty          = 0;
    (*Page).access         = 0;
}





// ------------- P R I N T S --------------


void printCommand(struct command Command)
{
      printf("command_id: %i action: %c page: %i\n", 
              Command.command_id, Command.action, Command.page);
}

void printProcess(struct process Process)
{
    printf("process_id: %i status: %i\n", 
              Process.process_id, Process.status);

    printProcessPageTable(Process.page_table);
}

void printPage(struct page Page)
{
    printf("process_id: %i dirty: %i access: %i timer: %i\n", 
              Page.process_id, Page.dirty, Page.access, Page.timer);
}

void printProcessList(struct process Process[])
{
    printf("\nProcess List\n");
    for(int i = 0; i < 100; i++)
    {
        if(Process[i].process_id != -1)
        {
            printProcess(Process[i]);
        }
    }
    printf("\n");
}

void printPhysicalSpace(struct page memory_space[])
{
    printf("\nPhysical Space\n");
    for(int i = 0; i < 20; i++)
    {
        if(memory_space[i].process_id != -1)
        {
            printf("%i. ", i);
            printPage(memory_space[i]);
        }
        else
        {
            printf("INDEX %i IS FREE\n", i);
        }
    }
    printf("\n");
}

void printProcessPageTable(struct address virtual_memory[])
{
    for(int i = 0; i < 20; i++)
    {
        if(virtual_memory[i].virtual_addr != -1)
        {
            printf("virtual_addr: %i physical_addr: %i\n", 
                    virtual_memory[i].virtual_addr, virtual_memory[i].physical_addr);
        }
    }
    printf("\n");
}

void printSwapSpace(struct page swap_space[])
{
    printf("\nSwap Space\n");
    for(int i = 0; i < 1000; i++)
    {
        if(swap_space[i].process_id != -1)
        {
            printPage(swap_space[i]);
        }
    }
}

void printHeader(char *message)
{
    printf("\n\t\t\t\t%s\n", message);
    printf("-----------------------------------------------------------------------------------\n");
}