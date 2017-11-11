#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <tuple>

using namespace std;

vector<string> parseASes(string AS);

int main()
{
	string IP_prefix, prefix_length, AS;

	vector<string> my_ASes;
	map<string, vector< tuple<string, string>> > my_map;

	ofstream outfile;
	ifstream infile("routeviews_prefix2as.txt");
	outfile.open("part2_2_3.txt");

	for (string line; getline(infile, line);)	// Pass through all lines of code 
	{
		istringstream iss(line);
		iss >> IP_prefix;
		iss >> prefix_length;
		iss >> AS;

		my_ASes = parseASes(AS);

		for (int i = 0; i < my_ASes.size(); ++i)
		{
			auto it = my_map.find(my_ASes[i]);

			if (it != my_map.end())
			{
				tuple<string, string> new_tuple = make_tuple(IP_prefix, prefix_length);
				vector<tuple<string, string>> &curVec = it->second;
				curVec.push_back(new_tuple);
			}
			else
			{
				vector<tuple<string, string>> my_vector;
				tuple<string, string> new_tuple = make_tuple(IP_prefix, prefix_length);
				my_vector.push_back(new_tuple);
				my_map[my_ASes[i]] = my_vector;
			}
		}		
	}


	for (auto it = my_map.begin(); it != my_map.end(); ++it)
	{
		string output = it->first;
		int vecSize = it->second.size();
		output += "\t" + to_string(vecSize);
		/*
		for (int i = 0; i < it->second.size(); ++i)
		{
			output += "\t" + get<0>(it->second[i]);
		}
		*/
		outfile << output << endl;
	}

	infile.close();
	outfile.close();

	return 0;
}

vector<string> parseASes(string AS)
{
	vector<string> ASesVec;
	string tempAS;

	for (int i = 0; i < AS.length(); ++i)
	{
		if (isdigit(AS[i]))
		{
			tempAS += AS[i];
		}
		else
		{
			ASesVec.push_back(tempAS);
			tempAS = "";
		}
	}

	ASesVec.push_back(tempAS);

	return ASesVec;
};