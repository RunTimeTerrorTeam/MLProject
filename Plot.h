#pragma once
#include <vector>
#include <string>

#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <map>

const double COLORS[][3] = {
	{0,0,0},
	{0.390625,0.453125,0.542969},
	{0.878906,0.113281,0.28125},
	{0.53125,0.0742188,0.214844},
	{0.953125,0.445312,0.710938},
	{0.289062,0.015625,0.304688},
	{0.75,0.148438,0.824219},
	{0.417969,0.128906,0.65625},
	{0.308594,0.273438,0.894531},
	{0.0742188,0.304688,0.289062},
	{0.289062,0.867188,0.5},
	{0.515625,0.796875,0.0859375},
	{0.988281,0.875,0.277344},
	{0.46875,0.207031,0.0585938},
	{0.21875,0.738281,0.96875},
	{0.367188,0.914062,0.828125},
	{0.75,0.515625,0.984375},
};

class Plot
{
public:
	Plot(const int&, const int&);
	void scatter(const std::vector<std::vector<double>>&, const std::vector<int>&);
	void scatter(const std::vector<std::vector<double>>&, const std::string&);
	void title(const std::string&);
	void xLabel(const std::string&);
	void yLabel(const std::string&);
	void show();
	void plot(const std::vector<double>&, const std::vector<double>&);
	~Plot();
private:
	RGBABitmapImageReference* imageReference;
	ScatterPlotSettings* settings;
	std::string _title;
	StringReference* errorMessage;
	std::map<int, std::pair<std::vector<double>, std::vector<double>>> storage;
	std::vector<std::pair<std::vector<double>, std::vector<double>>>   storage2;
	double diam[2][2];

	void set(std::vector<wchar_t>&, const std::string&);
	std::string getErrors();
	void updateStorage(const std::vector<std::vector<double>>&, const std::vector<int>&);
	void updateStorage(const std::vector<std::vector<double>>&);
};

Plot::Plot(const int& width = 1400, const int& height = 700) {
	settings = GetDefaultScatterPlotSettings();
	settings->width = width;
	settings->height = height;

	imageReference = CreateRGBABitmapImageReference();
	errorMessage = CreateStringReferenceLengthValue(0, L' ');

	settings->autoBoundaries = false;
	settings->autoPadding = true;

	settings->title = toVector(L"");
	settings->xLabel = toVector(L"");
	settings->yLabel = toVector(L"");

	_title = "untitled";
	
	diam[0][0] = diam[1][0] = INFINITY;
	diam[1][0] = diam[1][1] = 0;
}

void Plot::scatter(const std::vector<std::vector<double>>& data2D, const std::vector<int>& clusters_assignment)
{
	updateStorage(data2D, clusters_assignment);
	
	for (auto& d : storage) {
		ScatterPlotSeries* series = GetDefaultScatterPlotSeriesSettings();
		series->xs = &d.second.first;
		series->ys = &d.second.second;
		series->linearInterpolation = false;
		series->pointType = toVector(L"dots");
		series->color = CreateRGBColor(COLORS[d.first + 1][0], COLORS[d.first + 1][1], COLORS[d.first + 1][2]);

		settings->scatterPlotSeries->push_back(series);
	}
}

void Plot::scatter(const std::vector<std::vector<double>>& data2D, const std::string& shape = "filled triangles")
{
	updateStorage(data2D);

	ScatterPlotSeries* dots_series2 = GetDefaultScatterPlotSeriesSettings();
	dots_series2->xs = &storage2.back().first;
	dots_series2->ys = &storage2.back().second;
	dots_series2->linearInterpolation = false;
	dots_series2->pointType = toVector(L"dots");
	dots_series2->color = CreateRGBColor(0.996094, 0.839844, 0);

	settings->scatterPlotSeries->push_back(dots_series2);

	ScatterPlotSeries* dots_series = GetDefaultScatterPlotSeriesSettings();
	dots_series->xs = &storage2.back().first;
	dots_series->ys = &storage2.back().second;
	dots_series->linearInterpolation = false;
	dots_series->pointType = toVector(L"circles");
	dots_series->color = CreateRGBColor(0, 0, 0);

	settings->scatterPlotSeries->push_back(dots_series);
}

