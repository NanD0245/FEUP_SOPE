#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/*
1.
    a)
        i - "first fit" - 20KiB ; 10KiB ; 18KiB
        ii - "next fit" - 18KiB ; 20KiB ; 12KiB
        iii - "best fit" - 12KiB ; 10KiB ; 9KiB
        iv - "worst fit" - 20KiB ; 18KiB ; 15KiB

    b) iv > ii > i > iii
*/

/*
2.
    advantage: memory are divided into very small sections
    disadvantage: memory sections of a program aren't contigous
*/

/*
3.
The different pages are not contiguos so they cant be associated to an physical address.
If the strategy dictated by the statement were adopted, we would always have a restricted space 
for each process, which did not make possible a continues allocating memory space for processes.
*/

int f4() {
    size_t size = 50 * pow(10,6);
    int sum = 0; 
    while(1) {
        //sleep(1);
        if (malloc(size) == NULL)
            break;
        sum += 50;
        printf("space: %d MB\n",sum);
    }
    printf("Rejected Malloc");
    return 0;
}


//8
void *garbage_collected_malloc(size_t size) {
    void *p = malloc(size);
    return p;
}

int f8(int argc, char* argv[]) {
    char *buf;

    if (argc < 2) {
        fprintf (stderr, "\nUsage: %s <string>\n", argv[0]);
        exit (1);
    }

    printf("Input string: %s\n", argv[1]);

    buf = (char *) garbage_collected_malloc(strlen(argv[1]) + 1);
    
    strncpy(buf, argv[1], strlen(argv[1]) + 1);
    
    for (int i=0; i < strlen(argv[1]) ; i++) {
        buf[i] = toupper(buf[i]);
    }
    
    printf("Output string: %s\n", buf);
    free(buf);
    return 0;
} 


//9
int f9() {
    char buff[15];
    int pass = 0;

    printf("\n Enter the password : \n");
    fgets(buff, 15, stdin); //gets -> danger function, never use it
    if (strcmp(buff, "thegeekstuff")) {
        printf ("\n Wrong Password. \n");
        pass = 0; //simple correction of the bug
    } else {
        printf ("\n Correct Password.\n");
        pass = 1;
    }
    if (pass) {  // do somelhing priviledged stuff
        printf ("\n Root privileges given to the user.\n");
    }
    return 0;
}

/*
a) if the user writes a password longer than 15 chars, the value of pass will change, given the root privileges to the user
*/

/*
11
The basic difference between paging and segmentation is that a page is always of fixed block size whereas, a segment is of variable size.

Paging may lead to internal fragmentation as the page is of fixed block size, but it may happen that the process does not acquire the entire
block size which will generate the internal fragment in memory. The segmentation may lead to external fragmentation as the memory is filled
with the variable sized blocks.

In paging the user only provides a single integer as the address which is divided by the hardware into a page number and Offset.
On the other hands, in segmentation the user specifies the address in two quantities i.e. segment number and offset.

The size of the page is decided or specified by the hardware. 
On the other hands, the size of the segment is specified by the user.

In paging, the page table maps the logical address to the physical address, and it contains base address of each page stored in
the frames of physical memory space. 

However, in segmentation, the segment table maps the logical address to the physical address, and it contains 
segment number and offset (segment limit).
*/

/*
12

i) - 2
ii) - 3
iii) - 1
iv) - 2
*/


int main(int argc, char* argv[], char* envp[]) {
    //return f4();
    //return f8(argc, argv);
    return f9();
}