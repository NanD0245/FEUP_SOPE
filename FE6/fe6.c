#include <stdio.h>
#include <unistd.h>

/*
1
Advantages:
	-faster
	-independent of OS

Desadvantages:
	-Adress tables ocupies some space
*/

/*
2
Naming a file should provide unequivocal (human) identification which brings good organization and easy identification of a file for a efficient access to data.

Databases and web search engines:
	-The sames rules of file naming can be applied, in the context of databases or search engines. 
*/

/*
3
By default, Microsoft decides to hide file extensions in Windows so that a file named 'report. txt' is simply shown in File Explorer as 'report'.
The original reason for this was probably to make it less confusing to users, but regardless of the reason, it is a security risk that attackers abuse.
*/

int f4() { 
	int i = 1234567;
	write(STDOUT_FILENO, &i, sizeof(i));
	printf("\n%d", i);
	return 0;
}


int f5() {

}


/*
binary file: 
	87d6 1200 0a31 3233 3435 3637 

text file:
	��
	1234567

note: 1234567(dec) = 0x12D687 (first six numbers in binary file)
*/

/*
6
FILE:
	- name: (human) name of a file
	- attributes: file type, permissions, number of links, file size, date and time of last modification, etc
	- modes: read/write/execute file
	- permissions: read/write/execute permissions of the file for each group
*/ 

/*
7
Create: create a file
Delete: delete a file
Append: append something (ex: text) to a file

struct dirent *readdir(DIR *dirp);
ssize_t read(int fd, void *buf, size_t count);

read -> read() attempts to read up to count bytes from file descriptor fd into the buffer starting at buf.
readdir -> The readdir() function returns a pointer to a dirent structure representing the next directory entry in the directory stream pointed to by dirp.
*/

int f8() {

}

/*
9

*/

/*
10
Hard Link: link that connects many files with the same inode, so the same data block is shared with these files.
A change in the data of a file will reflect on the others.

Symbolic Link: link that acts as a pointer to another file or a shortcut of a file. Each soft link file contains a different inode which links to the
original file. Just like hard links, changing the content of a soft link file will reflect on the others.
*/

/*
11

*/
int main(int argc, char * argv[]) {
	return f4();
}