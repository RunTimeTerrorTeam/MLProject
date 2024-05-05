#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

class CSV
{
public:
	CSV(const std::string path) {
		_path = path;
		std::ifstream file(path);

		if (!file.is_open()) {
			throw std::invalid_argument("Error: Can't open file \"" + path + "\" make sure it's exists.");
		}

		std::string line;

		// get header
		std::getline(file, line);
		// line to cells -> strings
		_headers = convert<std::string>(line, [](std::string s) {return s; });

		while (std::getline(file, line))
		{
			std::vector<double> row = convert<double>(
				line,
				[](std::string s) { return atof(s.c_str()); }
			);

			_data.push_back(row);
		}

		file.close();
	}

	std::vector<std::string> headers() {
		return _headers;
	}

	std::vector<std::vector<double>> data() {
		return _data;
	}
	
	template <typename Any>
	void save(std::string newHeader, std::vector<Any> newData) {
		std::ofstream cloneFile(fileName(_path));
		
		for (const auto& header : _headers) {
			cloneFile << header << ",";
		}

		_headers.push_back(newHeader);
		cloneFile << newHeader << std::endl;

		int i = 0;
		for (auto& row : _data) {
			for (const auto& e : row) {
				cloneFile << std::to_string(e) + ",";
			}
			
			row.push_back(newData[i]);
			cloneFile << std::to_string(newData[i++]) << "\n";
		}

		cloneFile.close();
	}

private:
	std::string	                     _path;
	std::vector<std::string>         _headers;
	std::vector<std::vector<double>> _data;

	std::string fileName(std::string path) {
		auto ptr = path.rfind('/');
		return path.insert(++ptr, "(output)");
	}

	template<typename T>
	std::vector<T> convert(std::string line, T(*converter)(std::string)) {
		std::stringstream lineStream(line);
		std::string		  cell;
		std::vector<T>    row;

		while (std::getline(lineStream, cell, ','))
			row.push_back(converter(cell));

		return row;
	}
};