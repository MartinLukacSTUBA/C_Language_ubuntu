CC=gcc

compile:
	$(CC) -o ser server.c -lpthread -lrt
	$(CC) -o c1 client1.c -lpthread -lrt
	$(CC) -o c2 client2.c -lpthread -lrt
	$(CC) -o c3 client3.c -lpthread -lrt
	$(CC) -o c4 client4.c -lpthread -lrt
	

