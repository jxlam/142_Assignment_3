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

struct address
{
    int         virtual_addr;
    int         physical_addr;
};

struct process
{
    int                 process_id;
    struct address      page_table[20];
    STATUS              status;
};

struct page
{
    int         process_id;
    int         dirty;
    int         access;
};

// ------------- C O M M A N D   F U N C S --------------
void getCommandList(struct command command_list[], int *num_of_commands);
void initPageTable(struct page pages[]);
void initProcessList(struct process Process[]);

void freePage(struct page* Page);


// ------------- M E M O R Y   A L G O S --------------
void FIFO_MEM(struct command command_list[], int *num_of_commands);
void LRU_MEM(struct command command_list[], int *num_of_commands);
void RANDOM_MEM(struct command command_list[], int *num_of_commands);

// ------------- A C T I O N S --------------
int executeAction(struct command command, struct process process_list[], struct page physical_space[], int swap_space[], int* process_index, int* swap_index, int* physical_index);
int createProcess(struct process process_list[], int* process_index, int process_id);
void terminateProcess(struct command command, struct process process_list[], struct page physical_space[]);
int allocateProcess(struct command command, struct process process_list[], struct page physical_space[]);
void readProcess(struct command command, struct process process_list[], struct page physical_space[]);
void writeProcess(struct command command, struct process process_list[], struct page physical_space[]);
void freeProcess(struct command command, struct process process_list[], struct page physical_space[]);

// ------------- P R I N T   F U N C S--------------
void printCommand(struct command Command);
void printProcess(struct process Process);
void printProcessList(struct process Process[]);
void printPhysicalSpace(struct page memory_space[]);
void printPage(struct page Page);
void printProcessPageTable(struct address virtual_memory[]);
void printHeader(char *message);