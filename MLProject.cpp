#include <iostream>
#include <exception>

#include "CSV.h"
#include "KMeansExample.h"
#include "AgglomerativeExample.h"

std::vector <std::vector<double>> testData1 = {
	{1,1},
	{1.5,1.5},
	{5,5},
	{3,4},
	{4,4},
	{3,3.5}
};

std::vector <std::vector<double>> testData2 = {
	{0.7, 3.2},
	{2.45, 2.89},
	{3.47, 1.12},
	{5.23, 5.24},
	{5.98, 6.23},
	{7.778, 8.63},
	{8.97, 6.12}
};

int main()
{
	try {
		//CSV csv("Resources/editedForce.csv");
		CSV& csv = CSV::getInstance("Resources/editedForce.csv");
		auto allData = csv.data();
		

		//auto data = allData;
		auto data = std::vector<std::vector<double>>(allData.begin(), allData.begin() +  1000/* max 12203 */);

		std::cout << "~--~--~--~" << std::endl;
		std::cout << "| KMeans |" << std::endl;
		std::cout << "~--~--~--~" << std::endl;
		KMeans::KMeansExample::run1(data, 10, 80 /* max INT_MAX*/ /*, Distance::Euclidean*/);
		std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
		KMeans::KMeansExample::run2(data, 10, 80 /* max INT_MAX*/ /*, Distance::Euclidean*/);
		std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
		
		std::cout << "Next Agglomerative ";
		std::system("pause");

		std::cout << "~--~--~--~--~--~" << std::endl;
		std::cout << "| Agglomerative |" << std::endl;
		std::cout << "~--~--~--~--~--~" << std::endl;
		Agglomerative::AgglomerativeExample::run1(data);
	}
	catch (std::exception e) {
		std::cout << e.what();
	}

}

