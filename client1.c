#include <sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<math.h>
#include<sys/types.h>
#include<time.h>


#define SHMSZ 27
int randomize(void) {
	int lower_limit, upper_limit;
	lower_limit = 0;
	upper_limit = 100;

	return((rand() % (upper_limit - lower_limit + 1)) + lower_limit);
}

void output(char point[]) {
	int i, l;
	l = strlen(point);
	for (i = 0;i < l;i++) {
		if (point[i] == '+') {
			point[i] = '\0';
			printf("%c", point[i]);
			break;
		}
		else {
			printf("%c", point[i]);
		}
	}
}


int main()
{
	srand(time(NULL));
	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);// funkcia sokcet vracia identifikator socketu
	if (sock_desc == -1) {
		printf("Cannot create socket!\n");
		return 0;
	}
	//socket settings
	struct sockaddr_in client;//struk pre klienta
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;client.sin_addr.s_addr = inet_addr("127.0.0.1");//joinujem sa len v ramci PC, resp lokal host
	client.sin_port = htons(1032);//port servera

	if (connect(sock_desc, (struct sockaddr*)&client, sizeof(client)) != 0)
	{
		printf("Cannot connetct to server !\n");
		close(sock_desc);
		return 0;
	}

	char buffer_c1[512];
	int number;
	int response;

	number = randomize();

	response = read(sock_desc, buffer_c1, 512);
	if (response == -1)
	{
		printf("Cannot read from server !\n");
		return 0;
	}

	output(buffer_c1);

	printf("\nI am sending %d to server\n\n\n", number);
	response = send(sock_desc, &number, sizeof(int), 0);
	if (response == -1)
	{
		printf("Cannot send data to server !\n");
	}

	response = read(sock_desc, buffer_c1, 512);
	if (response == -1)
	{
		printf("Cannot read from server !\n");
		return 0;
	}

	output(buffer_c1);

	printf("The winner(s) is/are :");
	int shmid;
	key_t key;
	char* shm, * s;
	key = 5678;

	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
		perror("shmget");
		exit(1);
	}
	if ((shm = shmat(shmid, NULL, 0)) == (char*)-1) {
		perror("shmat");
		exit(1);
	}

	for (s = shm;*s != '\0';s++) {
		putchar(*s);
		putchar(' ');
	}
	putchar('\n');

	close(sock_desc);
	printf("Client disconnected\n");
	sleep(4);
}
#include <sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<math.h>
#include<sys/types.h>
#include<time.h>


#define SHMSZ 27
int randomize(void) {
	int lower_limit, upper_limit;
	lower_limit = 0;
	upper_limit = 100;

	return((rand() % (upper_limit - lower_limit + 1)) + lower_limit);
}

void output(char point[]) {
	int i, l;
	l = strlen(point);
	for (i = 0;i < l;i++) {
		if (point[i] == '+') {
			point[i] = '\0';
			printf("%c", point[i]);
			break;
		}
		else {
			printf("%c", point[i]);
		}
	}
}


int main()
{
	srand(time(NULL));
	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);// funkcia sokcet vracia identifikator socketu
	if (sock_desc == -1) {
		printf("Cannot create socket!\n");
		return 0;
	}
	//socket settings
	struct sockaddr_in client;//struk pre klienta
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;client.sin_addr.s_addr = inet_addr("127.0.0.1");//joinujem sa len v ramci PC, resp lokal host
	client.sin_port = htons(1032);//port servera

	if (connect(sock_desc, (struct sockaddr*)&client, sizeof(client)) != 0)
	{
		printf("Cannot connetct to server !\n");
		close(sock_desc);
		return 0;
	}

	char buffer_c1[512];
	int number;
	int response;

	number = randomize();

	response = read(sock_desc, buffer_c1, 512);
	if (response == -1)
	{
		printf("Cannot read from server !\n");
		return 0;
	}

	output(buffer_c1);

	printf("\nI am sending %d to server\n\n\n", number);
	response = send(sock_desc, &number, sizeof(int), 0);
	if (response == -1)
	{
		printf("Cannot send data to server !\n");
	}

	response = read(sock_desc, buffer_c1, 512);
	if (response == -1)
	{
		printf("Cannot read from server !\n");
		return 0;
	}

	output(buffer_c1);

	printf("The winner(s) is/are :");
	int shmid;
	key_t key;
	char* shm, * s;
	key = 5678;

	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
		perror("shmget");
		exit(1);
	}
	if ((shm = shmat(shmid, NULL, 0)) == (char*)-1) {
		perror("shmat");
		exit(1);
	}

	for (s = shm;*s != '\0';s++) {
		putchar(*s);
		putchar(' ');
	}
	putchar('\n');

	close(sock_desc);
	printf("Client disconnected\n");
	sleep(4);
}
