#include "server.h"
#include <iostream>
#include <fstream>
#include <string>


ElectionServer::ElectionServer()
{
	// construction
}

ElectionServer::~ElectionServer()
{
	// destruction
}

void ElectionServer::loadCandidatesList()
{
	string line;
	ifstream candidatesListFile("candidates_list.txt"); // open the file containing the list of candidates to the election.
	if (candidatesListFile.is_open())
	{
		// read the file line by line to retrieve the list of candidates
		while (getline(candidatesListFile, line))
		{
			string name;
			for (auto c : line)
			{
				name.append(1, toupper(c));
			}
			candidates.push_back(name); // add the name (supposed to be the candidates name) to the list of candidates.
			results.insert(pair<string, int>(name, 0)); // add the name (supposed to be the candidates name) to the results table.
		}
		candidatesListFile.close(); // close the file when done!
	}
	else
	{
		ofstream file;
		file.open("candidates_list.txt");
		file.close();
	}
}

void ElectionServer::processVote(string candidateName)
{
	map<string, int>::iterator i = results.find(candidateName);
	if (i != results.end()){
		(*i).second++;
	}
}



