#include "connectionHandler.h"
#include <iostream>
#include "util.h"

using namespace std;

int main(void)
{
	// Create an election server.
	ElectionServer app_server;


	// Load Candidates' names.
	app_server.loadCandidatesList();



	// Create a connection handler.
	ConnectionHandler manager = ConnectionHandler::Instance();



	// Set the app server election logic.
	manager.setAppServer(app_server);

	cout << "Welcome to the set up of your election server.\n\n" << endl;
	string entry;

	/* --------------------------
	 UNCOMMENT TO SET UP THE ELECTION SCHEDULE.*/

	// Set the vote starting time.
	Time start;

	cout << "\nPlease, enter the date at which the election must start." << endl
		<< "-- The format is YYYY/MM/DD --" << endl;
	setUpDate(start);

	cout << "\nPlease, enter the time at which the election must start." << endl
		<< "-- The format is HH:mm:ss --" << endl;
	setUpSchedule(start);


	manager.setStartTime(start);



	// Set the vote ending time.
	Time end;

	cout << "\nPlease, enter the date at which the election must end." << endl
		<< "-- The format is YYYY/MM/DD --" << endl;
	setUpDate(end);

	cout << "\nPlease, enter the time at which the election must end." << endl
		<< "-- The format is HH:mm:ss --" << endl;
	setUpSchedule(end);

	manager.setEndTime(end);
	


	system("cls");
	cout << "Well done. Your election server has been successfully set up.\n" << endl;
	cout << "Beginning of the election : " << start;
	cout << "Ending of the election : " << end << endl;




	// Initialize.
	if (manager.initialize() == 1)
		return 1;
	
	// Handle connections on ports from 5000 to 5050.
	vector<int> ports;
	for (auto port = 5000; port < 5051; port++)
		ports.push_back(port);

	if (manager.handleIncomingRequests(ports) == 1)
		return 1;




	// Publishing the votes.
	system("cls");
	manager.printMessage("\nHere are the results.");

	map<string, int> results = app_server.getResults();
	for (map<string, int>::iterator it = results.begin(); it != results.end(); ++it)
	{
		manager.printMessage(it->first + " : " + to_string(it->second) + " vote(s).");
	}
	
	system("pause");

	return 0;
}