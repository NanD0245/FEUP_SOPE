#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
1
	i) polling/interrupts
	ii) polling
	iii) interrupts
	iv) dma
	v) polling/interrupts
*/

/*
2
	i) device-independent software
	ii) hardware
	iii) device drivers
	iv) device-independent software
	v) user processes
*/

int f4() {
	time_t t;
	time(&t);

	printf("the number of clock interruptions (clock ticks) per second: %ld\n", CLOCKS_PER_SEC);

	printf("date: %s", ctime(&t));

	printf("size: %ld\n", sizeof(time(NULL)));

	return 0;
}

/*
5
	/proc/stat, with its field "btime", which gives in seconds the booting time of the machine,
	 since the "Epoch" (1970-01-01 00:00:00 +0000, UTC);

	Its possible to calculate the time of the day with the "btime" on /proc/stat,
	the time at which the system booted, in seconds since the Unix epoch.
*/


/*
6
Scheduling of processes:
	- As a process is started, the scheduler initializes the counter in clock ticks for the process. When the
counter gets to zero, clock driver calls the scheduler to set up another process.

Accounting of processing times (to be charged to each user, for processes they have run):
	- When the process start, the system only needs to save the value of the system clock ticks and compare it to the current system clock ticks 
*/


int f7() {
	char c;
	struct termios tms, tms_ini;

	printf("\nTo end, press [Enter]\n");

	tcgetattr(STDIN_FILENO, &tms_ini); // read console's configuration
	tms = tms_ini;

	tms.c_lflag &= ~ECHO; // inhibit char echoing
	tms.c_lflag &= ~ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &tms); // set new configuration

	do {
		if (read(STDIN_FILENO, &c ,1) != 1) {
			perror("read failed");
		}
		else if (c == 127) {
			write(STDOUT_FILENO, "\b \b",3);
		} else if (c != '\n') {
			printf("%c", '*');
		}
	} while (c != '\n');	// wait for [ENTER]

	tcsetattr(STDIN_FILENO, TCSANOW, &tms_ini);	// reset configuration
	// experiment this program with the above line commented! ;-)
	printf("\nInitial console configuration has been reset!\n");

	return 0;
}

/*
8
	i) 500107862016/255/63/60801 ~= 512 Bytes
	
	ii) True
*/

/*
9
Two advantages of SSD over HDD:
	- SSD is faster than HDD
	- SSD are more durable than HDD
	- SSD have less power consuption than HDD

Two advantages of HDD over SSD:
	- HDD's are cheaper than SSD
	- HDD doesn’t have any limit to write cycles

Why is it that Flash NAND storage devices probably take much more time to modify a disk file than to merely append to it?
	-

Random writes to SSD generally perform better (about twice better) than random reads. Why?
	-

*/


int f10a() {
	char c;
	int diff = 32;
	struct termios tms, tms_ini;

	tcgetattr(STDIN_FILENO, &tms_ini); // read console's configuration
	tms = tms_ini;

	tms.c_lflag &= ~ECHO; // inhibit char echoing
	tms.c_lflag &= ~ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &tms); // set new configuration

	do {
		if (read(STDIN_FILENO, &c ,1) != 1) {
			//perror("read failed");
			break;
		} 
		else if (c == 127) {
			write(STDOUT_FILENO, "\b \b",3);
		} 
		else if (c > 96 && c < 123) {
			int a = c - diff;
			write(STDOUT_FILENO, &a, 1);
		} 
		else {
			write(STDOUT_FILENO, &c, 1);
		}
	} while (1);	// wait for [ENTER]

	tcsetattr(STDIN_FILENO, TCSANOW, &tms_ini);
	return 0;
}

int f10b(int argc, char* argv[]) {
	if (argc == 1)
		f10a();
	else if (argc >= 2) {
		char c;
		int diff = 32;
		struct termios tms, tms_ini;

		tcgetattr(STDIN_FILENO, &tms_ini); // read console's configuration
		tms = tms_ini;

		tms.c_lflag &= ~ECHO; // inhibit char echoing
		tms.c_lflag &= ~ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &tms); // set new configuration

		if (argc == 2) {
			int fd;
			if ((fd = open(argv[1], O_RDONLY)) == -1) perror("Error: ");

			while (read(fd,&c,1) > 0) {
				if (c == 127) {
					write(STDOUT_FILENO, "\b \b",3);
				} else if (c > 96 && c < 123) {
					int a = c - diff;
					write(STDOUT_FILENO, &a, 1);
				} else {
					write(STDOUT_FILENO, &c, 1);
				}
			}
		} else if (argc == 3) {
			int fd1, fd2;
			if ((fd1 = open(argv[1], O_RDONLY)) == -1 || (fd2 = open(argv[2], O_WRONLY | O_CREAT)) == -1) perror("Error: ");

			while (read(fd1,&c,1) > 0) {
				if (c == 127) {
					write(fd2, "\b \b",3);
				} else if (c > 96 && c < 123) {
					int a = c - diff;
					write(fd2, &a, 1);
				} else {
					write(fd2, &c, 1);
				}
			}
		}
		tcsetattr(STDIN_FILENO, TCSANOW, &tms_ini);
	}
	return 0;
}

//tee command -> the same but reads from STDIN and writes to a file


int main(int argc, char* argv[]) {
	setbuf(stdout, NULL);
	//return f4();
	return f7();
	//return f10a();
	//return f10b(argc, argv);
	return 0;
}