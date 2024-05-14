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
		// out => [points numbers, reachability distance]
		auto out = optics.fitPredict(data);
		t.end();

		auto& order_list = out.first;
		auto& reachability_distance = out.second;

		for (int point_index : order_list) {
			std::cout << "Point(" << point_index + 1 << "): " << reachability_distance[point_index] << std::endl;
		}

		std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;

		int size = (int)reachability_distance.size();

		auto ordered_reachability_distance = std::vector<double>(size);

		for (int i = 0; i < size; i++) {
			ordered_reachability_distance[i] = reachability_distance[order_list[i]];
		}

		t.ElapsedTime();

		return { order_list, ordered_reachability_distance };
	}
}