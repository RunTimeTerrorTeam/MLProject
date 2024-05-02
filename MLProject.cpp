
#include <iostream>
#include <exception>

#include "Distance.h"
#include "CSV.h"
#include "KMeansExample.h"
#include "Agglomerative.h"

int main()
{
	try {
		CSV csv("Resources/testData.csv");

		//KMeans::KMeansExample::run(
		//	KMeans::KMeans(Distance::Euclidean, 10, 30), 
		//	csv.data()
		//);
		
		auto data = csv.data();

		Agglomerative::Agglomerative agg(Distance::Euclidean);
		
		int i = 0;

		for (const auto& labels : agg.fitPredict(data)) {
			for (auto row : agg.distance_matrices[i]) {
				for (auto e : row)
				{
					std::cout << e << "\t\t\t";
				}
				std::cout << std::endl;
			}

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

