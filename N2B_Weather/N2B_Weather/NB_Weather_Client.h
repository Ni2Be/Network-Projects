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
#include <condition_variable>
#include <mutex>

//
#include "NB_Weather_Data.h"

//openWeahter
const std::string k_open_wether_url = "api.openweathermap.org";
const std::string k_open_wether_path = "/data/2.5/weather";
const std::string k_open_wether_key = "&APPID=88ec1eac36032997dd5ff32aa0a95aa7"; 
// yes this is my key, but its free. Use it for test purposes, 
// or go to openweathermap.org and get your own.




/*
NB_Weather_Client
purpose: 
	NB_Weather_Client will setup anything needed to get a 
	connection to the openWeatherMap API.
	It has a function get_weather(zip, country_code) that allows
	to ask for a NB_Weather_Data of the location.
	It also stores all gethered data in a map and if a request
	is allready in there it wont do a new http request.

	It has also a function save_to_file(file_name) that saves all
	the geathered entrys to a file

working method:
	NB_Weather_Client will start a new thread in it's consturctor
	that will handle the asynconous work.
		At the Moment there is no real need to do this, quite the reverse
		it complicades the thing only. 
		But I wanted to test std::condition_variable, and asio is named asio
		and not sio so I think it is the better way.

	After initialization the function get_weather(zip, country_code)
	will proofe if the data is already saved, if not it will 
	setup a new socket that will handle the connection. 
	
	The calling thread will now push the function connect_handler() into
	the asio io_service work_pool and then wait until the work is done.

	At the same time the work_thread will (if he is not bussy) begin to work.
	The function connect_handler() will generate a http request for the
	given zip and country_code and send it (syncroniously) 
	to api.openweathermap.org and then recieve (also syncroniously) 
	the answer as a JSON formatted string.
	
	After that it will push the parthing aka the creation of the 
	NB_Weather_Data object into the asio io_service work_pool.

	Then it will close the socket and after that the work_thread is
	free again to handle the work it pushed just a moment ago into the
	work_pool.
	After the generation of the NB_Weather_Data the work_thread will
	notify the main that the work is done and that it can now return
	the new NB_Weather_Data.
	

	Then is there the function save_to_file() which saves the weather_data
	with the calling thread to a file.
*/
class NB_Weather_Client
{
public:
	NB_Weather_Client()
		:
		socket_		{ io_service_ },
		resolver_	{ io_service_ },
		query_		{ k_open_wether_url, "80" },
		work_		{ io_service_ }
	{
		work_thread_	= new std::thread{ [this]() { this->io_service_.run(); } };
		endpoint_		= *resolver_.resolve(query_);
	}

	~NB_Weather_Client()
	{
		if (work_thread_->joinable())
			work_thread_->join();
	}

	NB_Weather_Data&	get_weather	(const std::string zip, const std::string country_code);
	void				save_to_file(const std::string);
private:
	asio::io_service						io_service_;

	tcp::socket								socket_;
	tcp::resolver							resolver_;
	tcp::resolver::query					query_;
	tcp::endpoint							endpoint_;

	asio::io_service::work					work_;
	std::thread*							work_thread_;

	std::condition_variable					condition_variable_;
	std::mutex								mutex_;

	std::map<std::string, NB_Weather_Data>	weather_data_;

	void				connect_handler(const std::string zip, const std::string country_code);
};
#endif // !NB_WEATHER_CLIENT_H
