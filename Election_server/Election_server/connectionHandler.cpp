#undef UNICODE
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "connectionHandler.h"
#include <vector>

#include <winsock2.h>
#include "winsock_errors.h"

#include "util.h"

// link with Ws2_32.lib
#pragma comment( lib, "ws2_32.lib" )

using namespace std;


// An external function to handle the data exchanged between the ElectionServer and the client.
extern DWORD WINAPI handleIncomingData(void* sd_);
extern DWORD WINAPI handleTime(void* sd_);
extern DWORD WINAPI handleConnection(void* ServerSocket);

/* -------------------------------------------------------------------------------- */

void ConnectionHandler::printErrorMessage(string errorMessage)
{
	mtx.lock();
	cerr << WSAGetLastErrorMessage(errorMessage.c_str()) << endl;
	mtx.unlock();
}

void ConnectionHandler::printMessage(string errorMessage)
{
	mtx.lock();
	cout << errorMessage << endl;
	mtx.unlock();
}

/* -------------------------------------------------------------------------------- */

ElectionServer *ConnectionHandler::appServer = new ElectionServer();

ConnectionHandler *ConnectionHandler::instance_ = nullptr;

Time *ConnectionHandler::start_time = new Time();
Time *ConnectionHandler::end_time = new Time();

bool ConnectionHandler::isTime2Vote_ = false;
bool ConnectionHandler::isEndOfVote_ = false;

mutex ConnectionHandler::mtx;


char* ConnectionHandler::extractHost()
{
	char *host = new char[80];
	if (gethostname(host, 80 * 8) == SOCKET_ERROR) {
		cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << std::endl;
		cout << "Entrer l'identifiant du poste ou son adresse Ip";
		cin >> host;
	}
	return host;
}

int ConnectionHandler::initialize()
{
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		std::cerr << "Error at WSAStartup()\n" << std::endl;
		return 1;
	}
	return 0;

}

int ConnectionHandler::handleIncomingRequests(vector<int> ports)
{

	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.

	vector<SOCKET> socketsListen;
	for (int port : ports)
	{
		SOCKET ServerSocket;
		ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (ServerSocket == INVALID_SOCKET) {
			printErrorMessage("Error at socket()");
			WSACleanup();
			return 1;
		}
		char* option = "1";
		setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, option, sizeof(option));

		socketsListen.push_back(ServerSocket);
	}
	

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.

	//Recuperation de l'adresse locale
	hostent *thisHost;
	thisHost = gethostbyname(extractHost());
	char* ip;
	ip = inet_ntoa(*reinterpret_cast<struct in_addr*>(*thisHost->h_addr_list));
	printMessage("Adresse locale trouvee "+string(ip)+" : \n");

	vector<int> effectivelyListenedports;
	for (size_t p = 0; p < ports.size(); p++)
	{
		SOCKET ServerSocket = socketsListen.at(p);
		int port = ports.at(p);

		sockaddr_in service;
		service.sin_family = AF_INET;

		service.sin_addr.s_addr = inet_addr(ip);
		service.sin_port = htons(port);

		if (::bind(ServerSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
			printErrorMessage("bind() failed.");
			closesocket(ServerSocket);
			WSACleanup();
			return 1;
		}

		//----------------------
		// Listen for incoming connection requests.
		// on the created socket
		if (listen(ServerSocket, 30) == SOCKET_ERROR) {
			printErrorMessage("Error listening on socket.");
			closesocket(ServerSocket);
			WSACleanup();
			return 1;
		}

		effectivelyListenedports.push_back(ntohs(service.sin_port));
		//printf("En attente des connections des clients sur le port %d...\n\n", ntohs(service.sin_port));
	}

	if (effectivelyListenedports.size()>0)
		printMessage("Wainting for connection requests from clients on ports "+listOfListenedPorts(effectivelyListenedports)+"\n");

	// ...
	DWORD timeThread;
	SOCKET timer = NULL;
	HANDLE time_handle = CreateThread(nullptr, 0, handleTime, reinterpret_cast<void*>(timer), 0, &timeThread);


	
	// Creating a thread to handle connections on each and every port.
	HANDLE* threadsHandles = new HANDLE[socketsListen.size()];
	int i = 0;
	for (SOCKET ServerSocket : socketsListen)
	{
		DWORD nThreadID;
		threadsHandles[i] = CreateThread(nullptr, 0, handleConnection, reinterpret_cast<void*>(ServerSocket), 0, &nThreadID);
		i++;
	}
	
	/*
	// Makes the main program wait until all connections are severed before going further in the execution.
	DWORD ncount = socketsListen.size();
	WaitForMultipleObjects(ncount, threadsHandles, true, INFINITE);*/

	// Makes the main program wait until the voting time has elapsed, thus all connections are severed before going further in the execution.
	WaitForSingleObject(time_handle, INFINITE);
	

	// No longer need ElectionServer socket
	for (SOCKET ServerSocket : socketsListen)
		closesocket(ServerSocket);

	WSACleanup();
	return 0;
}

