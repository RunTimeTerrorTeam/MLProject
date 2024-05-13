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

    typedef std::priority_queue<Point*, std::vector<Point*>, bool(*)(const Point*, const Point*)> Seed;


    class Optics {
    private:
        std::vector<std::vector<double>> distance_matrix;
        std::vector<Point> ordered_list;
        std::vector<Point> all_points;

        int     min_points;
        double  eps;

    public:
        Optics(std::vector<Point>& DB, double epsilon, int MinPts) : all_points(DB), min_points(MinPts), eps(epsilon),
        distance_matrix(generateDistanceMatrix(DB)) { }


        double manhatinDistance(Point p1, Point p2) {
            double dist = 0;
            for (int i = 0; i < p1.coordinates.size(); ++i) {
                dist += std::abs(p1.coordinates[i] - p2.coordinates[i]);
            }
            return dist;
        }

        double eculdian(Point p1, Point p2) {
            double dist = 0;
            for (int i = 0; i < p1.coordinates.size(); ++i) {
                dist += pow(p1.coordinates[i] - p2.coordinates[i], 2);
            }
            return sqrt(dist);
        }

        double getDistnace(int i, int j) {
            if (j > i) return distance_matrix[j][i];
            return distance_matrix[i][j];
        }


        std::vector<Point*> getNeighbors(Point p) {
            std::vector<Point*> neighbors;

            for (const Point& q : all_points) {
                if (getDistnace(p.point_index, q.point_index) <= eps) {
                    neighbors.push_back(const_cast<Point*>(&q));
                }
            }
            return neighbors;
        }




        double coreDistance(const Point p, std::vector<Point*> N) {
            if (N.size() < min_points) return -1;
            std::vector<double> distances_of_p;

            for (auto n: N) distances_of_p.push_back(getDistnace(p.point_index ,n->point_index));
            
            std::sort(distances_of_p.begin(), distances_of_p.end());
            return distances_of_p[min_points - 1];
        }




        void update(std::vector<Point*>& N, Point p, Seed& Seeds) {
            double core_distance = coreDistance(p, N);

            for (Point* q : N) {
                if (!q->processed) {
                    double new_reachability_distance = std::max(core_distance, getDistnace(p.point_index,q->point_index));
                    if (new_reachability_distance < q->reachability_distance) {
                        q->reachability_distance = new_reachability_distance;
                        Seeds.push(q);
                    }
                }
            }
        }


        std::vector<std::vector<double>> generateDistanceMatrix(std::vector<Point>& all_points) {
            int size = (int)all_points.size();
            std::vector<std::vector<double>> distance_matrix(size);

            for (int i = 0; i < size; i++) {
                distance_matrix[i] = std::vector<double>(i + 1);
                for (int j = 0; j < i + 1; j++) {
                    distance_matrix[i][j] = manhatinDistance(all_points[i], all_points[j]);
                }
            }
            return distance_matrix;
        }



        static bool condition(const Point* p1, const Point* p2) {
            return p1->reachability_distance > p2->reachability_distance;
        };

        void fit() {
            Seed Seeds(condition);

            for (Point& p : all_points) {
                if (!p.processed) {

                    std::vector<Point*> N = getNeighbors(p);

                    p.processed = true;

                    ordered_list.push_back(p);

                    double core_dist = coreDistance(p, N);
                    
                    if (core_dist != -1.0) {
                        update(N, p, Seeds);

                        while (!Seeds.empty()) {
                            Point* q = Seeds.top();
                            Seeds.pop();
                            if (!q->processed) {
                                std::vector<Point*> N = getNeighbors(*q);

                                q->processed = true;
                                ordered_list.push_back(*q);
                                core_dist = coreDistance(*q, N);

                                if (core_dist != -1.0) {
                                    update(N, *q, Seeds);
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

        void getClusters() {
            std::map<double, std::vector<int>> o_map;
            for (Point p : ordered_list) {
                o_map[p.reachability_distance].push_back(p.point_index);

            }
            std::cout << "--------------------------------------------\n";
            for (auto m : o_map) {
                std::cout << "cluster with reachibility " << m.first << "  ===>  ";
                for (auto v : m.second) {
                    std::cout << v << " ";
                }
                std::cout << "\n";
            }
        }

    };
}

