
#include <iostream>
#include <exception>

#include "Distance.h"
#include "CSV.h"
#include "KMeansExample.h"
#include "Agglomerative.h"

int main()
{
	try {
		CSV csv("Resources/force.csv");
		auto data = csv.data();
		
		//std::vector <std::vector<double>> data = /*{
		//	{1,1},
		//	{1.5,1.5},
		//	{5,5},
		//	{3,4},
		//	{4,4},
		//	{3,3.5}
		//};*/

		// {
		//	{0.7, 3.2},
		//	{2.45, 2.89},
		//	{3.47, 1.12},
		//	{5.23, 5.24},
		//	{5.98, 6.23},
		//	{7.778, 8.63},
		//	{8.97, 6.12}
		//};
		
		
		//KMeans::KMeansExample::run(
		//	KMeans::KMeans(Distance::Euclidean, 10, 30), 
		//	data
		//);
		
		
		Agglomerative::Agglomerative agg(Distance::Euclidean);
		
		
		int i = 0;

		for (const auto& labels : agg.fitPredict(std::vector<std::vector<double>>(data.begin(), data.begin() + 5000))) {
			std::cout << "_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_" << std::endl;
			std::cout << "step(" << ++i << "):" << std::endl;
			std::cout << "_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_" << std::endl;

			for (int j = 0; j < labels.size(); j++)
			{
				std::cout << "cluster(" << j + 1 << "): [" << labels[j] << "]" << std::endl;
			}

			std::cout << "==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==" << std::endl;
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}

}

