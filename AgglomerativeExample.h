#pragma once
#include <iostream>

#include "Agglomerative.h"
#include "Distance.h"

namespace Agglomerative {
	class AgglomerativeExample
	{
	public:
		static void run1(PointsArray, DistanceFun);
	};

	void AgglomerativeExample::run1(PointsArray points, DistanceFun distance = Distance::Euclidean) {
		auto agg = Agglomerative(distance);
		auto timer = Timer();

		int i = 0;

		timer.start();
		auto steps_labels = agg.fitPredict(points);
		timer.end();

		// TODO: write to csv

		timer.ElapsedTime();
	}
}


