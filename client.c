#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 8192
#define C_FOUND_ID "00"
#define C_NOT_FOUND_ID "01"
#define C_FOUND_PASSWORD "10"
#define C_NOT_FOUND_PASSWORD "11"
#define C_LOGOUT_OK "20"
#define C_LOGOUT_FAILS "21"
#define C_BLOCK "31"
#define C_NEW_USER "40"
#define C_SAME_USER "41"
#define C_CORRECT_PASS "50"
#define C_INCORRECT_PASS "51"
#define C_CORRECT_CODE "60"
#define C_INCORRECT_CODE "61"
#define C_IN_ROOM "70"
#define C_OUT_ROOM "71"
#define C_A_QQ_CORRECT "80"
#define C_A_QQ_INCORRECT "81"
#define C_READY "90"
#define C_NOT_READY "91"
#define C_A_CORRECT "04"
#define C_A_INCORRECT "05"
#define C_HELP_50_OK "14"
#define C_HELP_ADVISORY_OK "15"
#define C_HELP_NOT_OK "16"
#define C_STOP_OK "24"
#define C_STOP_NOT_OK "25"
#define C_CRE_ROOM_SUC "34"
#define C_CRE_ROOM_FAI "35"
#define C_LEAV_ROOM_SUC "36"
#define C_LEAV_ROOM_FAI "37"
#define C_YOU_WIN "45"
#define C_YOU_LOSE "44"
#define C_YOU_LOSE_1 "100"
#define C_YOU_LOSE_2 "200"
#define C_YOU_LOSE_3 "300"
#define C_YOU_STOP_1 "201"
#define C_YOU_STOP_2 "202"
#define C_YOU_STOP_3 "203"
#define C_YOU_STOP_4 "204"
#define C_YOU_STOP_5 "205"
#define C_YOU_STOP_6 "206"
#define C_YOU_STOP_7 "207"
#define C_YOU_STOP_8 "208"
#define C_YOU_STOP_9 "209"
#define C_YOU_STOP_10 "210"
#define C_YOU_STOP_11 "211"
#define C_YOU_STOP_12 "212"
#define C_YOU_STOP_13 "213"
#define C_YOU_STOP_14 "214"
#define C_YOU_STOP_15 "215"
#define C_YOU_ARE_KEY "101"
#define C_WAIT "102"
#define C_ALL_ROOM_INCORRECT_K "999"
#define C_ALL_ROOM_INCORRECT_M "998"
#define C_LOG_SEQ_WRONG "888" 
#define C_NOT_ROOM_MASTER "777" 

#define BLOCKED 0
#define ACTIVE 1
#define MAX 100

//send request to server
int request(int client_sock, char message[])
{
	if (send(client_sock, message, strlen(message), 0) > 0)
	{
		return 1;
	} else return 0;
}

//receive request
int receive(int client_sock, char respond[])
{
	int bytes_received = recv(client_sock, respond, BUFF_SIZE -1, 0);
	if (bytes_received > 0)
	{
		respond[bytes_received] = '\0';
		return 1;
	} else return 0;
}

