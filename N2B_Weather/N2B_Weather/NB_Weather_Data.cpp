#include "NB_Weather_Data.h"

#include <rapidjson/document.h>

#include <iostream>
#include <iomanip>
#include <string>

NB_Weather_Data::NB_Weather_Data(const std::string http_get_data, const std::string zip)
	:zip_(zip)
{
	std::size_t pos = http_get_data.find_first_of("{");
	std::string json_data = http_get_data.substr(pos);

	*this << json_data;
}

NB_Weather_Data& NB_Weather_Data::operator<<(const std::string json_data)
{
	//convert to rapidjson
	rapidjson::Document weather_data;
	weather_data.Parse(json_data.c_str());
	if (weather_data.HasParseError())
	{
		std::cerr << "could not parse weather_data" << std::endl;
		return *this;
	}

	//put data into object
	if (weather_data.HasMember("name"))
	{
		this->city_ = weather_data["name"].GetString();
	}
	if (weather_data.HasMember("main"))
	{
		if (weather_data["main"].HasMember("temp"))
		{
			this->temperature_ = weather_data["main"]["temp"].GetDouble();
		}
	}
	if (weather_data.HasMember("weather"))
	{
		this->weather_.main_weather = weather_data["weather"].GetArray()[0]["main"].GetString();
		this->weather_.description = weather_data["weather"].GetArray()[0]["description"].GetString();
	}
	if (weather_data.HasMember("wind"))
	{
		this->wind_.speed = weather_data["wind"]["speed"].GetDouble();
		if (weather_data["wind"].HasMember("deg"))
		{
			this->wind_.direction = weather_data["wind"]["deg"].GetDouble();
			this->wind_.direction_name = degree_to_string(wind_.direction);
		}
	}
	if (weather_data.HasMember("clouds"))
	{ 
		this->cloud_count_ = weather_data["clouds"]["all"].GetInt();
	}
	if (weather_data.HasMember("rain"))
	{
		this->last_3h_rain = weather_data["rain"]["3h"].GetDouble();
	}

	return *this;
}

std::ostream& operator<<(std::ostream & os, NB_Weather_Data& data)
{
	os << std::fixed << std::setprecision(2);
	os << "City:\t\t" << data.city_ << std::endl;
	os << "Weather:\t" << data.weather_.main_weather << std::endl;
	os << "\t\t" << data.weather_.description << std::endl;
	os << "Temperature:\t" << data.temperature_ - 273.15 << " C" << std::endl; //kelvin to celsius
	os << "Wind:\t\t" << data.wind_.speed << " m/s ";
	os << data.wind_.direction_name << std::endl;
	os << "Rain last 3h:\t" << data.last_3h_rain << std::endl;
	os << std::endl;
	return os;
}

std::string NB_Weather_Data::degree_to_string(double degree)
{
	if (degree < 22.5 || degree >= 337.5) return "North";
	if (degree >= 22.5 && degree < 67.5) return "North East";
	if (degree >= 67.5 && degree < 112.5) return "East";
	if (degree >= 112.5 && degree < 157.5) return "South East";
	if (degree >= 157.5 && degree < 202.5) return "South";
	if (degree >= 202.5 && degree < 247.5) return "South West";
	if (degree >= 247.5 && degree < 292.5) return "West";
	if (degree >= 292.5 && degree < 337.5) return "North West";
	return "should never be reached";
}
