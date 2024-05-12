#pragma once
#include <iostream>

#include "Agglomerative.h"
#include "Distance.h"
#include "Timer.h"
#include "Plot.h"


namespace Agglomerative {
	class AgglomerativeExample
	{
	public:
		static std::vector<int> run1(PointsArray, int, LINKAGE, DistanceFun);

	private:
		static std::string linkageValue(LINKAGE);
	};

	std::vector<int> AgglomerativeExample::run1(PointsArray points, int n_cluster = 2, LINKAGE linkage = LINKAGE::single, DistanceFun distance = Distance::Euclidean) {
		auto agg = Agglomerative(distance, n_cluster, linkage);
		auto timer = Timer();

		timer.start();
		auto cluster_assignment = agg.fitPredict(points);
		timer.end();
		
		timer.ElapsedTime();


		Plot plot;

		plot.scatter(points, cluster_assignment);
		plot.xLabel("X axis");
		plot.yLabel("Y axis");
		plot.title("Agglomerative (Number of clusters = " + std::to_string(n_cluster) + ", Linkage = " + linkageValue(linkage) + ")");

		plot.show();

		return cluster_assignment;
	}

	std::string AgglomerativeExample::linkageValue(LINKAGE linkage)
	{
		switch (linkage)
		{
			case LINKAGE::single:   return "single";
			case LINKAGE::average:  return "average";
			case LINKAGE::complete: return "complete";
			default: throw std::invalid_argument("Error: linkage not found");
		}
	}
}