//change the code from server to notification which users can understand
char *makeFull(char respond[])
{
	if (strcmp(respond, C_IN_ROOM) == 0) //user is in room
	{
		return "WAIT THE START COMMAND!";
	}
	if (strcmp(respond, C_FOUND_ID) == 0) //found user id, now enter password
	{
		return "ENTER THE PASSWORD: ";
	} 
	if (strcmp(respond, C_NOT_FOUND_ID) == 0) //can't find user id, ask user to try again
	{
		return "USER INCORRECT, TRY AGAIN: ";
	}
	if (strcmp(respond, C_NOT_FOUND_PASSWORD) == 0) //password is wrong, ask user to try again
	{
		return "PASS INCORRECT, TRY AGAIN: ";
	}
	if (strcmp(respond, C_LOGOUT_OK) == 0) //log out successful
	{
		return "LOGOUT SUCCESSFUL!\n>ENTER YOUR COMMAND : ";
	}
	if (strcmp(respond, C_LOGOUT_FAILS) == 0) //log out fails
	{
		return "LOGOUT FAILS, TRY AGAIN: ";
	}
	if (strcmp(respond, C_BLOCK) == 0) //user has been blocked
	{
		return "USER BLOCKED, TRY AGAIN: ";
	}
	if (strcmp(respond, C_NEW_USER) == 0) //when sign up an user, now user name is valid, ask user password to create a new
	{
		return "USER VALID, ENTER PASSWORD: ";
	}
	if (strcmp(respond, C_SAME_USER) == 0) //user existed
	{
		return "USER EXITS, PLEASE CHOOSE OTHER NAME: ";
	}
	if (strcmp(respond, C_INCORRECT_PASS) == 0) //password is incorrect
	{
		return "PASS IS TOO SHORT, PLEASE ENTER PASS >= 5 CHARACTER: ";
	}
	if (strcmp(respond, C_CORRECT_CODE) == 0) //code which input is correct, user is created
	{
		return "USER IS CREATED, LOGIN NOW: ";
	}
	if (strcmp(respond, C_CRE_ROOM_SUC) == 0) //create room success
	{
		return "THE ROOM IS CREATED, COMMAND \"STAR\" TO START GAME: ";
	}
	if (strcmp(respond, C_A_QQ_INCORRECT) == 0) //answer quick quizz us incorrect or slower than other user
	{
		return "SORRY, YOU ANSWERED WRONG OR TOO SLOW!";
	}
	
	if (strcmp(respond, C_CRE_ROOM_FAI) == 0) //room is existed, create room become fails
	{
		return "ROOM ID IS EXISTS! CHOOSE ANOTHER ID: ";
	}
	if (strcmp(respond, C_OUT_ROOM) == 0) //can't join to a room
	{
		return "ID IS NOT EXISTS OR THE ROOM IS PLAYING!\n>TRY AGAIN: ";
	}
	if (strcmp(respond, C_YOU_ARE_KEY) == 0) //you are the master of the room
	{
		return "THE MAIN PLAYER LOSES, YOU BECOME THE MASTER OF THE ROOM!\n>ENTER YOUR COMMAND: ";
	}
	if (strcmp(respond, C_WAIT) == 0) //wait for other user start game
	{
		return "MAIN PLAYERS LOSE, WAITING FOR NEW OWNERS START COMMAND ROOM\nCOMMAND \"STAR\" TO START GAME: ";
	}
	if (strcmp(respond, C_LEAV_ROOM_FAI) == 0) //can't leave room
	{
		return "CAN'T LEAVE THIS ROOM!";
	}
	if (strcmp(respond, C_HELP_NOT_OK) == 0) //can't use help feature 
	{
		return "CAN'T USE THIS FEATURE\n>CHOOSE OTHER HELP OR ANSWER NOW: ";
	}
	if (strcmp(respond, C_HELP_ADVISORY_OK) == 0) //wait the advisory from other user
	{
		return "PLEASE WAIT!";
	}
	if (strcmp(respond, C_ALL_ROOM_INCORRECT_K) == 0) //all members of this room answer incorrected
	{
		return "ALL OF MEMBER WRONG\n>TRY AGAIN, COMMAND \"STAR\" TO START GAME: ";
	}
	if (strcmp(respond, C_ALL_ROOM_INCORRECT_M) == 0) //all members of this room answer incorrected, ask user to wait
	{
		return "ALL OF MEMBER WRONG! PLEASE WAIT!\n";
	}
	if (strcmp(respond, C_LOG_SEQ_WRONG) == 0) //wrong sequence
	{
		return "WRONG SEQUENCE!\n>TRY AGAIN: ";
	}
	if (strcmp(respond, C_NOT_ROOM_MASTER) == 0) //the user isn't master
	{
		return "YOU ARE NOT ROOM MASTER!\n>TRY AGAIN: ";
	}
	else
		return respond;
}

//menu of this game
void menu()
{
	printf("*************************************************\n");
	printf("**********         WELCOME       ****************\n");
	printf("*******     WHO IS A MILLIONAIRE     ************\n");
	printf("**********           ???          ***************\n");
	printf("*************************************************\n\n");
	printf(">ENTER YOUR COMMAND: ");
}

//main function
int main(int argc, char const *argv[])
{
	int SERVER_PORT;
	char SERVER_ADDR[MAX];
	int client_sock;
	char buff[BUFF_SIZE + 1],respond[BUFF_SIZE];
	struct sockaddr_in server_addr; 
	int msg_len, bytes_sent, bytes_received, check;
	
	if (argc != 3) exit(1);
	SERVER_PORT = atoi(argv[2]);
	strcpy(SERVER_ADDR, argv[1]);

	//construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);

	//specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	//request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}

	menu();
	while(1){
		strcpy(buff,"");
		memset(buff,'\0',(strlen(buff)+1));
		fgets(buff, BUFF_SIZE, stdin);		
		msg_len = strlen(buff);
		if (msg_len == 1) break;
		
		//message send fails
		if (!request(client_sock, buff)){
			printf("message send fails\n");
		}

		//message receive fails
		if (!receive(client_sock, respond)){
			printf("message receive fails\n");
		} else {
			//success
			printf("*********************************\n");
			printf(">%s", makeFull(respond));
			while (strcmp(respond, C_IN_ROOM) == 0 || strcmp(respond, C_A_QQ_INCORRECT) == 0 
				|| strcmp(respond, C_WAIT) == 0 || strcmp(respond, C_HELP_ADVISORY_OK) == 0 || strcmp(respond, C_ALL_ROOM_INCORRECT_M) == 0)
			{
				bzero(respond,600);
				if (!receive(client_sock, respond)){
					printf("message receive fails\n");
				} else 
					printf("*********************************\n");
					printf(">%s", makeFull(respond));
			}
		}
	}
	
	close(client_sock);
	return 0;
}