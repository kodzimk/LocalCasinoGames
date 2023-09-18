#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<string>
#include<WS2tcpip.h>
#include<iostream>

#pragma warning(disable:4996)

SOCKET ConnectSocket;
char message[522];
bool isInviteUser;
bool gamestart;
bool waitanswer;

void ClientLogic()
{
	char buffer[512];
	char msg[522];

	while (true) {
		int result = 0;
		int result2 = 0;
		recv(ConnectSocket, msg, sizeof(msg), 0);
		recv(ConnectSocket, buffer, sizeof(buffer), 0);

		if (buffer[0] == 'G' && gamestart == true)
		{
			std::cout << "UserName: " << msg << " Message: " << buffer << std::endl;
			isInviteUser = false;
			waitanswer = false;
			gamestart = false;
		}
		else if (buffer[0] == 'G' && msg[0] == 'S')
		{
			gamestart = true;
			std::cout << "UserName: " << msg << " Message: " << buffer << std::endl;
		}
		else if (buffer[0] == '!')
		{
			std::cout << "Warning: " << buffer << " Username: " << msg << std::endl;
		}
		else if (buffer[0] == '?')
		{
			std::cout << "UserName: " << msg << " Invite you to join a game " << buffer << std::endl;
			std::cout << "Enter you answer: ";
		}
		else if (isInviteUser == false && buffer[0] >= '0' && buffer[0] <= '9' && gamestart == true)
		{
			std::cout << "Enter your answer aftr entr click any button or keyboard button to sending answer: ";
			std::cin.getline(buffer, 512);
			send(ConnectSocket, message, sizeof(message), 0);
			send(ConnectSocket, buffer, sizeof(buffer), 0);
			gamestart = false;
		}
		else if (gamestart == false) {
			std::cout << "UserName: " << msg << " Reciving message: " << buffer << std::endl;
		}


	}
}
void ClientPrecieve()
{
	ClientLogic();
}



int main(int argc, char* argv[])
{
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;


	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		exit(1);
	}

	ConnectSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(ConnectSocket, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		std::cout << "Error! failed connect to server\n";
		exit(1);
	}
	else {
		std::cout << "Connected to game!\n";
	}

	std::cin.getline(message, 522);
	std::cout << "Your name: " << message << std::endl;

	char buffer[512];
	recv(ConnectSocket, buffer, sizeof(buffer), NULL);
	std::cout << buffer << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientPrecieve, NULL, NULL, NULL);



	std::cout << "Enter 'start' to start a game\n";
	std::cout << "Enter any message to send (only if you is in not a game)\n";
	std::cout << "Enter 'play' to start a game with other after all players will give answer\n";
	std::cout << "Enter 'balance' to see your balance\n";

	do {

		ZeroMemory(buffer, 512);
		if (isInviteUser == true && waitanswer == false && gamestart == true)
		{
			std::cout << "Enter number to find: ";
			std::cin.getline(buffer, 512);
			send(ConnectSocket, message, sizeof(message), 0);
			send(ConnectSocket, buffer, sizeof(buffer), 0);
			waitanswer = true;
		}
		else if (gamestart == false)
		{
			std::cin.getline(buffer, 512);
			if (buffer == "start")
			{
				isInviteUser = true;
				send(ConnectSocket, message, sizeof(message), NULL);
				send(ConnectSocket, buffer, sizeof(buffer), NULL);
			}
			else {
				send(ConnectSocket, message, sizeof(message), NULL);
				send(ConnectSocket, buffer, sizeof(buffer), NULL);
			}
		}

	} while (true);



	return 0;
}