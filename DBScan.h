#pragma once
#include <vector>
#include <queue>
#include "Silhouette.h"

namespace DBScan {

    typedef std::vector<double> Point;
    typedef std::vector<Point> PointsArray;
    using DistanceFun = double(*)(Point, Point);

    class DBScan
    {
    public:
        DBScan(DistanceFun, double, int);
        std::vector<int> fitPredict(const PointsArray&);
        std::vector<int> getClustersCounts();

        double getEps() const;
        int getMinPoints() const;
        double score() const;

        static std::pair<DBScan, double> bestClusters(const std::vector<Point>&, DistanceFun, const std::pair<double, double>&, const std::pair<int, int>&, double);
    private:                                                               
        std::vector<int>                 clusters_assignments;
        double                           eps;
        int                              min_points;
        std::vector<std::vector<double>> distance_matrix;
        std::vector<std::vector<double>> points;
        DistanceFun                      distance_fun;
        std::vector<int>                 clusters_count;
        std::deque<int>                  neighbors;

        DBScan();
        void updateNeighbors(int);
        static std::vector<std::vector<double>> generateDistanceMatrix(const PointsArray&, DistanceFun);
        bool is(std::vector<bool>&, int);
        std::vector<int> fitPredict_helper();
    };

    DBScan::DBScan() 
        :distance_fun(NULL), eps(0), min_points(0){}

    DBScan::DBScan(DistanceFun distance_fun, double eps, int min_points)
        :distance_fun(distance_fun), eps(eps), min_points(min_points) {}

    int DBScan::getMinPoints() const {
        return min_points;
    }

    double DBScan::getEps() const
    {
        return eps;
    }

    std::vector<int> DBScan::fitPredict(const PointsArray& points) {
        this->points = points;
        distance_matrix = generateDistanceMatrix(points, distance_fun);
        return fitPredict_helper();
    }
     
    std::vector<int> DBScan::fitPredict_helper() {
        const int size = (int)distance_matrix.size();
        std::vector<bool> processed(size, false);
        int cluster_number = 0;

        clusters_assignments = std::vector<int>(size, -1);

        for (int i = 0; i < size; ++i) {
            if (is(processed, i)) continue;

            updateNeighbors(i);

            if (neighbors.empty()) continue;

            clusters_assignments[i] = cluster_number;
            clusters_count.push_back(1);

            while (!neighbors.empty()) {
                int j = neighbors.front();
                neighbors.pop_front();

                if (!is(processed, j)) updateNeighbors(j);

                if (clusters_assignments[j] == -1) {
                    clusters_assignments[j] = cluster_number;
                    clusters_count[cluster_number]++;
                }
            }

            cluster_number++;
        }

        return clusters_assignments;
    }

    std::vector<int> DBScan::getClustersCounts() {
        return clusters_count;
    }

    void DBScan::updateNeighbors(int pointIndex) {
        std::deque<int> neighbors;

        for (int i = 0; i < pointIndex; i++) {
            if (distance_matrix[pointIndex][i] <= eps) 
                neighbors.push_back(i);    
        }

        neighbors.push_back(pointIndex);

        for (int i = pointIndex + 1; i < distance_matrix.size(); i++) {
            if (distance_matrix[i][pointIndex] <= eps) {
                neighbors.push_back(i);
            }
        }

        if (neighbors.size() < min_points) return;

        this->neighbors.insert(this->neighbors.end(), neighbors.begin(), neighbors.end());
    }

    std::vector<std::vector<double>> DBScan::generateDistanceMatrix(const PointsArray& points, DistanceFun distance_fun) {
        int size = (int)points.size();

        std::vector<std::vector<double>> distance_matrix(size);

        for (int i = 0; i < size; i++)
        {
            distance_matrix[i] = std::vector<double>(i);

            for (int j = 0; j < i; j++) {

                distance_matrix[i][j] = distance_fun(points[i], points[j]);
            }
        }

        return distance_matrix;
    }

    bool DBScan::is(std::vector<bool>& bools , int index) {
        bool r = bools[index];
        bools[index] = true;

        return r;
    }

    double DBScan::score() const {
        Silhouette s(distance_matrix);
        return s.score(points, clusters_assignments);
    }

    std::pair<DBScan, double> DBScan::bestClusters(
        const std::vector<Point>& points,
        DistanceFun distance_fun,
        const std::pair<double, double>& eps,
        const std::pair<int, int>& min_points,
        double step
    ) {
        auto   distance_matrix = generateDistanceMatrix(points, distance_fun);
        double bestSilhouette  = -1.0;
        DBScan db_scan;

        for (int min_pts = min_points.first; min_pts <= min_points.second; ++min_pts) {
            std::cout << min_pts << " / " << min_points.second << "\r";
            
            for (double _eps = eps.first; _eps < eps.second + 0.0000001; _eps += step) {
                DBScan _db_scan(distance_fun, _eps, min_pts);
                _db_scan.distance_matrix = distance_matrix;
                _db_scan.points = points;
                auto out = _db_scan.fitPredict_helper();

                if (_db_scan.clusters_count.size() < 2) continue;

                double silhouette_ans = _db_scan.score();

                if (silhouette_ans <= bestSilhouette) continue; 

                bestSilhouette = silhouette_ans;
                db_scan = _db_scan;
            }
        }

        std::cout << std::endl;

        return std::make_pair(db_scan, bestSilhouette);
    }
}