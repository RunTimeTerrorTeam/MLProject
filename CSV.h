#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

class CSV
{
public:
	CSV(){}
	CSV(const std::string path) {
		std::ifstream file(path);
		_path = path;
		std::cout << path;
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
	}

	std::vector<std::string> headers() {
		return _headers;
	}

	std::vector<std::vector<double>> data() {
		return _data;
	}
	
	template <typename Any>void save(std::string newHeader, std::vector<Any> newData) {
		
		std::ofstream cloneFile(fileName(_path));
		_headers.push_back(newHeader);
		for (int i = 0; i < newData.size(); i++) {
			_data[i].push_back(newData[i]);
		}
		std::string headers="";
		for (int i = 0; i < _headers.size(); i++) {
			i != _headers.size()-1?headers += _headers[i] + "," : headers += _headers[i];
		}
		cloneFile << headers+"\n";
		for (int i = 0; i < _data.size(); i++) {
			for (int j = 0; j < _data[i].size(); j++)
				cloneFile << std::to_string(_data[i][j])+",";
			cloneFile << "\n";
		}
	}

	static CSV& getInstance(std::string path) {
		if (!obj) {
			obj = path!="" ? new CSV(path) : new CSV;
		}
		return *obj;
	}

private:
	std::string	                     _path;
	std::vector<std::string>         _headers;
	std::vector<std::vector<double>> _data;
	std::string fileName(std::string path) {
		auto ptr = path.rfind('/');
		return path.insert(++ptr, "(output)");
	}
	static CSV* obj;


	template<typename T>
	// typedef int T;
	std::vector<T> convert(std::string line, T(*converter)(std::string)) {
		std::stringstream lineStream(line);
		std::string		  cell;
		std::vector<T>    row;

		while (std::getline(lineStream, cell, ','))
			row.push_back(converter(cell));

		return row;
	}
};
CSV* CSV::obj = nullptr;
