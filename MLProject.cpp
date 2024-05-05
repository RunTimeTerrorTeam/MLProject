#include <iostream>
#include <exception>

#include "CSV.h"
#include "KMeansExample.h"
#include "AgglomerativeExample.h"
#include "DBScanExample.h"

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

std::vector<std::vector<double>> testData3 = {
	{ 3, 7 }, //0
	{ 4, 6 }, //1
	{ 5, 5 }, //2
	{ 6, 4 }, //3
	{ 7, 3 }, //4
	{ 6, 2 }, //5
	{ 7, 2 }, //6
	{ 8, 4 }, //7
	{ 3, 3 }, //8
	{ 2, 6 }, //9
	{ 3, 5 }, //10
	{ 2, 4 }, //11
};

int main()
{
	try {
		CSV csv("Resources/sample2d.csv");
		auto data = csv.data();

		std::cout << "~--~--~--~" << std::endl;
		std::cout << "| KMeans |" << std::endl;
		std::cout << "~--~--~--~" << std::endl;
		KMeans::KMeansExample::run1(data, 10, 100 /* max INT_MAX*/ /*, Distance::Euclidean*/);
		std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
		KMeans::KMeansExample::run2(data, 10, 100 /* max INT_MAX*/ /*, Distance::Euclidean*/);
		std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
		
		std::cout << "Next Agglomerative ";
		std::system("pause");

		std::cout << "~--~--~--~--~--~" << std::endl;
		std::cout << "| Agglomerative |" << std::endl;
		std::cout << "~--~--~--~--~--~" << std::endl;
		Agglomerative::AgglomerativeExample::run1(data);
		std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;

		std::cout << "Next DBScan ";
		std::system("pause");

		std::cout << "~--~--~--~" << std::endl;
		std::cout << "| DBScan |" << std::endl;
		std::cout << "~--~--~--~" << std::endl;
		DBScan::DBScanExample::run1(data, 4, 1 /*, Distance::Euclidean*/);
		std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
	
	return 0;
}

