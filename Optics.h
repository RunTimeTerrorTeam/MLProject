#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>


namespace Optics {

    class Point {
    public:
        std::vector<double> coordinates;
        double              reachability_distance;
        bool                processed;
        int                 point_index;

        Point(std::vector<double> _coordinates, int i)
            :coordinates(_coordinates), reachability_distance(INFINITY), processed(false), point_index(i) {}
    };



    class Optics {
    private:
        std::vector<std::vector<double>> distance_matrix;
        std::vector<Point> ordered_list;
        std::vector<Point> all_points;

        int     min_points;
        double  eps;

    public:
        Optics(std::vector<Point>& DB, double epsilon, int MinPts) : all_points(DB), min_points(MinPts), eps(epsilon),
            distance_matrix(generateDistanceMatrix(DB)) {}


        double manhatinDistance(Point p1, Point p2) {
            double dist = 0;
            for (size_t i = 0; i < p1.coordinates.size(); ++i) {
                dist += std::abs(p2.coordinates[i] - p1.coordinates[i]);
            }
            return dist;
        }

        double eculdian(Point p1, Point p2) {
            double dist = 0;
            for (size_t i = 0; i < p1.coordinates.size(); ++i) {
                dist += pow(p2.coordinates[i] - p1.coordinates[i], 2);
            }
            return sqrt(dist);
        }


        std::vector<Point*> getNeighbors(Point p) {
            std::vector<Point*> neighbors;

            for (const Point& q : all_points) {
                if (distance_matrix[p.point_index][q.point_index] <= eps) {
                    neighbors.push_back(const_cast<Point*>(&q));
                }
            }
            return neighbors;
        }




        double coreDistance(const Point p) {

            std::vector<double> distances_of_p;
            for (int i = 0; i < distance_matrix[p.point_index].size(); i++) {
                distances_of_p.push_back(distance_matrix[p.point_index][i]);
            }
            std::sort(distances_of_p.begin(), distances_of_p.end());

            double core_dis = distances_of_p[min_points - 1];

            if (core_dis > eps) return -1;

            return core_dis;
        }




        void update(std::vector<Point*>& N, Point p, std::priority_queue<Point*, std::vector<Point*>,
            bool(*)(const Point*, const Point*)>& Seeds) {

            double core_distance = coreDistance(p);

            for (Point* q : N) {
                if (!q->processed) {
                    double new_reachability_distance = std::max(core_distance, distance_matrix[p.point_index][q->point_index]);
                    if (new_reachability_distance < q->reachability_distance) {
                        q->reachability_distance = new_reachability_distance;
                        Seeds.push(q);
                    }
                }
            }
        }


        std::vector<std::vector<double>> generateDistanceMatrix(std::vector<Point>& all_points) {
            int size = (int)all_points.size();
            std::vector<std::vector<double>> distance_matrix(size, std::vector<double>(size));

            for (int i = 0; i < size; i++) {
                for (int j = i; j < size; j++) {
                    distance_matrix[i][j] = eculdian(all_points[i], all_points[j]);
                    distance_matrix[j][i] = eculdian(all_points[i], all_points[j]);
                }
            }
            return distance_matrix;
        }

        void fit() {
            std::priority_queue<Point*, std::vector<Point*>, bool(*)(const Point*, const Point*)> Seeds(
                [](const Point* p1, const Point* p2) {
                    return p1->reachability_distance > p2->reachability_distance;
                }
            );
            for (Point p : all_points) {
                if (!p.processed) {
                    std::vector<Point*> N = getNeighbors(p);

                    p.processed = true;

                    ordered_list.push_back(p);

                    double core_dist = coreDistance(p);

                    if (core_dist != -1.0) {
                        update(N, p, Seeds);

                        while (!Seeds.empty()) {
                            Point* q = Seeds.top();
                            Seeds.pop();
                            if (!q->processed) {
                                std::vector<Point*> N_prime = getNeighbors(*q);
                                q->processed = true;
                                ordered_list.push_back(*q);
                                core_dist = coreDistance(*q);
                                if (core_dist != -1.0) {
                                    update(N_prime, *q, Seeds);
                                }
                            }
                        }
                    }
                }
            }


        }

        void printClusters() {
            std::cout << "Ordered List:\n";
            for (Point p : ordered_list) {
                std::cout << "Coordinates: ";
                for (double coord : p.coordinates) {
                    std::cout << coord << " ";
                }
                std::cout << "- Reachability Distance: " << p.reachability_distance << "\n";
            }
        }

    };
}

