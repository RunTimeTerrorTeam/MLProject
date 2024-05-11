#pragma once
#include <iostream>
#include <string>

#include "timer.h"
#include "KMeans.h"
#include "Distance.h"
#include "Plot.h"

namespace KMeans {
	class KMeansExample
	{
	public:
		// run data and get points cluster assignment, centroids, number of points in cluster, and time spend
		static std::vector<int> run1(PointsArray, int, int, DistanceFun);
		// run data and get number of cluster and inertia
		static void run2(PointsArray, int, int, DistanceFun);

	private:
		// print point
		static std::string point(std::vector<double> p);
	};

	std::vector<int> KMeansExample::run1(PointsArray points, int n_cluster, int max_iterations, DistanceFun distance = Distance::Euclidean) {
		auto km = KMeans(distance, n_cluster, max_iterations);
		auto timer = Timer();
		
		timer.start();
		auto cluster_assignments = km.fitPredict(points);
		timer.end();

		auto centroids = km.getCentroids();
		auto count = km.getPointsCountAroundCentroids();

		std::cout << "Centroids:" << std::endl;

		for (int i = 0; i < centroids.size(); i++) {
			std::cout << "* Cluster(" << i + 1 << "): " << std::endl;
			std::cout << "\t- Centroid:" << point(centroids[i]) << std::endl; 
			std::cout << "\t- number of points: " << count[i] << std::endl;
		}
		std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
		timer.ElapsedTime();

		Plot plot;

		plot.scatter(points, cluster_assignments);

		plot.scatter(centroids);

		plot.xLabel("x axis");
		plot.yLabel("y axis");
		plot.title("kmeans clusters (k = " + std::to_string(n_cluster) + ")");

		plot.show();

		return cluster_assignments;
	}

	void KMeansExample::run2(PointsArray points, int max_n_cluster, int max_iterations, DistanceFun distance = Distance::Euclidean) {
		std::cout << "n_cluster\tinertia" << std::endl;

		std::vector<double> inertia(max_n_cluster);
		std::vector<double> cluster(max_n_cluster);

		for (int i = 1; i <= max_n_cluster; i++)
		{
			auto km = KMeans::KMeans(distance, i, max_iterations);
			km.fitPredict(points);

			cluster[i - 1] = i;
			inertia[i - 1] = km.inertia();

			std::cout << "    " << i << "    \t" << inertia[i - 1] << std::endl;
		}

		Plot plot2;

		plot2.plot(cluster, inertia);

		plot2.xLabel("Number of Clusters");
		plot2.yLabel("Inertia");
		plot2.title("KMeans with Inertia");

		plot2.show();
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