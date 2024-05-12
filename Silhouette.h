#pragma once
#include <vector>
#include <iostream>
#include <vector>
#include <cmath>
#include <map>

typedef std::vector<double> Point;
typedef std::vector<Point> PointsArray;
using DistanceFun = double(*)(Point, Point);

class Silhouette
{
public:
    Silhouette(DistanceFun);
    Silhouette(const std::vector<std::vector<double>>&);

    double score(const std::vector<std::vector<double>>&, const std::vector<int>&);
private:
	DistanceFun distance_fun;
    PointsArray points;
    std::map<int, std::vector<int>> clusters;
    std::vector<int> clusters_assignment;
    std::vector<std::vector<double>> distance_matrix;
   
    double a(int, int);
    double b(int, int);
    void groupClusters(const std::vector<int>&);
};

Silhouette::Silhouette(DistanceFun distance_fun) :distance_fun(distance_fun) {}
Silhouette::Silhouette(const std::vector<std::vector<double>>& distance_matrix) :distance_matrix(distance_matrix), distance_fun(NULL) {}


double Silhouette::a(int pointIndex, int label) {
    double sum = 0.0;
    if (distance_fun != NULL) {
        for (const auto& i : clusters[label]) {
            sum += distance_fun(points[pointIndex], points[i]);
        }
    }
    else {
        int min, max;

        for (const auto& i : clusters[label]) {
            if (i == pointIndex) continue;

            if (i > pointIndex) {
                min = pointIndex;
                max = i;
            }
            else {
                min = i;
                max = pointIndex;
            }

            sum += distance_matrix[max][min];
        }
    }

    return sum / (clusters[label].size() - 1);
}

double Silhouette::b(int pointIndex, int label) {
    double min_avg = INFINITY;

    for (const auto& cluster : clusters) {
        if (cluster.first == label) continue;

        double sum = 0.0;

        if (distance_fun != NULL) {
            for (const auto& i : cluster.second) {
                sum += distance_fun(points[pointIndex], points[i]);
            }
        }
        else {
            int min, max;

            for (const auto& i : cluster.second) {
                if (i == pointIndex) continue;

                if (i > pointIndex) {
                    min = pointIndex;
                    max = i;
                } else {
                    min = i;
                    max = pointIndex;
                }

                sum += distance_matrix[max][min];
            }
        }

        min_avg = std::min(min_avg, sum / cluster.second.size());
    }
    return min_avg;
}

double Silhouette::score(const std::vector<std::vector<double>>& points, const std::vector<int>& clusters_assignment) {
    this->points = points;
    
    groupClusters(clusters_assignment);

    double silhouetteTotal = 0.0;

    for (const auto& cluster : clusters) {
        for (int i : cluster.second) {
            auto _a = a(i, cluster.first);
            auto _b = b(i, cluster.first);

            silhouetteTotal += (_b - _a) / std::max(_a, _b);
        }
    }
    return silhouetteTotal / points.size();
}

void Silhouette::groupClusters(const std::vector<int>& clusters_assignment) {
    clusters = std::map<int, std::vector<int>>();
    this->clusters_assignment = clusters_assignment;

    for (int i = 0; i < points.size(); ++i) {
        clusters[clusters_assignment[i]].push_back(i);
    }

    //clusters.erase(-1);
}
