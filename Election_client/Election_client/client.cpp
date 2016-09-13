#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "client.h"
#include <ws2tcpip.h>
#include <iostream>
#include <iterator>
#include <time.h>
#include "util.h"

// Link avec ws2_32.lib
#pragma comment(lib, "ws2_32.lib")



int initialize(SOCKET& connectionSocket, string& ipServer, string& port)
{
	WSADATA wsaData;
	int iResult;

	struct addrinfo *result = nullptr,	hints;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "WSAStartup Error : "  << endl << iResult << endl;
		return 1;
	}


	// Create a socket to communicate with the server
	connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connectionSocket == INVALID_SOCKET) {
		cout << "Function socket() error: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		cout << "Press any key to finish ... " << endl;
		getchar();
		return 1;
	}


	// Search for the address of the server using the function getaddrinfo.
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;        // Address family
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  // Protocol used by the server



	// getaddrinfo gets the IP address of the given host
	iResult = getaddrinfo(ipServer.c_str(), port.c_str(), &hints, &result);
	if (iResult != 0) {
		cout << "Erreur de getaddrinfo : "<< iResult << endl;
		WSACleanup();
		return 1;
	}


	// Search for the first IPV4 address
	while ((result != nullptr) && (result->ai_family != AF_INET))
		result = result->ai_next;


	//-----------------------------------------

	if (((result == nullptr) || (result->ai_family != AF_INET))) {
		freeaddrinfo(result);
		cout << "Cannot retrieve the good address." << endl << endl;
		WSACleanup();
		cout << "Press any key to finish ... " << endl;
		getchar();
		return 1;
	}

	sockaddr_in *adresse;
	adresse = reinterpret_cast<struct sockaddr_in *>(result->ai_addr);

	//----------------------------------------------------

	cout << "Server address found " << ipServer << " : " << inet_ntoa(adresse->sin_addr) << endl << endl;
	cout << "Connection attempt" << inet_ntoa(adresse->sin_addr) << " on the port " << port << endl << endl;


	// On va se connecter au serveur en utilisant l'adresse qui se trouve dans
	// la variable result.
	iResult = connect(connectionSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		cout << "Cannot connect to server " << inet_ntoa(adresse->sin_addr) << " on the port " << port << endl << endl;
		freeaddrinfo(result);
		WSACleanup();
		cout << "Press any key to finish ... " << endl;
		getchar();
		return 1;
	}

	cout << "Connected to server " << ipServer << ":" << port << endl << endl;
	freeaddrinfo(result);

	return 0;
}

// Randomly chooses a candidate
string randomVote(string list_of_candidates)
{
	vector<string> list = split(list_of_candidates, ';');
	
	// Choose randomly a candidate's index
	srand(time(nullptr));
	int index = rand() % list.size();

	return list.at(index);
}

int receiveData(SOCKET& connectionSocket, char* dataRecipient, const int& length, const string& errorMessage)
{
	int iResult = recv(connectionSocket, dataRecipient, length, 0);
	if (!(iResult > 0)){
		cout << errorMessage << WSAGetLastError() << endl;
		return 1;
	}
	return 0;
}

int sendData(SOCKET& connectionSocket, const char* data, const int& length)
{
	int iResult = send(connectionSocket, data, length, 0);
	if (iResult == SOCKET_ERROR) {
		cout << "Error while sendind:" << WSAGetLastError() << endl;
		closesocket(connectionSocket);
		WSACleanup();
		cout << "Press any key to finish ... " << endl;
		getchar();
		return 1;
	}
	return 0;
}

void displayListOfCandidates(string list_of_candidates)
{
	for (string name : split(list_of_candidates, ';')){
		cout << name << endl;
	}

}