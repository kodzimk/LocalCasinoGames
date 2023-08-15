#define WIN32_LEAN_AND_MEAN

#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<algorithm>
#include<string>
#pragma warning(disable: 4996)
#include<WS2tcpip.h>

SOCKET Connections[100];
int Counter;
bool isHaveGame = false;
bool isConfirm[100];
int invitingUserIndex = -1;
int giveAnswerPlayerCount = 0;
bool startgame;
std::string finNumber;
int Count;
int eachusermoney[100];


void ServerLogic(int index)
{
	char buffer[522];
	char msg[512];

	while (true)
	{
		
		int result = 0;
		int result2 = 0;

		result = recv(Connections[index], buffer, sizeof(buffer), NULL);
		result2 = recv(Connections[index], msg, sizeof(msg), NULL);

		if (Count == giveAnswerPlayerCount&& startgame == true)
		{
			char message[522] = "Server";
			char mess[512] = "Game succefuly ended";
			send(Connections[invitingUserIndex], message, sizeof(message), NULL);
			send(Connections[invitingUserIndex], mess, sizeof(mess), NULL);
			isHaveGame = false;
			invitingUserIndex = -1;
			finNumber = "";
			Count = 0;
			giveAnswerPlayerCount = 0;
			startgame = false;
			for (int i = 0; i < 100; i++)
			{
				isConfirm[i] = false;
		    }
		}
		else if (msg[0] == 'b' && msg[1] == 'a' && msg[2] == 'l')
		{
			std::string message = std::to_string(eachusermoney[index]);
			char message2[512];
			strcpy(message2, message.c_str());
			char name[522] = "Server";
			message[0] = (char)eachusermoney[index];
			send(Connections[index], name, 522, 0);
			send(Connections[index], message2, 512, 0);
		}
		else if (index!= invitingUserIndex && result > 0 && startgame == true)
		{
			
			if (finNumber[0] == msg[0])
			{
				eachusermoney[index] += 200;
				char message[512] = "Correct you win 200$ for your balance enter 'balance' to see how much money do you have";
				char name[522] = "Server";
				send(Connections[index], name, 522, 0);
				send(Connections[index], message, sizeof(message), NULL);
				Count++;
			}
			else {
				Count++;
				char message[512] = "Wrong";
				char name[522] = "Server";
				send(Connections[index], name, 522, 0);
				send(Connections[index], message, sizeof(message), NULL);
			}
		}
		else if  (giveAnswerPlayerCount == Counter-1&& msg[0] == 'p'&&startgame==false&&isHaveGame==true)
		{
			char name[522] = "Server";
			char message[512] = "Game Started";
			giveAnswerPlayerCount = 0;
			for (int i = 0; i < Counter; i++)
			{
				if (isConfirm[i] == true)
				{
					giveAnswerPlayerCount++;
					send(Connections[i], name, 522, 0);
					send(Connections[i], message, sizeof(message), NULL);
				}
			}
			send(Connections[invitingUserIndex], name, 522, 0);
			send(Connections[invitingUserIndex], message, sizeof(message), NULL);
			startgame = true;
		}
		else if (startgame == true&&finNumber=="")
		{
			
				char name[522] = "Server";
				for (int i = 0; i < Counter; i++)
				{
					if (isConfirm[i] == true)
					{
						send(Connections[i], name, 522, 0);
						send(Connections[i], msg, sizeof(msg), NULL);
					}
				}
				finNumber[0] = msg[0];
			}	
		else if (invitingUserIndex != -1 && startgame==false&&isHaveGame==true)
		{	
			if (msg[0] == '0')
			{
				char message[512] = "Reject";
				giveAnswerPlayerCount++;
				send(Connections[invitingUserIndex], buffer, sizeof(buffer), NULL);
				send(Connections[invitingUserIndex], message, sizeof(message), NULL);
			}
			if (msg[0] == '1')
			{
				if (eachusermoney[index] >= 100)
				{
					eachusermoney[index] -= 100;
					isConfirm[index] = true;
					giveAnswerPlayerCount++;
					char message[512] = "Confirm";
					send(Connections[invitingUserIndex], buffer, sizeof(buffer), NULL);
					send(Connections[invitingUserIndex], message, sizeof(message), NULL);
					
					
				}
				else {
					char message[512] = "Reject";
					giveAnswerPlayerCount++;
					send(Connections[invitingUserIndex], buffer, sizeof(buffer), NULL);
					send(Connections[invitingUserIndex], message, sizeof(message), NULL);
					char mess[512] = "You dont have money";
					send(Connections[index], buffer, sizeof(buffer), NULL);
					send(Connections[index], mess, sizeof(mess), NULL);
				}
			}
		}
		else if (isHaveGame == false  && msg[0] == 's'&&msg[1]=='t')
		{

			invitingUserIndex = index;
			char message[512] = "? Click 1 to join game or 0 to refuse";

			for (int i = 0; i < Counter; i++)
			{
				if (i == index)continue;


				send(Connections[i], buffer, sizeof(buffer), NULL);
				send(Connections[i], message, sizeof(message), NULL);
			}

			isHaveGame = true;
		}
		else if (isHaveGame == true  && msg=="start")
	    {
		
			char message[512] = "! you Cant create a game becauce,In server already have a game";
			for (int i = 0; i < Counter; i++)
			{
				if (i != index)continue;


				send(Connections[i], buffer, sizeof(buffer), NULL);
				send(Connections[i], message, sizeof(message), NULL);
			}
		}
		else if(startgame==false)
		{
			for (int i = 0; i < Counter; i++)
			{
				if (i == index)continue;

				send(Connections[i], buffer, sizeof(buffer), 0);
				send(Connections[i], msg, sizeof(msg), 0);
			}
		}	
	}
}

void ClientPrecieve(int index)
{
	ServerLogic(index);
}

int main(int argc, char* argv[])
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		if (newConnection == 0)
		{
			std::cout << "Error 2#\n";
		}
		else {
			std::cout << "Client Connected!\n";
			char msg[256] = "Hello to the game Your Balance 200$";
			eachusermoney[i] = 200;
			send(newConnection, msg, sizeof(msg), NULL);
			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientPrecieve, (LPVOID)(i), NULL, NULL);

		}
	}

	system("pause");
	return 0;
}