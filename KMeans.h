#pragma once
#include <iostream>
#include <vector>

namespace KMeans {
    typedef std::vector<double> Point;
    typedef std::vector<Point> PointsArray;
    using DistanceFun = double(*)(Point, Point);

    class KMeans
    {
    public:
        // Construct the distance function is used in algorithm, number of clusters, and number of iterations
        KMeans(DistanceFun, int, int);

        // Power of algorithm get data and return assigns clusters
        std::vector<int> fitPredict(const PointsArray&);
        // Return last executed data centroids
        PointsArray getCentroids();
        // Return last executed data clusters
        std::vector<int> getClusterAssignments();
        // Return last executed data points count in every cluster
        std::vector<int> getPointsCountAroundCentroids();

    private:
        int              n_clusters;
        PointsArray      centroids;
        DistanceFun      distance_fun;
        int              max_iteration;
        PointsArray      points;
        std::vector<int> cluster_counts;
        std::vector<int> cluster_assignments;

        // Choose random points from data
        void setRandomCentroids();
        // Assign data points to clusters
        bool updateClusters();
        // Get min distance between point and every centroid
        int findNearestCentroid(const Point&);
        // Get average points in every cluster
        void updateCentroids();
    };

    KMeans::KMeans(DistanceFun distance_fun, int n_clusters = 2, int max_iteration = 100)
        : n_clusters(n_clusters), distance_fun(distance_fun), max_iteration(max_iteration) {}

    std::vector<int> KMeans::fitPredict(const PointsArray& points) {
        this->points = points;
        cluster_assignments = std::vector<int>(points.size(), -1);

        setRandomCentroids();

        for (int i = 0; i < max_iteration; i++) {
            bool clusters_is_updated = updateClusters();

            if (!clusters_is_updated)
                break;

            updateCentroids();
        }

        return cluster_assignments;
    }

    PointsArray KMeans::getCentroids() {
        return centroids;
    }

    std::vector<int> KMeans::getPointsCountAroundCentroids() {
        return cluster_counts;
    }

    std::vector<int> KMeans::getClusterAssignments() {
        return cluster_assignments;
    }


    void KMeans::setRandomCentroids() {
        const int size = (int)points.size();

        centroids = PointsArray(n_clusters);

        for (int i = 0; i < n_clusters; ++i) {
            centroids[i] = points[rand() % size];
        }
    }

    bool KMeans::updateClusters() {
        bool centroidsChanged = false;

        // set to every point the new centroid
        for (int i = 0; i < points.size(); ++i) {
            int nearestCentroid = findNearestCentroid(points[i]);

            if (cluster_assignments[i] == nearestCentroid)
                continue;

            cluster_assignments[i] = nearestCentroid;
            centroidsChanged = true;
        }

        return centroidsChanged;
    }

    int KMeans::findNearestCentroid(const Point& point) {
        int nearestIndex = 0;
        double minDistance = distance_fun(point, centroids[0]);

        // search to min distance between point and centroids
        for (int i = 1; i < centroids.size(); ++i) {
            double distance = distance_fun(point, centroids[i]);

            if (distance < minDistance) {
                minDistance = distance;
                nearestIndex = i;
            }
        }
        return nearestIndex;
    }

    void KMeans::updateCentroids() {
        int n_row = (int)points.size();
        int n_col = (int)points[0].size();

        cluster_counts = std::vector<int>(n_clusters, 0);
        centroids = PointsArray(n_clusters, Point(n_col, 0.0));

        // get summation has same clusters and count of them
        for (int i = 0; i < n_row; ++i) {
            int clusterIndex = cluster_assignments[i];

            cluster_counts[clusterIndex]++;

            for (int j = 0; j < n_col; ++j) {
                centroids[clusterIndex][j] += points[i][j];
            }
        }

        // get average
        for (int i = 0; i < n_clusters; ++i) {
            if (cluster_counts[i] == 0) continue;

            for (int j = 0; j < n_col; ++j) {
                centroids[i][j] /= cluster_counts[i];
            }
        }

    }
}