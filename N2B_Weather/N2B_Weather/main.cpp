#include <iostream>

#include "NB_Weather_Data.h"
#include "NB_Weather_Client.h"

int main()
{
	std::cout << "Weather Client:" << std::endl;
	std::cout << "powerd by: OpenWeatherMap.com and RapidJson" << std::endl << std::endl;
	std::cout << "enter negative zip code to exit (like -1)" << std::endl << std::endl;
	NB_Weather_Client weather_client;

	std::string country_code;
	std::cout << "enter country code(us, de, etc.): ";
	std::cin >> country_code;

	while (true)
	{
		std::string zip;
		std::cout << "enter zip: ";
		std::cin >> zip;
		if (std::stoi(zip) < 0)
			break;
		std::cout << std::endl << weather_client.get_weather(zip, country_code) << std::endl;
	}

	weather_client.save_to_file("save_file.txt");

	std::cout << "!!END!!" << std::endl;
	char ch;
	std::cin >> ch;
}
