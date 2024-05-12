#pragma once
#include <vector>
#include <string>

namespace Agglomerative {
	#define V1D std::vector<double>

	typedef std::vector<double> Point;
	typedef std::vector<Point> PointsArray;
	using DistanceFun = double(*)(Point, Point);
	using LinkageFun = double(*)(double, double);

	enum LINKAGE { single, complete, average };

	class Agglomerative
	{
	public:
		Agglomerative(DistanceFun, const int&, const LINKAGE&);
		std::vector<int> fitPredict(const PointsArray&);
		
	private:
		std::vector<std::vector<int>>		  clusters;
		std::vector<std::vector<double>>      distance_matrix;
		DistanceFun                           distance_fun;
		LinkageFun                            linkage_fun;
		int                                   n_cluster;

		void generateDistanceMatrix(const PointsArray&);
		std::pair<int, int> findMinIndex();
		void updateDistanceMatrix(const std::pair<int, int>&);
		void updateClusterAssignment(const std::pair<int, int>&);
		void generateClusterAssignment(const PointsArray&);
	};

	Agglomerative::Agglomerative(DistanceFun distance_fun, const int& n_cluster = 2, const LINKAGE& linkage = LINKAGE::single)
		: distance_fun(distance_fun), n_cluster(n_cluster) {

		switch (linkage)
		{
			case LINKAGE::single:
				linkage_fun = [](double p1, double p2) { return std::min(p1, p2); };
				break;
			case LINKAGE::complete:
				linkage_fun = [](double p1, double p2) { return std::max(p1, p2); };
				break;
			case LINKAGE::average:
				linkage_fun = [](double p1, double p2) { return (p1 + p2) / 2.0; };
				break;
			default:
				throw std::invalid_argument("Error: linkage not found");
		}
	}

	std::vector<int> Agglomerative::fitPredict(const PointsArray& points) {
		generateClusterAssignment(points);
		generateDistanceMatrix(points);
		

		while (distance_matrix.size() > n_cluster) {
			auto miniIndex = findMinIndex();
			updateDistanceMatrix(miniIndex);
			updateClusterAssignment(miniIndex);
		}

		std::vector<int> cluster_assignment(points.size());

		for (int i = 0; i < clusters.size(); i++) {
			for (int j : clusters[i]) {
				cluster_assignment[j] = i;
			}
		}

		return cluster_assignment;
	}

	void Agglomerative::generateDistanceMatrix(const PointsArray& points) {
		int size = (int)points.size();
		
		distance_matrix = std::vector<std::vector<double>>(size);

		for (int i = 0; i < size; i++)
		{	
			distance_matrix[i] = std::vector<double>(i);
			
			for (int j = 0; j < i; j++) {
				distance_matrix[i][j] = distance_fun(points[i], points[j]);
			}
		}
	}

	std::pair<int, int> Agglomerative::findMinIndex() {
		std::pair<int, int> index;
		double min = INFINITY;

		for (int i = 0; i < distance_matrix.size(); i++) {
			for (int j = 0; j < distance_matrix[i].size(); j++) {
				if (distance_matrix[i][j] >= min) continue;

				min = distance_matrix[i][j];
				index.first = i;
				index.second = j;
			}
		}

		return index;
	}

	// Hint: index.second < index.first always
	void Agglomerative::updateDistanceMatrix(const std::pair<int, int>& index)
	{	
		// row <=> row
		for (int i = 0; i < index.second; i++) {
			distance_matrix[index.second][i] = linkage_fun(distance_matrix[index.first][i], distance_matrix[index.second][i]);
		}

		// col <=> row
		for (int i = index.second + 1; i < index.first; i++) {
			distance_matrix[i][index.second] = linkage_fun(distance_matrix[index.first][i], distance_matrix[i][index.second]);
		}

		// col <=> col
		for (int i = index.first + 1; i < distance_matrix.size(); i++) {
			distance_matrix[i][index.second] = linkage_fun(distance_matrix[i][index.first], distance_matrix[i][index.second]);
			distance_matrix[i].erase(distance_matrix[i].begin() + index.first);
		}

		distance_matrix.erase(distance_matrix.begin() + index.first);
	}

	void Agglomerative::Agglomerative::generateClusterAssignment(const PointsArray& points)
	{
		int size = (int)points.size();
		clusters = std::vector<std::vector<int>>(size);
		for (int i = 0; i < size; i++) clusters[i] = { i };
	}

	void Agglomerative::updateClusterAssignment(const std::pair<int, int>& miniIndex) {
		auto& cluster1 = clusters[miniIndex.first];
		auto& cluster2 = clusters[miniIndex.second];
		
		cluster2.insert(cluster2.end(), cluster1.begin(), cluster1.end());
		clusters.erase(clusters.begin() + miniIndex.first);
	}
}
