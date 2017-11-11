#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <tuple>
#include <queue>

using namespace std;

int cnt_Transit(map<string, vector< tuple<string, string>> > my_map, map<string, int> node_degree);
int cnt_content(map<string, vector< tuple<string, string>> > my_map, map<string, int> node_degree);
int cnt_Enterprise(map<string, vector< tuple<string, string>> > my_map, map<string, int> node_degree);
void linkNodes(map<string, int> node_degree, map<string, vector< tuple<string, string>> > my_map);
void printDegree(map<string, int> node_degree);

ofstream outfile;
ifstream NodeDegreeFile("nodeDegree.txt");
//#define TEST 0;

int main()
{
	string frst, sec, thr, frth;

	map<string, vector< tuple<string, string>> > my_map;
	map<string, int> node_degree;

#ifdef TEST
	ifstream infile("testFile.txt");
	outfile.open("testOutput.txt");
#else
	ifstream infile("20170901_as_rel2.txt");
	outfile.open("part2_2_2.txt");
#endif
	bool customerExist;

	int enterprise, content, transit;

	for (string line; getline(infile, line);)	// Pass through all lines of code 
	{
		istringstream iss(line);
		iss >> frst;
		iss >> sec;
		iss >> thr;
		iss >> frth;

		auto it = my_map.find(frst);
		//First
		if (it != my_map.end()) // Present in Map
		{
			tuple<string, string> new_tuple = make_tuple(sec, thr);
			vector<tuple<string, string>> &curVec = it->second;
			curVec.push_back(new_tuple);
			node_degree[frst]++;
		}
		else  // Not Present in Map
		{
			vector<tuple<string, string>> my_vector;
			tuple<string, string> new_tuple = make_tuple(sec, thr);
			my_vector.push_back(new_tuple);
			my_map[frst] = my_vector;
			node_degree[frst]++;
		}

		//Second
		it = my_map.find(sec);
		if (it != my_map.end())		// EXISTS
		{
			if (thr.compare("-1") == 0) //customer
			{
				node_degree[sec]++;
			}
			else
			{
				tuple<string, string> new_tuple = make_tuple(frst, thr);
				vector<tuple<string, string>> &curVec = it->second;
				curVec.push_back(new_tuple);
				node_degree[sec]++;
			}

		}
		else		// DOESN"T EXISTS
		{
			if (thr.compare("-1") == 0) // customer
			{
				vector<tuple<string, string>> my_vector;
				my_map[sec] = my_vector;
			}
			else
			{
				vector<tuple<string, string>> my_vector;
				tuple<string, string> new_tuple = make_tuple(frst, thr);
				my_vector.push_back(new_tuple);
				my_map[sec] = my_vector;
			}
			node_degree[sec]++;;
		}
	}

	//printDegree(node_degree);
	/*
	transit = cnt_Transit(my_map, node_degree);
	content = cnt_content(my_map, node_degree);
	enterprise = cnt_Enterprise(my_map, node_degree);

	cout << "Transit: " << transit << endl;
	cout << "Content: " << content << endl;
	cout << "Enterprise: " << enterprise << endl;
	*/
	linkNodes(node_degree, my_map);

	infile.close();
	outfile.close();

	system("pause");
	return 0;
}

void linkNodes(map<string, int> node_degree, map<string, vector< tuple<string, string>> > my_map)
{
	queue <string> sortedNodes;
	string node;
	bool broken = false;
	bool found = false;
	vector<string> S;

	for (string line; getline(NodeDegreeFile, line);)	// Pass through all lines of code 
	{
		istringstream iss(line);
		iss >> node;
		sortedNodes.push(node);
	}

	string currentNode = sortedNodes.front();
	outfile << currentNode << endl;
	S.push_back(currentNode);
	sortedNodes.pop();

	string nextNode = sortedNodes.front();
	sortedNodes.pop();

	while (!broken)
	{
		broken = true;
		for (int i = 0; i < S.size(); ++i)
		{
			found = false;
			auto it = my_map.find(S[i]);
			for (int i = 0; i < it->second.size(); ++i)
			{
				if (get<0>(it->second[i]).compare(nextNode) == 0)
				{
					found = true;
					broken = false;
					S.push_back(nextNode);
					outfile << nextNode << endl;
					break;
				}
			}
			if (found)
				break;
		}
		nextNode = sortedNodes.front();
		sortedNodes.pop();
	}
	outfile << S.size() << endl;
	cout << S.size() << endl;
}

void printDegree(map<string, int> node_degree)
{
	for (auto it = node_degree.begin(); it != node_degree.end(); ++it)
	{
		string output = it->first + "\t" + to_string(it->second);
		outfile << output << endl;
	}
}

int cnt_Transit(map<string, vector< tuple<string, string>> > my_map, map<string, int> node_degree)
{
	int count = 0;
	bool customerExists;

	for (auto it = my_map.begin(); it != my_map.end(); ++it)
	{
		customerExists = false;
		for (int i = 0; i < it->second.size(); ++i)
		{
			if (get<1>(it->second[i]).compare("-1") == 0)	// Customer
				customerExists = true;
		}
		if (customerExists)
			++count;
	}
	return count;
}

int cnt_content(map<string, vector< tuple<string, string>> > my_map, map<string, int> node_degree)
{
	int count = 0;
	bool peerExists;
	bool customerExists;

	for (auto it = my_map.begin(); it != my_map.end(); ++it)
	{
		peerExists = false;
		customerExists = false;

		for (int i = 0; i < it->second.size(); ++i)
		{
			if (get<1>(it->second[i]).compare("0") == 0)	// Peer
				peerExists = true;
			else if (get<1>(it->second[i]).compare("-1") == 0)	// Customer
				customerExists = true;
		}
		if (!customerExists && peerExists)
			++count;
	}

	return count;
}

int cnt_Enterprise(map<string, vector< tuple<string, string>> > my_map, map<string, int> node_degree)
{
	int count = 0;

	for (auto it = my_map.begin(); it != my_map.end(); ++it)
	{
		auto it2 = node_degree.find(it->first);

		if ((it2->second <= 2) && (it->second.size() == 0))
		{
			++count;
		}
	}

	return count;
}