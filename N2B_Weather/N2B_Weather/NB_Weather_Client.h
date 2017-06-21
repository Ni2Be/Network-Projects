#ifndef NB_WEATHER_CLIENT_H
#define NB_WEATHER_CLIENT_H
#pragma once

//asio
#define WINVER 0x0A00 
#define _WIN32_WINNT 0x0A00
#define ASIO_STANDALONE
#include <asio.hpp>

using asio::ip::tcp;

//stdlib
#include <array>
#include <map>
#include <thread>

//
#include "NB_Weather_Data.h"

//openWeahter
const std::string k_open_wether_url = "api.openweathermap.org";
const std::string k_open_wether_path = "/data/2.5/weather";
const std::string k_open_wether_key = "&APPID=88ec1eac36032997dd5ff32aa0a95aa7";


class NB_Weather_Client
{
public:
	NB_Weather_Client()
		:
		socket_{ io_service_ },
		resolver_{ io_service_ },
		query_{ k_open_wether_url, "80"}
	{
		endpoint_ = *resolver_.resolve(query_);
	}

	NB_Weather_Data& get_weather(const std::string zip, const std::string country_code);
	void save_to_file(const std::string);
private:
	asio::io_service io_service_;
	tcp::socket socket_;
	tcp::resolver resolver_;
	tcp::resolver::query query_;
	tcp::endpoint endpoint_;

	std::map<std::string, NB_Weather_Data> weather_data;

	void connect_handler(const std::string zip, const std::string country_code);
};
#endif // !NB_WEATHER_CLIENT_H
