#pragma once
#include "DBScan.h"
#include "Distance.h"
#include "Timer.h"

namespace DBScan {
	class DBScanExample
	{
	public:
		static void run1(PointsArray data, int min_pts, double eps, DistanceFun distance = Distance::Euclidean) {
			auto db_scan = DBScan::DBScan(eps, min_pts, distance);

			Timer t;
			t.start();
			auto out = db_scan.fitPredict(data);
			t.end();

			//for (auto& cluster: out)
			//{
			//	std::string s = "{";

			//	for (auto x : cluster) {
			//		s += std::to_string(x) + ", ";
			//	}

			//	// Remove the last ", "
			//	if (!s.empty()) {
			//		s.erase(s.size() - 2);
			//	}

			//	std::cout << "cluster (" << ++i << ") size ("<< cluster.size() << ") : " << s << "}" << std::endl;
			//}
			// std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;

			t.ElapsedTime();
		}
	};
}
