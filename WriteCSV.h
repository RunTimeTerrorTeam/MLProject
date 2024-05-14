#pragma once
#include <string>
#include <vector>
#include <fstream>

class WriteCSV
{
public:
	WriteCSV(const std::string& path) 
		:path(path) {}

	template <typename Any>
	void append(std::string newHeader, std::vector<Any> newData, std::string(*converter)(Any) = std::to_string) {
		if (rows.empty()) rows.push_back(newHeader);
		else rows[0] += "," + newHeader;
		
		for (int i = 0; i < newData.size(); i++) {
			if (i >= rows.size() - 1) rows.push_back(converter(newData[i]));
			else rows[i + 1] += "," + converter(newData[i]);
		}
	}

	void save() {
		std::ofstream file(fileName(path));

		if (!file.is_open())
			throw std::invalid_argument("Error: Can't open file \"" + path + "\" something wrong.");
		
		for (const auto& row : rows) {
			file << row << std::endl;
		}

		file.close();
	}

private:
	std::string path;
	std::vector<std::string> rows;

	std::string fileName(std::string path) {
		auto ptr = path.rfind('/');
		return path.insert(++ptr, "(output)");
	}
};

