#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//Michael Schall
// 09/18/2022

//stack struct and methods obtained from techiedelight.com
struct stack
{
    int maxsize;    // define max capacity of the stack
    int top;
    int *items;
};
struct stack* newStack(int capacity);
void push(struct stack *pt, int x);
int pop(struct stack *pt);
int peek(struct stack *pt);
int isEmpty(struct stack *pt);
int sizeStack(struct stack *pt);
int isFull(struct stack *pt);
char* itoa(int value, char* buffer, int base);
char* reverse(char *buffer, int i, int j);
void swap(char *x, char *y);

int main(int argc, char *argv[])
{
    FILE *input;
    FILE *output;
    int parentID = getpid();
    int childID;
    char mypid[6];
    char str[100];
    char string[100];
    char f;
    int c, forks, size, status, pid, max;
    struct stack *outputStack;
    struct stack *idStack;
    int noOption = 0;

    while((c = getopt(argc, argv, "hi::o::")) !=-1)
    {
        switch (c)
        {
        case 'h':
            printf("Option 'h' will bring up this help menu. Option 'i' signals the inclusion of an input file, and option 'o' the inclusion of an output file");
            break;
        //output
        case 'o':
            noOption = 1;
            if(optarg == NULL && optind < argc && argv[optind][0] != '-')
            {
                optarg = argv[optind++];
            }
            if(optarg == NULL)
            {
                output = fopen("output.dat", "w");
                if(!output)
                {
                    perror("Error. File does not exist. Please create an output.dat file for further processing.");
                    exit(1);
                }
            }
            else
            {
                output = fopen(optarg, "w");
                if(!output)
                {
                    perror("Error. Output file was unable to be opened. Please try again with a valid file.");
                    exit(1);
                }
            }
            break;
        //input
        case 'i':
            //no argument
            noOption = 1;
            if(optarg == NULL && optind < argc && argv[optind][0] != '-')
            {
                optarg = argv[optind++];
            }
            if(optarg == NULL)
            {
                input = fopen("input.dat", "r");

                if(!input)
                {
                    perror("Error. File does not exist. Please create an input.dat file for further processing.");
                    exit(1);
                }
            }
            //argument given
            else
            {
                input = fopen(optarg, "r");
                if(!input)
                {
                    perror("Error. File does not exist, or was unable to be opened. Please ensure that a valid output file exists.");
                    exit(1);
                }
            }
            break;
        case ':':
            perror("Error. Option is undefined. Try again by assigning a value to the argument.");
            exit(1);
            break;
        case '?':
            perror("Error. Option does not exist, or an argument is required. Please try a valid option, or use option 'h' for help.");
            exit(1);
        default:
            break;
        }
    }
    if(noOption==0)
    {
        input = fopen("input.dat", "r");
            if(!input)
            {
                perror("Error. File does not exist. Please create an input.dat file for further processing.");
                exit(1);
            }
        output = fopen("output.dat", "w");
            if(!output)
            {
                perror("Error. File does not exist. Please create an input.dat file for further processing.");
                exit(1);
            }    
    }
    fscanf(input, "%d", &forks);
    fgetc(input);
    for(int i = 0; i < forks; i++)
    {
        childID = fork();
        if(childID == 0)
        {
            for(int k = 0; k<i*2; k++)
            {
                do{
                    f = fgetc(input);
                }while(f != '\n');
            }
            fscanf(input, "%d", &size);
            outputStack = newStack(size);
            int temp;
            for(int j = 0; j <size ; j++)
            {
                fscanf(input, "%d", &temp);
                push(outputStack, temp);
            }
            fgetc(input);
            pid = getpid();
            sprintf(mypid, "%d", pid);
            fputs(mypid, output);
            fputc(58, output);
            fputc(32, output);
            max = sizeStack(outputStack);
                
            for(int k = 0; k < max; k++)
            {
                itoa(outputStack->items[outputStack->top], str, 10);
                fputs(str, output);
                fputc(32,output);
                pop(outputStack);
            }
            fputc(10, output);
            return (pid);
        }
        else
        {
            pid = wait(&status);
        }
    }
    fputs("All children were: ", output);
    idStack = newStack(forks+1);
    push(idStack, pid);
    push(idStack, pid-1);
    push(idStack, pid-2);
    for (int x = 0; x<forks; x++)
    {
        itoa(idStack->items[idStack->top], str, 10);
        fputs(str, output);
        fputc(32, output);
        pop(idStack);
    }
    fputs("\nParent PID: ", output);
    itoa(getpid(), string, 10);
    fputs(string, output);
    

    

}

struct stack* newStack(int capacity)
{
    struct stack *pt = (struct stack*)malloc(sizeof(struct stack));
 
    pt->maxsize = capacity;
    pt->top = -1;
    pt->items = (int*)malloc(sizeof(int) * capacity);
 
    return pt;
}

void push(struct stack *pt, int x)
{
    // check if the stack is already full. Then inserting an element would
    // lead to stack overflow
    if (isFull(pt))
    {
        exit(EXIT_FAILURE);
    }
 
    // add an element and increment the top's index
    pt->items[++pt->top] = x;
}

int pop(struct stack *pt)
{
    // check for stack underflow
    if (isEmpty(pt))
    {
        exit(EXIT_FAILURE);
    }
    // decrement stack size by 1 and (optionally) return the popped element
    return pt->items[pt->top--];
}

int peek(struct stack *pt)
{
    // check for an empty stack
    if (!isEmpty(pt)) {
        return pt->items[pt->top];
    }
    else {
        exit(EXIT_FAILURE);
    }
}

int isEmpty(struct stack *pt) {
    return pt->top == -1;                   // or return size(pt) == 0;
}

int sizeStack(struct stack *pt) {
    return pt->top + 1;
}

int isFull(struct stack *pt) {
    return pt->top == pt->maxsize - 1;      // or return size(pt) == pt->maxsize;
}
 
 void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Function to reverse `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}
 
// Iterative function to implement `itoa()` function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}