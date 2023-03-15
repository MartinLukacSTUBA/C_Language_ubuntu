#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<math.h>
#include<sys/wait.h>
#include<time.h>
#include<pthread.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

#define SHMSZ 27
#define _USE_MATH_DEFINES

int servernumber;
void* thread2(void* arg)
{
	int lower_limit, upper_limit;
	lower_limit = 0;
	upper_limit = 100;

	servernumber = ((rand() % (upper_limit - lower_limit + 1)) + lower_limit);
}

void* vlaknoo()
{
	pthread_t ptid2;
	pthread_create(&ptid2, NULL, thread2, NULL);
	pthread_join(ptid2, NULL);
}

int* winner(int c1, int c2, int c3, int c4)
{
	int clients[] = { c1,c2,c3,c4 };
	int* c_porad;
	int i;
	c_porad = (int*)malloc(4 * sizeof(int));
	for (i = 0;i < 4;i++) {
		c_porad[i] = 110;
	}
	int minimum = 101;
	int j = 0;
	for (i = 0;i < 4;i++) {
		if (clients[i] < minimum) {
			minimum = clients[i];
		}
	}
	for (i = 0;i < 4;i++) {
		if (clients[i] == minimum) {
			c_porad[j] = i;
			j++;
		}
	}
	return c_porad;
}


int absV(int number)
{
	if (number < 0)
	{
		number = number * (-1);
	}
	return number;
}



void* vlakno(void* arg)
{
	//vymazanie prechadzajuceho vlakna
	pthread_detach(pthread_self());
	//vypis vlaknom v threade
	printf("Server stopped running.....\n");

	//zatvorenie vlakna
	pthread_exit(NULL);
}

void vypisvlaknom()
{
	pthread_t ptid;

	// tvorba threadu a ulozenie donho funkcie
	pthread_create(&ptid, NULL, &vlakno, NULL);
	pthread_exit(NULL);
}


void sigquit() {
	FILE* fa;
	fa = fopen("Server.txt", "w");
	time_t actualtime;
	actualtime = time(NULL);
	fprintf(fa, "Server is still running... 	%s \n", ctime(&actualtime));
	fclose(fa);
}


timer_t vytvorCasovac(int signal)
{
	struct sigevent kam;
	kam.sigev_notify = SIGEV_SIGNAL;
	kam.sigev_signo = signal;

	timer_t casovac;
	timer_create(CLOCK_REALTIME, &kam, &casovac);
	return(casovac);
}

//it_value kolko sa spusta . it_interval PERIODICITA ako casto sa bude spustat
void spusticasovac(timer_t casovac, int sekundy)
{
	struct itimerspec cass;
	cass.it_value.tv_sec = sekundy;
	cass.it_value.tv_nsec = 0;
	cass.it_interval.tv_sec = sekundy;
	cass.it_interval.tv_nsec = 0;
	timer_settime(casovac, CLOCK_REALTIME, &cass, NULL);
}


