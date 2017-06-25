#include "NB_Weather_Client.h"

#include <fstream>

NB_Weather_Data& NB_Weather_Client::get_weather(const std::string zip, const std::string country_code)
{
	//look if it is already downloaded
	if (weather_data_.find(zip + country_code) != weather_data_.end())
	{
		std::cout << "\ndata hit\n";
		return weather_data_[zip + country_code];
	}

	//else get data
	std::cout << "\ndata miss\n";
	std::cout << "connecting..\n";


	socket_.async_connect(endpoint_, [this, zip, country_code](const asio::error_code error)
	{
		if (error)
			std::cerr << "async_connect: " << error.message() << std::endl;
		else
			this->connect_handler(zip, country_code);
	});

	//wait for the new data
	std::unique_lock<std::mutex> lock(mutex_);
	condition_variable_.wait(lock);

	return weather_data_[zip + country_code];
}

void NB_Weather_Client::connect_handler(const std::string zip, const std::string country_code)
{
	std::cout << "connected" << std::endl;

	//setup request
	const std::string k_http_request = 
		"GET "		+ k_open_wether_path	+ "?zip=" + zip + "," + country_code + k_open_wether_key + " HTTP/1.1\r\n"
		"Host: "	+ k_open_wether_url		+ "\r\n"
		"Connection: close\r\n\r\n";

	socket_.send(asio::buffer(k_http_request));

	std::cout << "waiting.." << std::endl << std::endl;


	std::array<char, 4096> message;
	
	auto size = socket_.read_some(asio::buffer(message));

	message[size] = '\0';
	std::string message_string{ message.data() };

	io_service_.post([this, message_string, zip, country_code]()
	{
		weather_data_[zip + country_code] = NB_Weather_Data{ message_string, zip };

		//notify main thread to return the new entry
		std::unique_lock<std::mutex> lock(mutex_);
		condition_variable_.notify_all();
	});
	socket_.close();
}

void NB_Weather_Client::save_to_file(const std::string file)
{
	std::ofstream ofstream{ file.c_str() };
	if (!ofstream)
		std::cerr << "could not open " << file << std::endl;

	for (auto data : weather_data_)
		ofstream << data.second << std::endl;
}
