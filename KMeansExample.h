#pragma once
#include "KMeans.h"

namespace KMeans {
	class KMeansExample
	{
	public:
		static void run(KMeans km, PointsArray points) {
			auto cluster_assignments = km.fitPredict(points);

			for (int i = 0; i < points.size(); i++) {
				std::cout << "Point " << i + 1 << ": " << point(points[i]) << " is cluster : " << cluster_assignments[i] << std::endl;
			}
			std::cout << std::endl;

			auto centroids = km.getCentroids();
			auto count = km.getPointsCountAroundCentroids();

			std::cout << "Centroids:" << std::endl;

			for (int i = 0; i < centroids.size(); i++) {
				std::cout << "\t" << point(centroids[i]) << " number of points nearest to it " << count[i] << std::endl;
			}
		}
	private:
		static std::string point(std::vector<double> p) {
			std::string s = "{";

			for (auto x : p) {
				s += std::to_string(x) + ", ";
			}

			// Remove the last ", "
			if (!s.empty()) {
				s.erase(s.size() - 2);
			}

			return s + "}";
		}
	};
}