//2-1
/*
#include <stdio.h>
#include <stdlib.h>

// Node structure for the process list
typedef struct ProcessNode {
    int processId;                   // Process ID
    char type;                       // 'F' for Foreground, 'B' for Background
    int promoted;                    // Promotion status
    struct ProcessNode* next;        // Pointer to the next process node
} ProcessNode;

// Node structure for the stack
typedef struct StackNode {
    struct ProcessNode* processList; // Pointer to the process list
    struct StackNode* next;          // Pointer to the next stack node
} StackNode;

// Function to create a new process node
ProcessNode* createProcessNode(int processId, char type) {
    ProcessNode* newNode = (ProcessNode*)malloc(sizeof(ProcessNode));
    if (!newNode) {
        fprintf(stderr, "Failed to allocate memory for process node.\n");
        exit(EXIT_FAILURE);
    }
    newNode->processId = processId;
    newNode->type = type;
    newNode->promoted = 0;
    newNode->next = NULL;
    return newNode;
}

// Function to create a new stack node
StackNode* createStackNode() {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) {
        fprintf(stderr, "Failed to allocate memory for stack node.\n");
        exit(EXIT_FAILURE);
    }
    newNode->processList = NULL;
    newNode->next = NULL;
    return newNode;
}

// Function to push a new stack node
void push(StackNode** top) {
    StackNode* newNode = createStackNode();
    newNode->next = *top;
    *top = newNode;
}

// Function to pop a stack node
void pop(StackNode** top) {
    if (*top == NULL) {
        fprintf(stderr, "Stack is empty!\n");
        return;
    }
    StackNode* temp = *top;
    *top = (*top)->next;
    free(temp);
}

// Function to add a process to a process list
void addProcess(ProcessNode** head, int processId, char type) {
    ProcessNode* newNode = createProcessNode(processId, type);
    newNode->next = *head;
    *head = newNode;
}

// Function to remove a process from a process list
void removeProcess(ProcessNode** head, int processId) {
    if (*head == NULL) return;

    ProcessNode* temp = *head;
    ProcessNode* prev = NULL;

    if (temp != NULL && temp->processId == processId) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->processId != processId) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;
    free(temp);
}

// Function to enqueue a process
void enqueue(StackNode** stackTop, int processId, char type) {
    if (*stackTop == NULL) {
        push(stackTop);
    }

    if (type == 'F') {
        // Insert at the end of the list
        StackNode* current = *stackTop;
        while (current->next != NULL) {
            current = current->next;
        }
        addProcess(&current->processList, processId, type);
    }
    else {
        // Insert at the beginning of the list
        addProcess(&(*stackTop)->processList, processId, type);
    }
}

// Function to dequeue a process
void dequeue(StackNode** stackTop) {
    if (*stackTop == NULL || (*stackTop)->processList == NULL) {
        fprintf(stderr, "Queue is empty!\n");
        return;
    }

    ProcessNode* temp = (*stackTop)->processList;
    (*stackTop)->processList = temp->next;
    free(temp);

    if ((*stackTop)->processList == NULL) {
        pop(stackTop);
    }
}

// Function to promote processes to avoid starvation
void promote(StackNode** stackTop, StackNode** promotePtr) {
    if (*promotePtr == NULL || (*promotePtr)->processList == NULL) {
        return;
    }

    // Move the head of the promote list to the tail
    ProcessNode* process = (*promotePtr)->processList;
    (*promotePtr)->processList = process->next;
    process->promoted = 1;

    if (*promotePtr == *stackTop) {
        // If promoting from the top stack node, just remove and push to the end
        if ((*stackTop)->next == NULL) {
            push(stackTop);
        }
        addProcess(&(*stackTop)->next->processList, process->processId, process->type);
    }
    else {
        // Move the process to the next stack node in a circular manner
        if ((*promotePtr)->next == NULL) {
            push(promotePtr);
        }
        addProcess(&(*promotePtr)->next->processList, process->processId, process->type);
    }

    // Update the promote pointer
    *promotePtr = (*promotePtr)->next ? (*promotePtr)->next : *stackTop;
}

// Function to split and merge processes
void split_n_merge(StackNode** stackTop, int threshold) {
    StackNode* current = *stackTop;

    while (current != NULL) {
        ProcessNode* process = current->processList;
        int length = 0;
        while (process != NULL) {
            length++;
            process = process->next;
        }

        if (length > threshold) {
            StackNode* newStackNode = createStackNode();
            int mid = length / 2;
            process = current->processList;
            ProcessNode* prev = NULL;

            for (int i = 0; i < mid; i++) {
                prev = process;
                process = process->next;
            }

            if (prev != NULL) {
                prev->next = NULL;
            }

            newStackNode->processList = process;
            newStackNode->next = current->next;
            current->next = newStackNode;
        }

        current = current->next;
    }
}

// Function to print the dynamic queue
void printQueue(StackNode* stackTop) {
    StackNode* current = stackTop;
    int stackIndex = 0;

    while (current != NULL) {
        ProcessNode* process = current->processList;
        printf("Stack %d: ", stackIndex);

        while (process != NULL) {
            printf("%d%c", process->processId, process->type);
            if (process->promoted) {
                printf("*");
            }
            if (process->next != NULL) {
                printf(" -> ");
            }
            process = process->next;
        }
        printf("\n");
        current = current->next;
        stackIndex++;
    }
}

int main() {
    StackNode* stackTop = NULL;
    StackNode* promotePtr = NULL;
    int threshold = 3;

    // Enqueue processes
    enqueue(&stackTop, 0, 'F'); // Foreground process
    enqueue(&stackTop, 1, 'B'); // Background process
    enqueue(&stackTop, 2, 'F'); // Foreground process
    enqueue(&stackTop, 3, 'B'); // Background process

    // Set the promote pointer to the top of the stack initially
    promotePtr = stackTop;

    // Promote processes
    promote(&stackTop, &promotePtr);

    // Split and merge processes
    split_n_merge(&stackTop, threshold);

    // Print the queue
    printQueue(stackTop);

    // Dequeue processes
    dequeue(&stackTop);
    dequeue(&stackTop);
    dequeue(&stackTop);
    dequeue(&stackTop);

    return 0;
}
*/

