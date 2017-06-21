#ifndef NB_WEATHER_DATA_H
#define NB_WEATHER_DATA_H
#pragma once

#include <iostream>


class NB_Weather_Data
{
public:
	NB_Weather_Data() {}
	NB_Weather_Data(const std::string http_get_data, const std::string zip);
	
	std::string zip_;
	std::string city_;
	double temperature_;
	int cloud_count_;
	double last_3h_rain;

	struct Weather
	{
		std::string main_weather;
		std::string description;
	} weather_;

	struct Wind
	{
		double speed;
		double direction;
		std::string direction_name;
	} wind_;

	friend std::ostream& operator<<(std::ostream& os, NB_Weather_Data& data);
private:
	NB_Weather_Data& operator<<(const std::string right);
	std::string degree_to_string(double degree);
};

std::ostream& operator<<(std::ostream& os, NB_Weather_Data& data);
#endif // !NB_WEATHER_DATA_H
