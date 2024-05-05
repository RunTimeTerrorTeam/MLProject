#pragma once
#include <vector>

namespace DBScan {

    typedef std::vector<double> Point;
    typedef std::vector<Point> PointsArray;
    using DistanceFun = double(*)(Point, Point);

    struct CPoint
    {
        int                                         label;
        bool                                        is_processed;
        bool                                        is_clustered;
        std::vector<std::reference_wrapper<CPoint>> neighbors;

        CPoint()
            :label(-1), is_processed(false), is_clustered(false), neighbors() {}
    };

    class DBScan
    {
    public:
        DBScan(double eps, int minPts, DistanceFun distance_fun)
            :eps(eps), minPts(minPts), distance_fun(distance_fun) {}

        std::vector<std::vector<int>> fitPredict(const std::vector<Point>& points) {
            this->original_points = points;
            generateCPoints();

            clusters = std::vector<std::vector<int>>();

            for (auto& point : c_points) {
                if (point.is_processed) continue;

                point.is_processed = true;

                if (point.neighbors.size() + 1 < minPts) continue;

                std::vector<int> cluster = { point.label };

                point.is_clustered = true;
                auto& neighbors = point.neighbors;

                for (int i = 0; i < neighbors.size(); i++) {
                    if (!neighbors[i].get().is_processed) {
                        neighbors[i].get().is_processed = true;

                        auto& neighborNeighbors = neighbors[i].get().neighbors;

                        if (neighborNeighbors.size() + 1 >= minPts) {
                            neighbors.insert(neighbors.end(), neighborNeighbors.begin(), neighborNeighbors.end());
                        }
                    }

                    if (!neighbors[i].get().is_clustered) {
                        cluster.push_back(neighbors[i].get().label);
                        neighbors[i].get().is_clustered = true;
                    }
                }

                clusters.push_back(cluster);
            }

            return clusters;
        }


    private:
        std::vector<Point>               original_points;
        std::vector<CPoint>              c_points;
        std::vector<std::vector<double>> distance_matrix;
        double                           eps;
        int                              minPts;
        DistanceFun                      distance_fun;
        std::vector<std::vector<int>>    clusters;

        void generateCPoints();
    };


    void DBScan::generateCPoints()
    {
        int size = (int)original_points.size();

        c_points = std::vector<CPoint>(size, CPoint());

        for (int i = 0; i < size; i++)
        {
            c_points[i].label = i;

            for (int j = 0; j < i; j++) {
                const auto distance = distance_fun(original_points[i], original_points[j]);
                if (distance > eps) continue;

                c_points[i].neighbors.push_back(c_points[j]);
                c_points[j].neighbors.push_back(c_points[i]);
            }
        }
    }
}