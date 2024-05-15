#pragma once
#include "Timer.h"
#include "Optics.h"

namespace Optics {
	class OpticsExample
	{
	public:
		static std::pair<std::vector<int>, std::vector<double>> run1(PointsArray, double, int, DistanceFun);

	private:
	};


	std::pair<std::vector<int>, std::vector<double>> OpticsExample::run1(PointsArray data, double eps, int min_pts, DistanceFun distance = Distance::Manhatin) {
		auto optics = Optics::Optics(distance, eps, min_pts);

		Timer t;
		t.start();
		auto clusters_assignments = optics.fitPredict(data);
		t.end();

		auto order_list = optics.getOrderedList();
		auto reachability_distance = optics.getReachabilityDistance();
		int size = (int)reachability_distance.size();
		auto ordered_reachability_distance = std::vector<double>(size);
		auto indexes = std::vector<double>(size); 
		auto clusters_counts = optics.getClustersCounts();

		int count = 0;

		for (int i = 0; i < clusters_counts.size(); i++) {
			count += clusters_counts[i];
			std::cout << "Cluster(" << i << "): " << clusters_counts[i] << std::endl;
		}

		std::cout << "Cluster(-1): " << clusters_assignments.size() - count << std::endl;
		
		for (int i = 0; i < size; i++) {
			indexes[i] = i;
			ordered_reachability_distance[i] = reachability_distance[order_list[i]];
		}


		t.ElapsedTime();

		std::cout << "score: " << optics.score() << std::endl;

		/*Plot plot;
		plot.scatter(indexes, ordered_reachability_distance);
		plot.show();*/

		Plot plot2;

		plot2.scatter(data, clusters_assignments);
		plot2.xLabel("X axis");
		plot2.yLabel("Y axis");
		plot2.title("Optics clusters (eps = " + std::to_string(eps) + ", min points = " + std::to_string(min_pts) + ")");

		plot2.show();

		return { order_list, ordered_reachability_distance };
	}
}