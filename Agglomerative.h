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
		
	private:
		std::vector<std::string>              current_labels;
		std::vector<std::vector<std::string>> labels;
		std::vector<std::vector<double>>      distance_matrix;
		DistanceFun                           distance_fun;

		void generateLabels(const int&);
		void generateDistanceMatrix(const PointsArray&);
		std::pair<int, int> findMinIndex();
		void updateDistanceMatrix(const std::pair<int, int>&);
		void updateLabels(const std::pair<int, int>&);
	};

	Agglomerative::Agglomerative(DistanceFun distance_fun)
		: distance_fun(distance_fun) {}

	std::vector<std::vector<std::string>> Agglomerative::fitPredict(const PointsArray& points) {
		generateDistanceMatrix(points);
		generateLabels((int)points.size());

		while (distance_matrix.size() != 1) {
			auto miniIndex = findMinIndex();
			updateDistanceMatrix(miniIndex);
			updateLabels(miniIndex);
		}

		return labels;
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
			distance_matrix[index.second][i] = std::min(distance_matrix[index.first][i], distance_matrix[index.second][i]);
		}

		// col <=> row
		for (int i = index.second + 1; i < index.first; i++) {
			distance_matrix[i][index.second] = std::min(distance_matrix[index.first][i], distance_matrix[i][index.second]);
		}

		// col <=> col
		for (int i = index.first + 1; i < distance_matrix.size(); i++) {
			distance_matrix[i][index.second] = std::min(distance_matrix[i][index.first], distance_matrix[i][index.second]);
			distance_matrix[i].erase(distance_matrix[i].begin() + index.first);
		}

		distance_matrix.erase(distance_matrix.begin() + index.first);
	}

	void Agglomerative::generateLabels(const int& size) {
		current_labels = std::vector<std::string>(size);

		for (int i = 0; i < size; i++) {
			current_labels[i] = "p" + std::to_string(i + 1);
		}
		
		labels.push_back(current_labels);
	}

	void Agglomerative::updateLabels(const std::pair<int, int>& miniIndex) {
		current_labels[miniIndex.second] += ", " + current_labels[miniIndex.first];
		current_labels.erase(current_labels.begin() + miniIndex.first);
		labels.push_back(current_labels);
	}
}
