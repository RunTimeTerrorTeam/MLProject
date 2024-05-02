
#include <iostream>
#include <exception>

#include "Distance.h"
#include "CSV.h"
#include "KMeansExample.h"

int main()
{
	try {
		CSV csv("Resources/force.csv");

		KMeans::KMeansExample::run(
			KMeans::KMeans(Distance::Euclidean, 10, 30), 
			csv.data()
		);
	}
	catch (std::exception e) {
		std::cout << e.what();
	}

}

