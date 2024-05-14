#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

namespace Optics {
    struct Condition
    {
        std::vector<double>* array;

        Condition() = default;
        Condition(std::vector<double>* array) : array(array) {}

        bool operator()(const int l, const int r) const {
            return &array[l] > &array[r];
        }
    };
    
    typedef std::vector<double> Point;
    typedef std::vector<Point> PointsArray;
    using DistanceFun = double(*)(Point, Point);
    typedef std::priority_queue<int, std::vector<int>, Condition> Seed;


    class Optics {
    public:
        Optics(DistanceFun, double, int);
        std::pair<std::vector<int>, std::vector<double>> fitPredict(const std::vector<Point>&);

    private:
        std::vector<std::vector<double>> distance_matrix;
        std::vector<int>                 ordered_list;
        std::vector<Point>               points;
        DistanceFun                      distance_fun;
        int                              min_points;
        double                           eps;
        std::vector<bool>                processed;
        Seed                             seeds;
        std::vector<double>              reachability_distance;

        double getDistance(const int&, const int&);
        std::vector<int> getNeighbors(const int&);
        double coreDistance(const int&, const std::vector<int>&);
        bool is(std::vector<bool>&, const int&);
        void updateSeeds(const int&, const double&, const std::vector<int>&);
        std::vector<std::vector<double>> generateDistanceMatrix(const std::vector<Point>&);
    };

    Optics::Optics(DistanceFun distance_fun, double epsilon, int MinPts)
        :distance_fun(distance_fun), min_points(MinPts), eps(epsilon) {}

    std::pair<std::vector<int>, std::vector<double>> Optics::fitPredict(const std::vector<Point>& points) {
        int size = (int)points.size();

        this->points = points;
        distance_matrix = generateDistanceMatrix(points);
        processed = std::vector<bool>(size, false);
        reachability_distance = std::vector<double>(size, INFINITY);
        seeds = Seed(Condition(&reachability_distance));

        for (int i = 0; i < size; i++) {
            if (is(processed, i)) continue;

            auto neighbors = getNeighbors(i);
            double core_dist = coreDistance(i, neighbors);

            if (core_dist == -1.0) continue;

            updateSeeds(i, core_dist, neighbors);

            while (!seeds.empty()) {
                int q = seeds.top();
                seeds.pop();

                if (is(processed, q)) continue;

                neighbors = getNeighbors(q);
                core_dist = coreDistance(q, neighbors);

                if (core_dist == -1.0) continue;

                updateSeeds(q, core_dist, neighbors);
            }
        }

        return { ordered_list, reachability_distance };
    }

    double Optics::getDistance(const int& i, const int& j) {
        if (i == j) return 0;
        if (j > i) return distance_matrix[j][i];
        return distance_matrix[i][j];
    }

    std::vector<int> Optics::getNeighbors(const int& pointIndex) {
        std::vector<int> neighbors;

        for (int i = 0; i < distance_matrix.size(); i++) {
            if (getDistance(pointIndex, i) <= eps)
                neighbors.push_back(i);
        }

        return neighbors;
    }

    double Optics::coreDistance(const int& p, const std::vector<int>& neighbors) {
        if (neighbors.size() < min_points) return -1;
        std::vector<double> distances_of_p(neighbors.size());

        for (int i = 0; i < neighbors.size(); i++)
            distances_of_p.push_back(getDistance(p, neighbors[i]));

        std::sort(distances_of_p.begin(), distances_of_p.end());

        return distances_of_p[min_points - 1];
    }

    bool Optics::is(std::vector<bool>& bools, const int& index) {
        if (bools[index]) return true;

        bools[index] = true;
        ordered_list.push_back(index);

        return false;
    }

    void Optics::updateSeeds(const int& pointIndex, const double& core_distance, const std::vector<int>& neighbors) {
        for (auto q : neighbors) {
            if (processed[q]) continue;

            double new_reachability_distance = std::max(core_distance, getDistance(pointIndex, q));

            if (new_reachability_distance >= reachability_distance[q]) continue;

            reachability_distance[q] = new_reachability_distance;
            seeds.push(q);
        }
    }


    std::vector<std::vector<double>> Optics::generateDistanceMatrix(const std::vector<Point>& points) {
        int size = (int)points.size();
        std::vector<std::vector<double>> distance_matrix(size);

        for (int i = 0; i < size; i++) {
            distance_matrix[i] = std::vector<double>(i);
            for (int j = 0; j < i; j++) {
                distance_matrix[i][j] = distance_fun(points[i], points[j]);
            }
        }
        return distance_matrix;
    }
}