int main(void) {

	time_t actualtime;
	actualtime = time(NULL);
	timer_t casovac;


	srand(time(NULL));// na generovanie random cisel
	FILE* fw;
	fw = fopen("Server.txt", "w");
	fprintf(fw, "Running....\n	%s", ctime(&actualtime));
	printf("Running....\n");

	signal(SIGQUIT, sigquit);
	casovac = vytvorCasovac(SIGQUIT);
	spusticasovac(casovac, 3);

	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);//vytvor socket

	if (sock_desc == -1) {
		printf("Cannot create socket!\n");
		fprintf(fw, "Cannot create socket!\n");
		printf("Server could not run.\n");
		fprintf(fw, "Server could not run.\n");
		return 0;
	}
	else
	{
		printf("Socket for server was created successfully\n");
		fprintf(fw, "Socket for server was created successfully\n");
	}

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY; //socket prijma spojenie z hocijakej ip
	server.sin_port = htons(1032);//pod 1024 sys sluzby, nad 65 xxx sa neprijma

	if (bind(sock_desc, (struct sockaddr*)&server, sizeof(server)) != 0)//registruj socket
	{
		printf("cannot bind socket!\n");
		fprintf(fw, "cannot bind socket!\n");
		close(sock_desc);
		return 0;
	}
	else
	{
		printf("Socket for server successfully binded !\n");
		fprintf(fw, "Socket for server successfully binded !\n");
	}

	if (listen(sock_desc, 4) != 0)// max 4ja uzivatelia
	{
		printf("Cannot listen on socket! \n");
		fprintf(fw, "Cannot listen on socket! \n");
		close(sock_desc);
		return 0;
	}
	else
	{
		printf("Listening on sockets is available !\n");
		fprintf(fw, "Listening on sockets is available !\n");
	}

	//socket pre klienta 1
	struct sockaddr_in client1;
	memset(&client1, 0, sizeof(client1));
	socklen_t len1 = sizeof(client1);
	int temp_sock_desc1;
	//ak nadviaze spojenie vytvori descriptor
	temp_sock_desc1 = accept(sock_desc, (struct sockaddr*)&client1, &len1);


	if (temp_sock_desc1 == -1)
	{
		printf("Cannot accept client1 !\n");
		fprintf(fw, "Cannot accept client1 !\n");
		close(sock_desc);
		return 0;
	}
	else
	{
		printf("Client 1 accepted !\n");
		fprintf(fw, "Client 1 accepted !	%s\n", ctime(&actualtime));
	}
	actualtime = time(NULL);

	//socket pre klienta 2
	struct sockaddr_in client2;
	memset(&client2, 0, sizeof(client2));
	socklen_t len2 = sizeof(client2);
	int temp_sock_desc2;
	//ak nadviaze spojenie vytvori descriptor
	temp_sock_desc2 = accept(sock_desc, (struct sockaddr*)&client2, &len2);


	if (temp_sock_desc2 == -1)
	{
		printf("Cannot accept client2 !\n");
		fprintf(fw, "Cannot accept client2 !\n");
		close(sock_desc);
		return 0;
	}
	else
	{
		printf("Client 2 accepted !\n");
		fprintf(fw, "Client 2 accepted !	%s\n", ctime(&actualtime));
	}
	actualtime = time(NULL);

	//socket pre klienta 3
	struct sockaddr_in client3;
	memset(&client3, 0, sizeof(client3));
	socklen_t len3 = sizeof(client3);
	int temp_sock_desc3;
	//ak nadviaze spojenie vytvori descriptor
	temp_sock_desc3 = accept(sock_desc, (struct sockaddr*)&client3, &len3);


	if (temp_sock_desc3 == -1)
	{
		printf("Cannot accept client3 !\n");
		fprintf(fw, "Cannot accept client3 !\n");
		close(sock_desc);
		return 0;
	}
	else
	{
		printf("Client 3 accepted !\n");
		fprintf(fw, "Client 3 accepted !	%s\n", ctime(&actualtime));
	}

	actualtime = time(NULL);
	//socket pre klienta 4
	struct sockaddr_in client4;
	memset(&client4, 0, sizeof(client4));
	socklen_t len4 = sizeof(client4);
	int temp_sock_desc4;
	//ak nadviaze spojenie vytvori descriptor
	temp_sock_desc4 = accept(sock_desc, (struct sockaddr*)&client4, &len4);


	if (temp_sock_desc4 == -1)
	{
		printf("Cannot accept client4 !\n");
		fprintf(fw, "Cannot accept client4 !\n");
		close(sock_desc);
		return 0;
	}
	else
	{
		printf("Client 4 accepted !\n");
		fprintf(fw, "Client 4 accepted !	%s\n", ctime(&actualtime));
	}

	actualtime = time(NULL);

	printf("4 clients connected...\n");
	fprintf(fw, "4 clients connected...%s\n", ctime(&actualtime));


	printf("Server generates number from interval 0 - 100\n");
	fprintf(fw, "Server generates number from interval 0 - 100\n");
	printf("Clients guess one number from this interval\n");
	fprintf(fw, "Clients guess one number from this interval\n");
	printf("The one whose number was the closest number generated by the server wins\n");
	fprintf(fw, "The one whose number was the closest number generated by the server wins\n");


	vlaknoo();

	char* first_text = "Server generates number from interval 0 - 100\nTry to guess that number !\nPlease send me your tip+\n";


	int response;
	char buffer[1024];

	strcpy(buffer, first_text);

	response = write(temp_sock_desc1, buffer, strlen(buffer));
	if (response == -1)
	{
		printf("Could not send data to client1!\n");
		fprintf(fw, "Could not send data to client1!\n");
	}
	else
	{
		printf("Data was successfully send to client1!\n");
		fprintf(fw, "Data was successfully send to client1!\n");
	}

	response = write(temp_sock_desc2, buffer, strlen(buffer));
	if (response == -1)
	{
		printf("Could not send data to client2!\n");
		fprintf(fw, "Could not send data to client2!\n");
	}
	else
	{
		printf("Data was successfully send to client2!\n");
		fprintf(fw, "Data was successfully send to client2!\n");
	}

	response = write(temp_sock_desc3, buffer, strlen(buffer));
	if (response == -1)
	{
		printf("Could not send data to client3!\n");
		fprintf(fw, "Could not send data to client3!\n");
	}
	else
	{
		printf("Data was successfully send to client3!\n");
		fprintf(fw, "Data was successfully send to client3!\n");
	}

	response = write(temp_sock_desc4, buffer, strlen(buffer));
	if (response == -1)
	{
		printf("Could not send data to client4!\n");
		fprintf(fw, "Could not send data to client4!\n");
	}
	else
	{
		printf("Data was successfully send to client4!\n");
		fprintf(fw, "Data was successfully send to client4!\n");
	}


	actualtime = time(NULL);
	printf("Data sent to all clients\n");
	fprintf(fw, "Data sent to all clients	%s\n", ctime(&actualtime));
	sleep(5);// caka kym sa spracuju data na  klientoch

	int client1_tip, client2_tip, client3_tip, client4_tip;

	/////////////////////////////////////////////////////
	int counterforsema = 0;
	char* name = "my_semaphore";
	int VALUE = 2;
	sem_t* sema;
	//ak takyto semafor neexistuje vytvor ho
	actualtime = time(NULL);
	printf("Create a  semaphore, - %s, its value is %d\n", name, VALUE);
	fprintf(fw, "Open a  semaphore, - %s, its value is %d%s\n", name, VALUE, ctime(&actualtime));
	sema = sem_open(name, O_CREAT, 0666, VALUE);
	//stoj na semafore a zniz jeho hodnotu o 1dna , posun sa ked sa zvysi o 1 
	sem_wait(sema);
	//sem wai znizuje
	//semafor dalej

	for (counterforsema = 0;counterforsema < 4;) {//neuplny if bez poslednej  vyuzivanie overenia semaforu
		response = recv(temp_sock_desc1, &client1_tip, sizeof(int), 0);
		if (response == -1)
		{
			printf("Cannot read from client 1!\n");
			fprintf(fw, "Cannot read from client 1!\n");
		}
		if (response != (-1)) {
			counterforsema++;// ak sa  precita sprava  zvys pocitadlo o 1 
		}


		response = recv(temp_sock_desc2, &client2_tip, sizeof(int), 0);
		if (response == -1)
		{
			printf("Cannot read from client 2!\n");
			fprintf(fw, "Cannot read from client 2!\n");
		}
		if (response != (-1)) {
			counterforsema++;
		}

		response = recv(temp_sock_desc3, &client3_tip, sizeof(int), 0);
		if (response == -1)
		{
			printf("Cannot read from client 3!\n");
			fprintf(fw, "Cannot read from client 3!\n");
		}
		if (response != (-1)) {
			counterforsema++;
		}

		response = recv(temp_sock_desc4, &client4_tip, sizeof(int), 0);
		if (response == -1)
		{
			printf("Cannot read from client 4!\n");
			fprintf(fw, "Cannot read from client 4!\n");
		}
		if (response != (-1)) {
			counterforsema++;
		}

		if (counterforsema == 4) {//ak 4x sa podarilo precitat spravu od klienta
			//tak mi semafor zvys o jedna a pusti ho dalej inak zrus program 
			sem_post(sema);//zvysuje program o 1
			actualtime = time(NULL);
			printf("Citanie od klientov NEzlyhalo server bezi! \n");
			fprintf(fw, "Citanie od klientov NEzlyhalo server bezi! %s\n", ctime(&actualtime));
		}
		else {
			actualtime = time(NULL);
			printf("Citanie od klientov zlyhalo server padol! \n");
			fprintf(fw, "Citanie od klientov zlyhalo server padol! %s\n", ctime(&actualtime));
		}

	}//koniec semaforu
	/////////////////////////
	actualtime = time(NULL);
	printf("\n\nGenerated number was : %d! \n\n", servernumber);
	fprintf(fw, "\n\nGenerated number was : %d ! %s\n\n", servernumber, ctime(&actualtime));

	printf("Client1 tip : %d\n", client1_tip);
	fprintf(fw, "Client1 tip : %d\n", client1_tip);

	printf("Client2 tip : %d\n", client2_tip);
	fprintf(fw, "Client2 tip : %d\n", client2_tip);

	printf("Client3 tip : %d\n", client3_tip);
	fprintf(fw, "Client3 tip : %d\n", client3_tip);

	printf("Client4 tip : %d\n", client4_tip);
	fprintf(fw, "Client4 tip : %d\n", client4_tip);

	int aVc1, aVc2, aVc3, aVc4;

	aVc1 = absV(client1_tip - servernumber);
	aVc2 = absV(client2_tip - servernumber);
	aVc3 = absV(client3_tip - servernumber);
	aVc4 = absV(client4_tip - servernumber);


	printf("\n \t ABV : %d %d %d %d\n", aVc1, aVc2, aVc3, aVc4);


	char* second_text = "Thank you for your tip, I am calculating the winner\n+";
	strcpy(buffer, second_text);


	response = write(temp_sock_desc1, buffer, strlen(buffer));
	if (response == -1)
	{
		printf("Could not send data to client1!\n");
		fprintf(fw, "Could not send data to client1!\n");
	}
	else
	{
		printf("Data was successfully send to client1!\n");
		fprintf(fw, "Data was successfully send to client1!\n");
	}

	response = write(temp_sock_desc2, buffer, strlen(buffer));
	if (response == -1)
	{
		printf("Could not send data to client2!\n");
		fprintf(fw, "Could not send data to client2!\n");
	}
	else
	{
		printf("Data was successfully send to client2!\n");
		fprintf(fw, "Data was successfully send to client2!\n");
	}

	response = write(temp_sock_desc3, buffer, strlen(buffer));
	if (response == -1)
	{
		printf("Could not send data to client3!\n");
		fprintf(fw, "Could not send data to client3!\n");
	}
	else
	{
		printf("Data was successfully send to client3!\n");
		fprintf(fw, "Data was successfully send to client3!\n");
	}

	response = write(temp_sock_desc4, buffer, strlen(buffer));
	if (response == -1)
	{
		printf("Could not send data to client4!\n");
		fprintf(fw, "Could not send data to client4!\n");
	}
	else
	{
		printf("Data was successfully send to client4!\n");
		fprintf(fw, "Data was successfully send to client4!\n");
	}
	actualtime = time(NULL);
	int* win = winner(aVc1, aVc2, aVc3, aVc4);
	int i;

	for (i = 0;i < 4;i++) {
		if (win[i] > 100)continue;
		actualtime = time(NULL);
		printf("Closest tip had user(s) : %d!\n", win[i] + 1);
		fprintf(fw, "Closest tip had user(s) : %d! %s\n", win[i] + 1, ctime(&actualtime));
	}
	//spravime zdielanu pamat pre  klientov

	char c;
	int shmid;
	key_t key;
	char* shm, * s;
	key = 5678;
	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("shmget");//toto if tvorba zdiel pamate , ak sa nevytvori napise error
		exit(1);
	}

	if ((shm = shmat(shmid, NULL, 0)) == (char*)-1) {
		perror("shmat");
		exit(1);
	}
	s = shm;


	for (i = 0;i < 4;i++) {
		if (win[i] > 100)continue;
		*s++ = (char)win[i] + 49;
		*s = '\0';// S a shm pracuju na rovnakej adrese v pamati nezalezi kam veci zapiseme zapisuju sa na oba premenne
	}//TUNA  si ulozis do odosielaneho stringu to co posielas  do zdielanej pamate 

	actualtime = time(NULL);
	printf("In SHM is saved winner...\n");
	fprintf(fw, "In SHM is saved winner...	%s\n", ctime(&actualtime));

	//3ti argument ako struktura pozri hore
	sleep(5);
	shmctl(shmid, IPC_RMID, 0);//zmaze shm
	printf("SHM was deleted.\n");
	actualtime = time(NULL);
	fprintf(fw, "SHM was deleted. 	%s\n", ctime(&actualtime));



	pid_t fork_pid = fork();

	if (fork_pid == 0) {// TOTO POZATVARA DIETA RESP POTOMOK PROCESU
		close(sock_desc);//zavrie socket komunikacie
		close(temp_sock_desc1);//zavrie descriptor pre clienta 1
		close(temp_sock_desc2);//zavrie descriptor pre clienta 2
		close(temp_sock_desc3);//zavrie descriptor pre clienta 3
		close(temp_sock_desc4);//zavrie descriptor pre clienta 4
		printf("Sockets were closed by child\n");
		actualtime = time(NULL);
		fprintf(fw, "Sockets were closed by child%s \n", ctime(&actualtime));
	}
	else {//toto je robota predka, rodica
		printf("Last output was written by parent ! \n");
		printf("Documentation of this server was closed by  parent !\n");
		printf("Pointers were released by parent ! \n");
		fprintf(fw, "Pointers were released by parent !%s\n", ctime(&actualtime));
		actualtime = time(NULL);
		fprintf(fw, "Documentation of this server was closed by  parent !%s\n", ctime(&actualtime));
		fprintf(fw, "Last output was written by parent ! \n");
		vypisvlaknom();
		fprintf(fw, "Server stopped runnig.....%s\n", ctime(&actualtime));

		sleep(20);
		free(first_text);
		free(name);
		free(second_text);
		fclose(fw);
	}
	sleep(3);

	return 0;

}