//2-2
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>

#define MAX_PROCESSES 100
#define MAX_CMD_LENGTH 256

typedef struct {
    int pid;
    char type; // 'F' for FG, 'B' for BG
    int promotion; // 0: no promotion, 1: promoted
    int remaining_time;
} Process;

Process DQ[MAX_PROCESSES];
Process WQ[MAX_PROCESSES];
int dq_size = 0;
int wq_size = 0;

int pid_counter = 0;

void add_process_to_DQ(int pid, char type, int promotion) {
    DQ[dq_size].pid = pid;
    DQ[dq_size].type = type;
    DQ[dq_size].promotion = promotion;
    DQ[dq_size].remaining_time = 0;
    dq_size++;
}

void add_process_to_WQ(int pid, char type, int remaining_time) {
    WQ[wq_size].pid = pid;
    WQ[wq_size].type = type;
    WQ[wq_size].promotion = 0;
    WQ[wq_size].remaining_time = remaining_time;
    wq_size++;
}

void print_queues() {
    printf("Running: [1B]\n");
    printf("--------------------------\n");
    printf("DQ: ");
    for (int i = 0; i < dq_size; i++) {
        printf("[%d%c", DQ[i].pid, DQ[i].type);
        if (DQ[i].promotion) printf("*");
        printf("] ");
    }
    printf("(bottom/top)\n");
    printf("--------------------------\n");
    printf("WQ: ");
    for (int i = 0; i < wq_size; i++) {
        printf("[%d%c:%d] ", WQ[i].pid, WQ[i].type, WQ[i].remaining_time);
    }
    printf("\n");
    printf("--------------------------\n");
}

char** parse(const char* command) {
    char** args = malloc(MAX_CMD_LENGTH * sizeof(char*));
    char* cmd_copy = _strdup(command);
    char* context = NULL;
    char* token = strtok_s(cmd_copy, " ", &context);
    int index = 0;

    while (token != NULL) {
        args[index++] = _strdup(token);
        token = strtok_s(NULL, " ", &context);
    }
    args[index] = NULL;
    free(cmd_copy);
    return args;
}

void exec_cmd(char** args) {
    if (_spawnvp(_P_WAIT, args[0], args) == -1) {
        perror("exec failed");
    }
    for (int i = 0; args[i] != NULL; i++) {
        free(args[i]);
    }
    free(args);
}

void shell_process(int sleep_time) {
    char command[MAX_CMD_LENGTH];

    while (1) {
        printf("shell> ");
        fgets(command, MAX_CMD_LENGTH, stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) > 0) {
            char** args = parse(command);
            exec_cmd(args);
        }

        Sleep(sleep_time * 1000); // Windows에서는 Sleep 함수가 밀리초 단위로 동작
    }
}

unsigned __stdcall monitor_thread(void* interval) {
    int sleep_interval = *(int*)interval;
    while (1) {
        print_queues();
        Sleep(sleep_interval * 1000);
    }
    return 0;
}

int main() {
    int shell_sleep_time = 5; // Y 초
    int monitor_interval = 3; // X 초

    // Create shell process
    _beginthread((void(__cdecl*)(void*))shell_process, 0, &shell_sleep_time);

    // Create monitor thread
    HANDLE monitor_handle;
    unsigned monitor_thread_id;
    monitor_handle = (HANDLE)_beginthreadex(NULL, 0, monitor_thread, &monitor_interval, 0, &monitor_thread_id);

    // Main thread waits for the monitor thread to complete
    WaitForSingleObject(monitor_handle, INFINITE);

    return 0;
}
*/

