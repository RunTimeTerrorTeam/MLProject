#pragma once
#include <vector>
#include <string>

namespace Agglomerative {
#define V1D std::vector<double>

	typedef std::vector<double> Point;
	typedef std::vector<Point> PointsArray;
	using DistanceFun = double(*)(Point, Point);

	class Agglomerative
	{
	public:
		Agglomerative(DistanceFun);
		std::vector<std::vector<std::string>> fitPredict(const PointsArray&);
		
		std::vector<std::vector<std::vector<double>>> distance_matrices;
	private:
		std::vector<std::string>                      current_labels;
		std::vector<std::vector<std::string>>         labels;
		std::vector<std::vector<double>>              distance_matrix;
		DistanceFun                                   distance_fun;

		void generateDistanceMatrix(const PointsArray&);
		std::pair<int, int> findMinIndex();
		V1D mergingRows(const std::pair<int, int>&);
		void shrink(const int&);
		void insertCluster(V1D, int);


		void generateLabels(const int&);
	};

	Agglomerative::Agglomerative(DistanceFun distance_fun)
		: distance_fun(distance_fun) {}

	std::vector<std::vector<std::string>> Agglomerative::fitPredict(const PointsArray& points) {
		generateDistanceMatrix(points);
		generateLabels(points.size());
		
		distance_matrices.push_back(distance_matrix);

		while (distance_matrix.size() != 1) {
			auto miniIndex = findMinIndex();
			auto cluster = mergingRows(miniIndex);
			shrink(miniIndex.second);
			insertCluster(cluster, miniIndex.first);
			distance_matrices.push_back(distance_matrix);
		}

		return labels;
	}

	void Agglomerative::generateDistanceMatrix(const PointsArray& points) {
		int size = (int)points.size();
		
		distance_matrix = std::vector<std::vector<double>>(points.size(), std::vector<double>(size));

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				distance_matrix[i][j] = distance_fun(points[i], points[j]);
			}
		}
	}

	std::pair<int, int> Agglomerative::findMinIndex() {
		std::pair<int, int> index;
		double min = INFINITY;

		for (int i = 0; i < distance_matrix.size(); i++) {
			for (int j = 0; j < distance_matrix[i].size(); j++) {
				if (distance_matrix[i][j] >= min || distance_matrix[i][j] == 0) continue;

				min = distance_matrix[i][j];
				index.first = i;
				index.second = j;
			}
		}

		return index;
	}

	V1D Agglomerative::mergingRows(const std::pair<int, int>& index) {
		V1D rows[2] = { distance_matrix[index.first], distance_matrix[index.second] };

		rows[0].erase(rows[0].begin() + index.second);
		rows[1].erase(rows[1].begin() + index.second);

		int size = (int)rows[1].size();
		auto merging_row = V1D(size);

		for (int i = 0; i < size; i++) {
			merging_row[i] = std::min(rows[0][i], rows[1][i]);
		}

		
		current_labels[index.first] = current_labels[index.first] + ", " + current_labels[index.second];
		current_labels.erase(current_labels.begin() + index.second);
		labels.push_back(current_labels);

		return merging_row;
	}

	// TODO: Optimize that
	void Agglomerative::shrink(const int& index) {
		// delete row
		distance_matrix.erase(distance_matrix.begin() + index);

		// delete col
		for (int i = 0; i < distance_matrix.size(); i++) {
			for (int j = 0; j < distance_matrix[i].size(); j++) {
				if (j != index) continue;
					
				distance_matrix[i].erase(distance_matrix[i].begin() + index);
			}
		}
	}

	void Agglomerative::insertCluster(V1D updated_distance, int index) {
		distance_matrix[index] = updated_distance;

		for (int i = 0; i < distance_matrix.size(); i++)
		{
			for (int j = 0; j < distance_matrix.size(); j++) {
				if (j == index && i != index) {
					distance_matrix[i][j] = updated_distance[i];
				}
			}
		}
	}

	void Agglomerative::generateLabels(const int& size) {
		current_labels = std::vector<std::string>(size);

		for (int i = 0; i < size; i++) {
			current_labels[i] = "p" + std::to_string(i + 1);
		}
		
		labels.push_back(current_labels);
	}
}
