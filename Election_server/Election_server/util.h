#include <string>
#include <vector>
#include <sstream>


// Splits  a string.
inline vector<string> split(string str, char delimiter)
{
	istringstream split(str);
	vector<string> tokens;
	for (string each; getline(split, each, delimiter); tokens.push_back(each));

	return tokens;
}

inline void setUpDate(Time& time)
{
	string date;
	getline(cin, date);
	//cin >> date;
	vector<string> dateFields;
	for (string s : split(date, '/'))
		dateFields.push_back(s);

	// set Year
	if (dateFields.size() >= 1)
		time.setYear(atoi(dateFields.at(0).c_str()));

	// set Month
	if (dateFields.size() >= 2)
		time.setMonth(atoi(dateFields.at(1).c_str()));

	// set Day
	if (dateFields.size() >= 3)
		time.setDay(atoi(dateFields.at(2).c_str()));
}

inline void setUpSchedule(Time& time)
{
	string schedule;
	getline(cin, schedule);
	//cin >> schedule;
	vector<string> scheduleFields;
	for (string s : split(schedule, ':'))
		scheduleFields.push_back(s);

	// set Hours
	if (scheduleFields.size() >= 1)
		time.setHour(atoi(scheduleFields.at(0).c_str()));

	// set Minutes
	if (scheduleFields.size() >= 2)
		time.setMinutes(atoi(scheduleFields.at(1).c_str()));

	// set Seconds
	if (scheduleFields.size() >= 3)
		time.setSeconds(atoi(scheduleFields.at(2).c_str()));
}

inline string listOfListenedPorts(vector<int> ports)
{
	string list = to_string(ports.at(0)) + " to ";

	if (ports.size() >= 3){

		for (auto index = 1; index < ports.size()-1; ++index)
		{
			if (ports.at(index + 1) != ports.at(index) + 1)
			{
				list.append(to_string(ports.at(index)) + ", " + to_string(ports.at(index + 1)) + " to ");
			}
		}
		list.append(to_string(ports.back()));
	}
	else if (ports.size() == 2)
	{
		list.append(to_string(ports.at(1)));
	}


	return list;
}