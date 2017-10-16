# [Network-Projects](https://github.com/Nice2Bee/Network-Projects/tree/master)
## N2B_Weather
A program that will ask for a country-code like “us” and then ask for ZIP-codes.
After it gets the needed input it will return the actual weather of the asked location.<br><br>

\[there are tab errors as you see]


![image](https://github.com/Nice2Bee/Network-Projects/blob/N2B_Weather/N2B_Weather/screenshot.PNG)


out of [NB_Weather_Client.h](https://github.com/Nice2Bee/Network-Projects/blob/N2B_Weather/N2B_Weather/N2B_Weather/NB_Weather_Client.h):
```
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
  ```
