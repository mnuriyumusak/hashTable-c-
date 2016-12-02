#include <string>
#include <iostream>
using namespace std;
#include "hashing.h"
#include <fstream>
#include <iomanip>
int giveMeTransaction(int t)
{
	//as a return type
	//0 - inserr
	//1 - delete
	//2 - find
	int remainer = t % 24;
	if (remainer >= 16 && remainer < 24)
	{
		if (remainer >= 16 && remainer <= 21)
			return 0;
		else if (remainer == 22)
			return 1;
		else if (remainer == 23)
			return 2;
	}
	else if (remainer >= 8 && remainer < 16)
	{
		//remainer = t % 16;
		if (remainer >= 8 && remainer <= 11)
			return 0;
		else if (remainer == 12 || remainer == 13)
			return 1;
		else if (remainer == 14 || remainer == 15)
			return 2;
		else
			return 9;
	}
	else if (remainer >= 0 && remainer < 8)
	{
		//remainer = t % 8;
		if (remainer == 0 || remainer == 1)
			return 0;
		else if (remainer == 2)
			return 1;
		else if (remainer > 2 && remainer < 8)
			return 2;
	}
}

int generateNumber(int i)
{
	int n;
	srand(i);
	n = rand() % 10000000;
	return n;
}


 
int main()
{
	hashing<int> h(10000);
	int p = 0;
	int processNumber = 0;
	int transactionType = 0;
	float currentLoadFActor = 0;
	float howManyProbesConductedIS = 0;
	float howManyProbesConductedIF = 0;
	float howManyProbesConductedDS = 0;
	float howManyProbesConductedDF = 0;
	float howManyProbesConductedFS = 0;
	float howManyProbesConductedFF = 0;
	bool transactionStatus = false;
	int i=0, d =0,f=0;
	string insertDataS="",deleteDataS = "",findDataS = "", insertDataF = "", deleteDataF = "", findDataF = "";
	while (h.hashTableSize != h.currentTotalItem && h.getTotalTransaction() != 1000000)
	{
		currentLoadFActor = (float)h.currentTotalItem / (float)h.hashTableSize;
		processNumber = generateNumber(p);
		transactionType = giveMeTransaction(p);
		//cout << processNumber << "," << transactionType << endl;
		if (transactionType == 0)
		{
			h.insert(processNumber);
			transactionStatus = h.transactionStatus;
			i++;
			if (transactionStatus)
			{
				howManyProbesConductedIS += (float)h.howManyProbesConducted / (float)h.insertTableS;
				insertDataS += to_string(howManyProbesConductedIS) + ";";
				insertDataS += to_string(currentLoadFActor) + ";\n";
			}
			else
			{
				howManyProbesConductedIF += (float)h.howManyProbesConducted / (float)h.insertTableF;
				insertDataF += to_string(howManyProbesConductedIF) + ";";
				insertDataF += to_string(currentLoadFActor) + ";\n";
			}

		}
		else if (transactionType == 1)
		{
			h.remove(processNumber);
			transactionStatus = h.transactionStatus;
			d++;
			if (transactionStatus)
			{
				howManyProbesConductedDS += (float)h.howManyProbesConducted / (float)h.deleteTableS;
				deleteDataS += to_string(howManyProbesConductedDS) + ";";
				deleteDataS += to_string(currentLoadFActor) + ";\n";
			}
			else
			{
				howManyProbesConductedDF += (float)h.howManyProbesConducted / (float)h.deleteTableF;
				deleteDataF += to_string(howManyProbesConductedDF) + ";";
				deleteDataF += to_string(currentLoadFActor) + ";\n";
			}
		}
		else if (transactionType == 2)
		{
			h.find(processNumber);
			transactionStatus = h.transactionStatus;
			f++;
			if (transactionStatus)
			{
				howManyProbesConductedFS += (float)h.howManyProbesConducted / (float)h.findTableS;
				findDataS += to_string(howManyProbesConductedFS) + ";";
				findDataS += to_string(currentLoadFActor) + ";\n";
			}
			else
			{
				howManyProbesConductedFF += (float)h.howManyProbesConducted / (float)h.findTableF;
				findDataF += to_string(howManyProbesConductedFF) + ";";
				findDataF += to_string(currentLoadFActor) + ";\n";
			}
		}
		p++;
	}
	string output;
	output = to_string(h.insertTableS) + ";";
	output += to_string(h.insertTableF) + ";";
	output += to_string(h.deleteTableS) + ";";
	output += to_string(h.deleteTableF) + ";";
	output += to_string(h.findTableS) + ";";
	output += to_string(h.findTableF) + ";";
	output += to_string(h.currentTotalItem) + ";";
	output += to_string(h.hashTableSize) + ";";
	output += to_string(h.getTotalTransaction()) + ";";
	output += to_string(i) + ";";
	output += to_string(d) + ";";
	output += to_string(f) + ";";
	output += "\nSuccess insert;\n";
	output += insertDataS;
	output += "\nFail insert;\n";
	output += insertDataF;
	output += "\nSuccess delete;\n";
	output += deleteDataS;
	output += "\nFail delete;\n";
	output += deleteDataF;
	output += "\nSuccess find;\n";
	output += findDataS;
	output += "\nFail find;\n";
	output += findDataF;
	//std::cout << output << endl;
	ofstream myfile;
	myfile.open("insertDataS.txt");
	myfile << insertDataS;
	myfile.close();
	myfile.open("insertDataF.txt");
	myfile << insertDataF;
	myfile.close();
	myfile.open("deleteDataS.txt");
	myfile << deleteDataS;
	myfile.close();
	myfile.open("deleteDataF.txt");
	myfile << deleteDataF;
	myfile.close();
	myfile.open("findDataS.txt");
	myfile << findDataS;
	myfile.close();
	myfile.open("findDataF.txt");
	myfile << findDataF;
	myfile.close();
	myfile.open("output.txt");
	myfile << output;
	myfile.close();
	//int y;
	//std::cin >> y;
	return 0;
}