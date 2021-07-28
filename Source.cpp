#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include<cmath>
#include <algorithm>


using namespace std;

void print_vector(vector<double> x) // to print the vector
{
	for (size_t i = 0; i < x.size(); i++)
	{
		cout << x[i] << " " << endl;
	}
}

string check_data(vector<string> vec) // checks if user entered a state correctly
{
	bool done=false;
	while (!done)
	{
		cout << "Enter a state (capitalized two letter initials, for example MD): " << endl;
		string input;
		cin >> input;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i] == input)
			{
				return input;
			}
		}
		cout << "Not a state. Try again." << endl;
	}
	return 0;
}


void print_data(vector< vector<string> > vec, string input) // find input in vec and prints other data from same row
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		for (size_t j = 0; j < vec[i].size(); j++)
		{
			if (vec[i][j] == input)
			{
				cout << "State info for " << input << " is population: " << vec[i][j + 1] << ", area: " << vec[i][2] << ", latitude: " << vec[i][3] << ", longitude: " << vec[i][4] << endl;
				break;
			}
		}
	}
}

string ret_data(vector< vector<string> > vec, string input, int num) // find input in vec and one piece of data based on num index 
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		for (size_t j = 0; j < vec[i].size(); j++)
		{
			if (vec[i][j] == input)
			{
				return (vec[i][num]); // num info: population = 1, area = 2, latitude = 3, longitude = 4, culture = 5
			}
		}
	}
}

// void operation(vector< vector<string> > vec2, string input2, int num2, void (*function)(vector< vector<string> >, string, int)) { return function(vec2, input2, num2); } // abandoned for now

/*double avgPop(vector< vector<string> > vec) // ended up not needing this, but it does work to find the sum and average
{
	double sum = 0.0;
	for (size_t i = 0; i < vec.size(); i++)
	{
		double x = 0.0;
		string str1 = vec[i][1];
		x = stoi(str1);
		sum = sum+x;
	} //  
	return (sum/51); 
}*/


double statePop(vector< vector<string> > firstVector, string firstInput, string secondInput, int column=1) // combines states population
{
	string str1 = ret_data(firstVector, firstInput, 1); // returns population for home state
	int homePop = stoi(str1);
	string str2 = ret_data(firstVector, secondInput, 1); // returns population for out of state plate
	int outPop = stoi(str2);
	double totalPop = (homePop + outPop); //maximum of two states is Cali + Texas at 68,508,105 : this is equal to 1 and everything else is divided by this number.
	return (totalPop / (68508105)*0.25); // just doing each of four factors contributes quarter to program result. Will return double between 0.0 and 0.25.
}

double driving(vector< vector<string> > firstVector, string firstInput, int column = 2) // area is home state area in miles, drive is how much driving user will do
{
	while (true)
	{
		cout << "How many miles will you drive?";
		double drive;
		cin >> drive;
		if (drive < 0)
		{
			cout<< "Can't drive negative miles! Unless you can reverse time. Try again.";
		}
		else
		{
			string str1 = ret_data(firstVector, firstInput, 2); // returns area for home state
			int area = stoi(str1);
			double totalArea = (drive / area);// how much area covered by driving
			if (totalArea >= 1.0)
			{
				totalArea = 1.0;
			}
			return (totalArea * 0.25); // just doing each of four factors contributes quarter to program result
		}
	}
}

double distance(vector< vector<string> > firstVector, string firstInput, string secondInput) // latitude and longitude of home state and out of state to calculate distance
{
	double diffLat, diffLong;
	string str1 = ret_data(firstVector, firstInput, 3); // returns latitude for home state
	double homeLat = stoi(str1);
	
	string str2 = ret_data(firstVector, firstInput, 4); // returns longitude for home state
	double homeLong = stoi(str2);
	
	string str3 = ret_data(firstVector, secondInput, 3); // returns latitude for home state
	double outLat = stoi(str3);
	
	string str4 = ret_data(firstVector, secondInput, 4); // returns latitude for home state
	double outLong = stoi(str4);
	
	diffLat = abs(abs(homeLat) - abs(outLat)); // finds the difference in latitude
	diffLong = abs(abs(homeLong) - abs(outLong)); // finds difference in longitude
	
	int ptsLat, ptsLong;
	if (diffLat < 3.0)
	{
		ptsLat = 4;
	}
	else if (diffLat > 3.0 && diffLat < 6.0)
	{
		ptsLat = 3;
	}
	else if (diffLat > 6.0 && diffLat < 9.0)
	{
		ptsLat = 2;
	}
	else
	{
		ptsLat = 1;
	}

	if (diffLong < 10.0)
	{
		ptsLong = 4;
	}
	else if (diffLong > 10.0 && diffLong < 20.0)
	{
		ptsLong = 3;
	}
	else if (diffLong > 20.0 && diffLong < 30.0)
	{
		ptsLong = 2;
	}
	else
	{
		ptsLong = 1;
	}

	return (ptsLat*0.125 + ptsLong*.125)*0.25; // each worth 0.125 max for total max of 0.25
}