void Plot::title(const std::string& title) {
	set(*settings->title, title);
	_title = title;
}

void Plot::xLabel(const std::string& label) {
	set(*settings->xLabel, label);
}
void Plot::yLabel(const std::string& label) {
	set(*settings->yLabel, label);
}

void Plot::set(std::vector<wchar_t>& key, const std::string& value) {
	std::wstring _value(value.begin(), value.end());
	key = *toVector(_value.c_str());
}

void Plot::show()
{
	settings->xMin = diam[0][0] - 0.5;
	settings->xMax = diam[0][1] + 0.5;
	settings->yMin = diam[1][0] - 0.5;
	settings->yMax = diam[1][1] + 0.5;

	bool success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);

	if (!success) {
		throw std::invalid_argument(getErrors());
	}

	std::vector<double>* png_data = ConvertToPNG(imageReference->image);
	WriteToFile(png_data, _title + ".png");
	DeleteImage(imageReference->image);

	system(("start ./\"" + _title + ".png\"").c_str());
}

std::string Plot::getErrors()
{
	std::string errors = "";
	
	errors += "Error: ";
	
	for (wchar_t c : *errorMessage->string) {
		errors += c;
	}
	
	return errors;
}

void Plot::updateStorage(const std::vector<std::vector<double>>& data, const std::vector<int>& cluster_assignment)
{	
	for (int i = 0; i < data.size(); ++i) {
		storage[cluster_assignment[i]].first.push_back(data[i][0]);
		storage[cluster_assignment[i]].second.push_back(data[i][1]);

		if (data[i][0] < diam[0][0]) diam[0][0] = data[i][0];
		if (data[i][0] > diam[0][1]) diam[0][1] = data[i][0];
		if (data[i][1] < diam[1][0]) diam[1][0] = data[i][1];
		if (data[i][1] > diam[1][1]) diam[1][1] = data[i][1];
	}
}

void Plot::updateStorage(const std::vector<std::vector<double>>& data) {
	storage2.push_back({});
	
	for (int i = 0; i < data.size(); ++i) {
		storage2.back().first.push_back(data[i][0]);
		storage2.back().second.push_back(data[i][1]);

		if (data[i][0] < diam[0][0]) diam[0][0] = data[i][0];
		if (data[i][0] > diam[0][1]) diam[0][1] = data[i][0];
		if (data[i][1] < diam[1][0]) diam[1][0] = data[i][1];
		if (data[i][1] > diam[1][1]) diam[1][1] = data[i][1];
	}
}

void Plot::plot(const std::vector<double>& x_axis, const std::vector<double>& y_axis)
{
	storage2.push_back({x_axis, y_axis});

	for (int i = 0; i < x_axis.size(); ++i) {
		if (x_axis[i] < diam[0][0]) diam[0][0] = x_axis[i];
		if (x_axis[i] > diam[0][1]) diam[0][1] = x_axis[i];
		if (y_axis[i] < diam[1][0]) diam[1][0] = y_axis[i];
		if (y_axis[i] > diam[1][1]) diam[1][1] = y_axis[i];
	}

	ScatterPlotSeries* series = GetDefaultScatterPlotSeriesSettings();
	series->xs = &storage2.back().first;
	series->ys = &storage2.back().second;
	series->linearInterpolation = true;
	series->color = CreateRGBColor(0, 0, 0);

	settings->scatterPlotSeries->push_back(series);

	ScatterPlotSeries* dots_series = GetDefaultScatterPlotSeriesSettings();
	dots_series->xs = &storage2.back().first;
	dots_series->ys = &storage2.back().second;
	dots_series->linearInterpolation = false;
	dots_series->pointType = toVector(L"dots");
	dots_series->color = CreateRGBColor(0, 0, 0);

	settings->scatterPlotSeries->push_back(dots_series);
}

Plot::~Plot() {
	FreeAllocations();
}