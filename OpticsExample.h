#pragma once
#include "Timer.h"
#include "Optics.h"

namespace Optics {
	class OpticsExample
	{
	private: 
		static std::vector<Point> prepareData(PointsArray data) {
			std::vector<Point> DB;
			int index = 0;

			for (auto p : data) {
				std::vector<double> points;
				for (int j = 0; j < p.size(); j++) {
					points.push_back(p[j]);
				}
				DB.push_back({ points, index++ });
			}

			return DB;
		}
	public:
		static void run(PointsArray data, int min_pts, double eps) {
			Timer t;
			t.start();

			std::vector<Point> ready_points = OpticsExample::prepareData(data);
			
			auto o = Optics::Optics(ready_points, eps, min_pts);
			o.fit();
			o.printClusters();
			
			
			t.end();
			std::cout << "-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-" << std::endl;
		
			t.ElapsedTime();
		}


		

	};

}