ConnectionHandler& ConnectionHandler::Instance()
{
	if (instance_ == nullptr)
		instance_ = new ConnectionHandler();
	return *instance_;
}

void ConnectionHandler::checkTime()
{
	Time current_time;
	current_time.toCurrentTime();
	if ((current_time >= getStartTime()) && (current_time <= getEndTime()))
		startTheVote();
	else
		endTheVote();
}

void ConnectionHandler::startTheVote()
{
	isTime2Vote_ = true;		
}

void ConnectionHandler::endTheVote()
{
	isTime2Vote_ = false;
	isEndOfVote_ = true;
}


/* --------------------------------------------------------------------- */

DWORD WINAPI handleConnection(void* ServerSocket)
{
	while (!ConnectionHandler::isEndOfVote()) {

		
		sockaddr_in sinRemote;
		int nAddrSize = sizeof(sinRemote);
		// Create a SOCKET for accepting incoming requests.
		// Accept the connection.
		SOCKET sd = accept(reinterpret_cast<SOCKET>(ServerSocket), reinterpret_cast<sockaddr*>(&sinRemote), &nAddrSize);

		if (ConnectionHandler::isTime2Vote())
		{
			if (sd != INVALID_SOCKET) {
				ConnectionHandler::printMessage("Connection acceptee De : " +
					string(inet_ntoa(sinRemote.sin_addr)) + ":" +
					to_string(ntohs(sinRemote.sin_port)) + ".");
			
				DWORD nThreadID;
				CreateThread(nullptr, 0, handleIncomingData, reinterpret_cast<void*>(sd), 0, &nThreadID);
			
			}
			else {
				ConnectionHandler::printErrorMessage("Echec d'une connection.");
				// return 1;
			}
		}
		
		

	}

	return 0;
}

DWORD WINAPI handleIncomingData(void* sd_)
{
	SOCKET sd = reinterpret_cast<SOCKET>(sd_);
	
	size_t max_name_length = 0; // The maximum length of a candidate's name.

	// First turn the list of candidates into a string
	string list_of_candidates;
	for (string candidate : ConnectionHandler::getAppServer().getCandidates())
	{
		list_of_candidates.append(candidate+";");
		max_name_length = max(candidate.size(), max_name_length);
	}
	

	// Send the size of the string list_of_candidates to the client
	string len;
	for (auto c : to_string(list_of_candidates.size()))
		len.append(1, c);

	char* buf = const_cast<char*>(len.c_str());
	send(sd, buf, 10, 0);
	ConnectionHandler::printMessage("\nSize of list_of_candidates sent.");

	// Send list of candidates to the client
	char* buffer = const_cast<char*>(list_of_candidates.c_str());
	send(sd, buffer, list_of_candidates.size(), 0);
	ConnectionHandler::printMessage("\nList of candidates sent.");

	// Read data from client
	char *readBuffer = new char[100];
	//char *readBuffer = new char[max_name_length];
	int readBytes;

	readBytes = recv(sd, readBuffer, 100, 0);
	//readBytes = recv(sd, readBuffer, max_name_length, 0);
	if (readBytes > 0)
	{
		//ConnectionHandler::printMessage("\nReceived a vote from a client.");
		//ConnectionHandler::printMessage("\nReceived : " + string(readBuffer));

		//--------------------------------------
		string vote = "";
		for (auto c : string(readBuffer))
		{
			 vote.append(1, toupper(c));
		}
		//--------------------------------------
		ConnectionHandler::printMessage("\nReceived : " + vote);
		ConnectionHandler::getAppServer().processVote(vote);
		ConnectionHandler::printMessage("\nVote counted.");
	}
	else if (readBytes == SOCKET_ERROR) {
		ConnectionHandler::printMessage(WSAGetLastErrorMessage("Echec de la reception !"));
	}
	closesocket(sd);

	return 0;
}



DWORD WINAPI handleTime(void* sd_)
{
	Time current_time;
	current_time.toCurrentTime(); // Get current time

	while (current_time <= ConnectionHandler::getEndTime()) // Until we reach the end of the vote period ...
	{
		if (current_time >= ConnectionHandler::getStartTime() && !ConnectionHandler::isTime2Vote())
		{
			ConnectionHandler::startTheVote(); // If we are within the voting time period, we start the vote.
			ConnectionHandler::printMessage("The election has started at " + current_time.toString());
		}
			
		
		current_time.toCurrentTime(); // Get current time
	}
	
	ConnectionHandler::endTheVote(); // When we reach the end of the vote period, the vote stops.
	ConnectionHandler::printMessage("The election has ended at " + current_time.toString());

	return 0;
}



