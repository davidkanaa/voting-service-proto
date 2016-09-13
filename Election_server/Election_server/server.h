#include <vector>
#include <map>

/**
<summary> The class ElectionServer is responsible for the application logic. It holds the methods related to the voting and election process. </summary>
*/

using namespace std;

class ElectionServer
{
	private:
		vector<string> candidates; // The list of candidates for the election.

		/* The table of results. A map of candidates' names associated with
			the number of votes they got. */
		map<string, int> results;

	protected:
	public:
		ElectionServer(); // Constructor
		~ElectionServer(); // Destructor

		const vector<string>& getCandidates(){ return candidates; };
		const map<string, int>& getResults(){ return this->results; };

		void loadCandidatesList(); // Loads the list of candidates from a file.

		void processVote(string candidateName); // Counts the votes.

};