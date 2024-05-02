#pragma once
#include <vector>
#include <cmath>

class Distance
{
public:
	static double Euclidean(std::vector<double> point1, std::vector<double> point2) {
		double distance = 0.0;
		for (int i = 0; i < point1.size(); ++i) {
			distance += pow(point1[i] - point2[i], 2);
		}
		return sqrt(distance);
	}

	static double Manhatin(std::vector<double> point1, std::vector<double> point2) {
		double distance = 0.0;
		for (int i = 0; i < point1.size(); ++i) {
			distance += abs(point1[i] - point2[i]);
		}
		return distance;
	}
};

