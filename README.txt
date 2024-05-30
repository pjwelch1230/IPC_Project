Patrick Welch
CSCE 311
Project 2

Program has utilized code from https://man7.org/linux/man-pages/man7/unix.7.html

Also a special thanks to Professor Zuo Fei for providing a base for code as well as for help with the word search problem
	
	##### Use Instructions #####
	
	make server: compiles the server.c program
	
	./server (target word): runs server program with desired word in argument field
	
	make client: compiles the client.c program
	
	./client (file path): runs client program with file path to desired file in the argument field
	
	make clean: removes executable server and client files.
	
	##### NOTE #####
	
	-Must run server and client programs in seperate teminals.
	
	-Run server program before you run the client program. Server must be running before client tries 
	to connect, otherwise client program will exit.
	
	-If you receive message "address is already in use", close out the terminals, run 
	"make clean", and then recompile the programs. After that you should be good to run them
	again.
	
	##### Server.c File Description #####
	
	-The goal of the server program is to take in a string as part of an argument, receive a
	text file from a client server, search the lines of the text file for instances of the
	string argument, and send to the client only the lines containing the target word.
	
	-After the client is connected to the server  multiple strings are declared as well as
	an int. Immediately afterwards the program enters a for loop. Inside the for loop is uses
	the int ret and the read() call to receive the file from the client program and place it
	into a string. Then it uses a while loop to search through the string and adds only the 
	lines containing the target word to a string called result. This string is printed to the
	server terminal as a test to make sure it is working properly.
	
	-After the program exits the for loop, it uses sprintf to move the result string to our
	buffer bufferOut, which is then written to the client_fd socket. After that the client_fd 
	socket is closed, however to end the program one should enter ctrl+C (^C). 
	
	
	##### Client.c File Description #####
	-The goal of the client program is to take in a file path as part of an argument, connect
	to a host program, send the host the file taken in through the argument, and then receive 
	only the lines containing a specific word as designated on the side of the host and 
	output these lines to the console.
	
	-After the client is connected to the server, it immediately writes to the server using 
	the write() call, and then writes an additional character to the end so that the host 
	will know when it has reached the end of the file. Afterwards, the client waits and reads 
	the socket for the host's response.
	
	-After the client receives the host's response it writes to the console upon the closing 
	of the server program. 
	
	
