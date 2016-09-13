#include "client.h"
#include <iostream>

int main(void)
{
	SOCKET connectionSocket;
	cout << "Welcome to the set up of your election client."<< endl << endl << endl;

	string ipServer, connectionPort;
	cout << "Please, enter the IP address of the server." << endl, getline(cin, ipServer);
	cout << "Please, enter the connection port." << endl, getline(cin, connectionPort);

	// inializing the client :: elector
	if ( initialize(connectionSocket, ipServer, connectionPort) == 1 )
		return 1;

	system("cls");
	cout << "Well done. Your election server has been successfully set up." << endl << endl;
	cout << "Welcome !\nThank you for taking part in this election." << endl << endl << endl;

	// Receive the length of the string representing the list of candidates.
	char len[10];
	if (receiveData(connectionSocket, len, 10, "Error : didn't receive the length of the string representing the list of candidates.") == 1)
		return 1;

	// Receive the list of candidates
	char* list = new char[atoi(len)];
	if (receiveData(connectionSocket, list, atoi(len), "Error : didn't receive the list of candidates.") == 1)
		return 1;

	// Display the list of candidates
	displayListOfCandidates(string(list));
	cout << endl << endl;

	// Voting ...
	string vote;

	/************************************************************************************
				UNCOMMENT THIS FOR HUMAN VOTE ONLY, NOT MACHINE RANDOM VOTE
	-------------------------------------------------------------------------------------

	cout << "Express your vote by typing the name of your favourite candidate. " << endl;
	cout << "(The case does not matter you can type in capital letters or not)" << endl;
	getline(cin, vote);

	**************************************************************************************/
	
	/************************************************************************************
				UNCOMMENT THIS FOR MACHINE RANDOM VOTE ONLY, NOT HUMAN VOTE
	-------------------------------------------------------------------------------------*/
	vote = randomVote(list);
	/**************************************************************************************/

	// Sending vote to the server.
	if (sendData(connectionSocket, vote.c_str(), 100) == 1)
		return 1;
	

	system("cls");
	cout << "Your vote has been counted." << endl;
	cout << "Thank you once again for taking part in this election." << endl;



	closesocket(connectionSocket);
	WSACleanup();
	cout << "\n\nPress any key to finish ... " << endl;
	getchar();

	return 0;
}