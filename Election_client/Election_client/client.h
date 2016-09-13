#include <string>
#include <WinSock2.h>


using namespace std;
/*
 Initializes Winsock, creates a socket to connect to the server,
 retrieves information about the server,
 and tries to establish a connection to the server.
*/
int initialize(SOCKET& connectionSocket, string& ipServer, string& port);

/*
 Randomly chooses a candidate
*/
string randomVote(string list_of_candidates);

/*
*/
int receiveData(SOCKET& connectionSocket, char *dataRecipient, const int& length, const string& errorMessage);

/*
*/
int sendData(SOCKET& connectionSocket, const char *data, const int& length);

/*
*/
void displayListOfCandidates(string list_of_candidates);