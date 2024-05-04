#pragma once
#include <iostream>

#include "Agglomerative.h"
#include "Distance.h"

namespace Agglomerative {
	class AgglomerativeExample
	{
	public:
		static void run1(PointsArray, DistanceFun);
	private:

	};

	void AgglomerativeExample::run1(PointsArray points, DistanceFun distance = Distance::Euclidean) {
		auto agg = Agglomerative(distance);

		int i = 0;

		for (const auto& labels : agg.fitPredict(points)) {
			std::cout << "_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_" << std::endl;
			std::cout << "step(" << i << "):" << std::endl;
			std::cout << "_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_" << std::endl;

			for (int j = 0; j < labels.size(); j++)
			{
				std::cout << "cluster(" << j + 1 << "): [" << labels[j] << "]" << std::endl;
			}

			std::cout << "==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==" << std::endl;
		}
	}
}


