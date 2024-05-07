#pragma once
#include "DBScan.h"
#include "Distance.h"
#include "Timer.h"

namespace DBScan {
	class DBScanExample
	{
	public:
		static void run1(PointsArray data, int min_pts, double eps, DistanceFun distance = Distance::Euclidean) {
			auto db_scan = DBScan::DBScan(distance, eps, min_pts);

			Timer t;
			t.start();
			auto out = db_scan.fitPredict(data);
			t.end();

			int i = 0;
			int count = 0;
			
			for (auto c : db_scan.getClustersCounts()) {
				count += c;
				std::cout << "cluster (" << i++ << "): " << c << std::endl;
			}

			std::cout << "cluster (-1): " << data.size() - count << std::endl;

			std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;

			t.ElapsedTime();
		}

		static void run2(PointsArray data, std::pair<int, int> min_pts, std::pair<double, double> eps, double step, DistanceFun distance = Distance::Euclidean) {
			Timer t;

			t.start();
			auto db_scan = DBScan::bestClusters(data, distance, eps, min_pts, step);
			t.end();
			
			int i = 0;
			int count = 0;

			for (auto c : db_scan.first.getClustersCounts()) {
				count += c;
				std::cout << "cluster (" << i++ << "): " << c << std::endl;
			}

			std::cout << "cluster (-1): " << data.size() - count << std::endl;

			std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
			std::cout << "silhouette: " << db_scan.second << ", eps: " << db_scan.first.getEps();
			std::cout << ", Min points: " << db_scan.first.getMinPoints() << std::endl;
			std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
			t.ElapsedTime();
		}
	};
}