//2-3
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <fcntl.h>
#include <io.h>

#define BUFFER_SIZE 1024

void execute_command(char* command);

// 쓰레드 함수 정의
unsigned __stdcall thread_function(void* arg) {
    char* command = (char*)arg;
    execute_command(command);
    _endthreadex(0);
    return 0;
}

// 명령어 실행 함수 정의
void execute_command(char* command) {
    char* args[BUFFER_SIZE];
    char* token;
    int arg_index = 0;
    char* context = NULL;

    // 공백을 기준으로 명령어와 인자 분리
    token = strtok_s(command, " ", &context);
    while (token != NULL) {
        args[arg_index++] = token;
        token = strtok_s(NULL, " ", &context);
    }
    args[arg_index] = NULL;

    if (args[0] == NULL) {
        return;
    }

    if (strcmp(args[0], "echo") == 0) {
        // echo 명령어 처리
        for (int i = 1; args[i] != NULL; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
    }
    else if (strcmp(args[0], "dummy") == 0) {
        // dummy 명령어 처리
        Sleep(1000);
    }
    else if (strcmp(args[0], "gcd") == 0) {
        // gcd 명령어 처리
        if (args[1] != NULL && args[2] != NULL) {
            int x = atoi(args[1]);
            int y = atoi(args[2]);
            while (y != 0) {
                int temp = y;
                y = x % y;
                x = temp;
            }
            printf("GCD is %d\n", x);
        }
    }
    else if (strcmp(args[0], "prime") == 0) {
        // prime 명령어 처리
        if (args[1] != NULL) {
            int x = atoi(args[1]);
            int count = 0;
            for (int i = 2; i <= x; i++) {
                int is_prime = 1;
                for (int j = 2; j * j <= i; j++) {
                    if (i % j == 0) {
                        is_prime = 0;
                        break;
                    }
                }
                if (is_prime) count++;
            }
            printf("Prime count is %d\n", count);
        }
    }
    else if (strcmp(args[0], "sum") == 0) {
        // sum 명령어 처리
        if (args[1] != NULL) {
            int x = atoi(args[1]);
            long long total = 0;
            for (int i = 1; i <= x; i++) {
                total = (total + i) % 1000000;
            }
            printf("Sum is %lld\n", total);
        }
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", args[0]);
    }
}

int main() {
    // UTF-8 인코딩을 사용하도록 콘솔 설정
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);

    FILE* file;
    wchar_t buffer[BUFFER_SIZE];

    // commands.txt 파일을 UTF-8로 열기
    errno_t err = _wfopen_s(&file, L"commands.txt", L"r, ccs=UTF-8");
    if (err != 0) {
        fwprintf(stderr, L"Failed to open file\n");
        return EXIT_FAILURE;
    }

    // 파일의 각 줄을 읽어 명령어 실행
    while (fgetws(buffer, BUFFER_SIZE, file) != NULL) {
        // 줄바꿈 문자 제거
        size_t len = wcslen(buffer);
        if (len > 0 && buffer[len - 1] == L'\n') {
            buffer[len - 1] = L'\0';
        }

        // 명령어를 ';'로 분리하여 순차적으로 실행
        wchar_t* context = NULL;
        wchar_t* command = wcstok_s(buffer, L";", &context);
        while (command != NULL) {
            wprintf(L"prompt> %ls\n", command);  // 명령어 출력
            // 각 명령어를 '&'로 분리하여 백그라운드 실행 처리
            wchar_t* sub_context = NULL;
            wchar_t* sub_command = wcstok_s(command, L"&", &sub_context);
            while (sub_command != NULL) {
                wchar_t trimmed_command[BUFFER_SIZE];
                wcscpy_s(trimmed_command, sizeof(trimmed_command) / sizeof(trimmed_command[0]), sub_command);
                trimmed_command[wcslen(trimmed_command)] = 0;

                if (wcschr(sub_command, L'&') != NULL) {
                    // 백그라운드 실행
                    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, thread_function, (void*)sub_command, 0, NULL);
                    if (hThread == NULL) {
                        fwprintf(stderr, L"CreateThread error: %d\n", GetLastError());
                        return EXIT_FAILURE;
                    }
                    else {
                        wprintf(L"Running: [1B]--------------------------\n");
                    }
                    CloseHandle(hThread);
                }
                else {
                    // 포그라운드 실행
                    char command_mb[BUFFER_SIZE];
                    size_t converted_chars = 0;
                    wcstombs_s(&converted_chars, command_mb, BUFFER_SIZE, sub_command, _TRUNCATE);
                    execute_command(command_mb);
                }
                sub_command = wcstok_s(NULL, L"&", &sub_context);
            }
            command = wcstok_s(NULL, L";", &context);
        }
    }

    fclose(file);
    return 0;
}
*/