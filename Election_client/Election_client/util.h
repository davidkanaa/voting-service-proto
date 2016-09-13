#include <string>
#include <vector>
#include <sstream>


// Splits  a string.
inline vector<string> split(string str, char delimiter)
{
	istringstream split(str);
	vector<string> tokens;
	for (string each; getline(split, each, delimiter); tokens.push_back(each));

	tokens.pop_back();
	return tokens;
}

