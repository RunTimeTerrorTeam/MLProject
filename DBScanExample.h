#pragma once
#include "DBScan.h"
#include "Distance.h"
#include "Timer.h"

namespace DBScan {
	class DBScanExample
	{
	public:
		static std::vector<int> run1(PointsArray points, double eps, int min_pts, DistanceFun distance = Distance::Euclidean) {
			auto db_scan = DBScan::DBScan(distance, eps, min_pts);

			Timer t;
			t.start();
			auto cluster_assignment = db_scan.fitPredict(points);
			t.end();

			int i = 0;
			int count = 0;
			
			for (auto c : db_scan.getClustersCounts()) {
				count += c;
				std::cout << "cluster (" << i++ << "): " << c << std::endl;
			}

			std::cout << "cluster (-1): " << points.size() - count << std::endl;

			std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;

			t.ElapsedTime();

			std::cout << "score:" << db_scan.score() << std::endl;

			Plot plot;

			plot.scatter(points, cluster_assignment);
			plot.xLabel("X axis");
			plot.yLabel("Y axis");
			plot.title("DBScan (eps = " + std::to_string(eps) + ", min points = " + std::to_string(min_pts) + ")");

			plot.show();

			return cluster_assignment;
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
