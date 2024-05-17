#include <iostream>
#include <exception>
#include <string>

#include "CSV.h"
#include "KMeansExample.h"
#include "AgglomerativeExample.h"
#include "DBScanExample.h"
#include "OpticsExample.h"
#include "Silhouette.h"

std::vector <std::vector<double>> testData1 = {
	{1,1},
	{2,1},
	{1,2},
	{2,2},
	{3,5},
	{3,9}, 
	{3, 10}, 
	{4, 10}, 
	{4, 11}, 
	{5, 10}, 
	{7, 10}, 
	{10, 9}, 
	{10, 6}, 
	{9, 5}, 
	{10, 5}, 
	{11, 5}, 
	{9, 4}, 
	{10, 4}, 
	{11, 4}, 
	{10, 3}

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
	{ 3, 7 },
	{ 4, 6 },
	{ 5, 5 },
	{ 6, 4 },
	{ 7, 3 },
	{ 6, 2 },
	{ 7, 2 },
	{ 8, 4 },
	{ 3, 3 },
	{ 2, 6 },
	{ 3, 5 },
	{ 2, 4 },
};
std::vector <std::vector<double>> testData4 = {
	{1, 1},
	{5, 5},
	{8, 8},
	{10, 10},
	{2, 2}
};

void test1(const std::vector<std::vector<double>>& data) {
	auto clusters = KMeans::KMeans(Distance::Euclidean, 10, INT_MAX/* max INT_MAX*/ /*, Distance::Euclidean*/);
	
	Timer t;
	t.start();

	for (int i = 0; i < 10; i++) {
		clusters.fitPredict(data);
	}

	t.end();

	t.ElapsedTime();
}

void test2() {
	std::vector<double> time(9);
	std::vector<double> data_per(9);

	int n = 1;

	for (int i = 1; i <= 8; i += 1) {
		auto read_csv = CSV::read("Resources/SKIN_" + std::to_string(i) + ".csv");
		auto clusters = DBScan::DBScan(Distance::Euclidean, 1.0,2 /* max INT_MAX*/ /*, Distance::Euclidean*/);

		Timer t;
		t.start();

		for (int j = 0; j < 10; j++) {
			clusters.fitPredict(read_csv.data());
		}

		t.end();

		t.ElapsedTime();

		time[n] = t.time();
		data_per[n] = i;
		n++;
	}

	Plot p;
	p.plot(data_per, time);
	p.xLabel("size data");
	p.yLabel("Time");
	p.title("SKIN_NO_SKIN");
	p.show();
}

int main()
{
	try {
		//test2();
		auto read_csv = CSV::read("Resources/SKIN_1.csv");
		auto allData = read_csv.data();
		
		auto& data = allData;
		
		//test1(allData);

		//auto data = std::vector<std::vector<double>>(allData.begin(), allData.begin() +  3000/* max 12203 */);

		//std::cout << "~--~--~--~" << std::endl;
		//std::cout << "| KMeans |" << std::endl;
		//std::cout << "~--~--~--~" << std::endl;
		//auto clusters = KMeans::KMeansExample::run1(data, 10, INT_MAX/* max INT_MAX*/ /*, Distance::Euclidean*/);
		//csv.save("cluster", clusters);

		//std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
		//KMeans::KMeansExample::run2(data, 10, INT_MAX /* max INT_MAX*/ /*, Distance::Euclidean*/);
		//std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
		
		//std::cout << "Next Agglomerative ";
		//std::system("pause");
		
		//std::cout << "~--~--~--~--~--~" << std::endl;
		//std::cout << "| Agglomerative |" << std::endl;
		//std::cout << "~--~--~--~--~--~" << std::endl;
		//Agglomerative::AgglomerativeExample::run1(allData, 2, Agglomerative::LINKAGE::complete);
		//std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;

		//std::cout << "Next DBScan ";
		//std::system("pause");

		//double eps = 5;
		//int min_pts = 4;

		//std::cout << "~--~--~--~" << std::endl;
		//std::cout << "| DBScan |" << std::endl;
		//std::cout << "~--~--~--~" << std::endl;
		DBScan::DBScanExample::run1(data, 8, 10, Distance::Euclidean);
		//std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
		//DBScan::DBScanExample::run2(data, { 2, 10 }, { 0.1, 2 }, 0.5 /*, Distance::Euclidean*/);
		//std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;

		//std::cout << "Next Optics ";
		//std::system("pause");

		//std::cout << "~--~--~--~" << std::endl;
		//std::cout << "| Optics |" << std::endl;
		//std::cout << "~--~--~--~" << std::endl;
		//Optics::OpticsExample::run1(data, eps, min_pts, Distance::Manhatin);
		//std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;

	}
	catch (std::exception e) {
		std::cout << e.what();
	}
	
	return 0;
}

