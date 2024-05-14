#pragma once

#include "ReadCSV.h"
#include "WriteCSV.h"

class CSV
{
public:
	static ReadCSV read(const std::string& path) {
		return ReadCSV(path);
	}

	static WriteCSV write(const std::string& path) {
		return WriteCSV(path);
	}
};