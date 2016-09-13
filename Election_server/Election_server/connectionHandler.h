#include "server.h"
#include "Time.h"
#include <mutex>


/**
<summary> A ConnectionHandler is an object responsible for the management of server-client interactions on the server-side. </summary>
*/

class ConnectionHandler
{
	private:
		static ElectionServer *appServer;
		static ConnectionHandler *instance_; // The unique instance of the connection handler.
		ConnectionHandler(){};

		// ...
		static Time *start_time;
		static Time *end_time;

		/* The state attribute : it is true when we are within the vote periode and false otherwise.
		When false, the connection handler allows no connection requests from the client or if there is any active connection, it is severed. */
		static bool isTime2Vote_;
		static bool isEndOfVote_;

		char* extractHost(); // retreives the host IPv4 active address.

		static mutex mtx;

	public:		
		~ConnectionHandler(){};

		int initialize(); // Intialize Winsock.
		int handleIncomingRequests(vector<int> ports); // Manage the connection between the ElectionServer and the client.

		static ElectionServer& getAppServer(){ return *appServer; };
		void setAppServer(ElectionServer &server_){ appServer = &server_; };


		static ConnectionHandler& Instance(); // Returns the unique instance of the connection handler.
		static bool isTime2Vote(){ return isTime2Vote_; }; // Returns the current state of the vote schedule : true if we still are within the vote period, false otherwise.
		static bool isEndOfVote(){ return isEndOfVote_; };

		// -------------------
		void setStartTime(const Time &start_){ *start_time = start_; };
		void setEndTime(const Time &end_){ *end_time = end_; };

		static const Time& getStartTime(){ return *start_time; };
		static const Time& getEndTime(){ return *end_time; };

		// ---
		static void checkTime(); // checks if we are within the vote period or not and updates the state attribute instance_ accordingly.
		static void startTheVote();
		static void endTheVote();

		// ---
		
		static mutex& _mutex(){ return mtx; };
		static void printErrorMessage(string errorMessage);
		static void printMessage(string errorMessage);
};