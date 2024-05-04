#pragma once
#include <iostream>

#include "KMeans.h"
#include "Distance.h"

namespace KMeans {
	class KMeansExample
	{
	public:
		// run data and get points cluster assignment, centroids, and count every point nearest point
		static void run1(PointsArray, int, int, DistanceFun);
		// run data and get number of cluster and inertia
		static void run2(PointsArray, int, int, DistanceFun);

	private:
		// print point
		static std::string point(std::vector<double> p);
	};

	void KMeansExample::run1(PointsArray points, int n_cluster, int max_iterations, DistanceFun distance = Distance::Euclidean) {
		auto km = KMeans(distance, n_cluster, max_iterations);

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

	void KMeansExample::run2(PointsArray points, int max_n_cluster, int max_iterations, DistanceFun distance = Distance::Euclidean) {
		std::cout << "n_cluster\tinertia" << std::endl;

		for (int i = 1; i <= max_n_cluster; i++)
		{
			auto km = KMeans::KMeans(distance, i, max_iterations);
			km.fitPredict(points);
			std::cout << "    " << i << "    \t" << km.inertia() << std::endl;
		}
	}

	std::string KMeansExample::point(std::vector<double> p) {
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
}