double culture(vector< vector<string> > firstVector, string firstInput, string secondInput)
{
	int homeCult, outCult;
	string str1 = ret_data(firstVector, firstInput, 5); // returns culture for home state
	string str2 = ret_data(firstVector, secondInput, 5); // returns culture for other state

	if (str1 == "Uppy") // four possible cultures assigned by string in text file and converted to int here so can calculate the difference in culture
	{
		homeCult = 0;
	}
	else if (str1 == "Wild")
	{
		homeCult = 1;
	}
	else if (str1 == "Flyover")
	{
		homeCult = 2;
	}
	else
	{
		homeCult = 3;
	}

	if (str2 == "Uppy")
	{
		outCult = 0;
	}
	else if (str2 == "Wild")
	{
		outCult = 1;
	}
	else if (str2 == "Flyover")
	{
		outCult = 2;
	}
	else
	{
		outCult = 3;
	}
	
	int diffCulture = abs(homeCult-outCult); // absolute value difference in culture
	int diffCultSwitch=100;
	switch (diffCulture) // assigning points based on similiarities but in reverse numbers 
	{
		case 0: 
			diffCultSwitch = 4;
			break;
		case 1: 
			diffCultSwitch = 3;
			break;
		case 2: 
			diffCultSwitch = 2;
			break;
		case 3: 
			diffCultSwitch = 1;
			break;
	}
	return (diffCultSwitch*0.25*0.25); // equal cultures get 0.25 points (4*0.25*0.25)
}

int main()
{
	vector<string> states_list{};
	ifstream file("c:\\test\\states_list.txt");
	string element;
	while (file >> element)
	{
		states_list.push_back(element);
	}
	
	vector< vector<string> > data; // vector of vectors

	ifstream in("c:\\test\\population.txt"); // open the file
	for (string line; getline(in, line); ) // create object to getline from file
	{
		stringstream ss(line);
		vector<string> row; // make the first vector - row
		for (string d; ss >> d; ) row.push_back(d); // for each element in the stringstream
		data.push_back(row); // make the second vector - column
	}

	string home = { check_data(states_list) };
	string state = { check_data(states_list) };

	//print_data(data, home); // calls function above for population
	//print_data(data, state); // calls function above for population
	statePop(data, home, state); // calls function to calculate population
	double drive = driving(data, home); // calls function to calculate area and driving amount
	distance(data, home, state); // calls function to calculate distance between states
	culture(data, home, state); // calls function to calculate cultural aspects
	
	cout << endl << endl << "The chance of finding an out of state plate:" << state << " in " << home << " is: " << endl;
	cout.precision(8);
	cout<<(statePop(data, home, state) + drive + distance(data, home, state) + culture(data, home, state))*100<< " % "<<endl; // as a percent

	return 0;
}

/*create basic vector and pushback some elements into it.
	vector<double> vec1{0,1,2,3};
	vec1.push_back(4);
	cout<<"Element at index 4 (last): "<<vec1[4] << endl;
	cout<<"Original vector size: "<< vec1.size() << "\n";
	print_vector(vec1); //print the first vector
	cout << endl;

	cout << "Value at the index: " << data[0][state]<<endl;
	//can do calculations on values in index by using &reference like (&data[0][3] - &data[1][3])
	cout << "Attempting to subtract two values: " << (&data[0][1] - &data[1][1])<<endl; 

	cout << "Your data:\n"; // prints the entire text file/vector
	for (size_t i = 0; i < data.size(); i++)
	{
		for (size_t j = 0; j < data[i].size(); j++)
			cout << data[i][j] << " ";
		cout << endl;
	}
	*/