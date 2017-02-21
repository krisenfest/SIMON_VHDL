#include <iostream>
#include <string>
//API define
	//used PI pins
	unsigned int controlPins[] = {2,3,4,5,6,7,8,9};
	unsigned int inputPins[] = {10,11,12,13,14,15,16,17};
	unsigned int outputPins[] = {18,19,20,21,22,23,24,25};
	

//GPIO utility functions
	#define IN  0
	#define OUT 1
	 
	#define LOW  0
	#define HIGH 1

	//dev functions
	bool GPIO_export(int pinNumber){return true;}
	bool GPIO_unexport(int pinNumber){return true;}
	bool GPIO_direction(int pinNumber, int direction){return true;}
	static int GPIO_read(int pinNumber){return 0;}
	bool GPIO_write(int pinNumber, int value){return true;}
	
/*
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>

	bool GPIO_export(int pinNumber){
		#define BUFFER_MAX 3

		char buffer[BUFFER_MAX];
		ssize_t bytes_written;
		int fd;
	 
		fd = open("/sys/class/gpio/export", O_WRONLY);
		if (-1 == fd) {
			fprintf(stderr, "GPIO_export::Failed to export pin\n");
			return false;
		}
	 
		bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pinNumber);
		write(fd, buffer, bytes_written);
		close(fd);
		return true;
	}
	
	bool GPIO_unexport(int pinNumber){
		char buffer[BUFFER_MAX];
		ssize_t bytes_written;
		int fd;
	 
		fd = open("/sys/class/gpio/unexport", O_WRONLY);
		if (-1 == fd) {
			fprintf(stderr, "GPIO_unexport::Failed to unexport pin\n");
			return false;
		}
	 
		bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pinNumber);
		write(fd, buffer, bytes_written);
		close(fd);
		return true;
	}

	bool GPIO_direction(int pinNumber, int direction){
		static const char s_directions_str[]  = "in\0out";
	 
		#define DIRECTION_MAX 35
		char path[DIRECTION_MAX];
		int fd;
	 
		snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pinNumber);
		fd = open(path, O_WRONLY);
		if (-1 == fd) {
			fprintf(stderr, "GPIO_direction::GPIO direction access failure\n");
			return false;
		}
	 
		if (-1 == write(fd, &s_directions_str[IN == direction ? 0 : 3], IN == direction ? 2 : 3)) {
			fprintf(stderr, "GPIO_direction::Failed to set direction\n");
			return false;
		}
	 
		close(fd);
		return true;
	}
	
	static int GPIO_read(int pinNumber){
		#define VALUE_MAX 30
		char path[VALUE_MAX];
		char value_str[3];
		int fd;
	 
		snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pinNumber);
		fd = open(path, O_RDONLY);
		if (-1 == fd) {
			fprintf(stderr, "GPIO_read::Failed to open gpio value for reading!\n");
			return(-1);
		}
	 
		if (-1 == read(fd, value_str, 3)) {
			fprintf(stderr, "GPIO_read::Failed to read value!\n");
			return(-1);
		}
	 
		close(fd);
	 
		return(atoi(value_str));
	}

	bool GPIO_write(int pinNumber, int value){
		static const char s_values_str[] = "01";
	 
		char path[VALUE_MAX];
		int fd;
	 
		snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pinNumber);
		fd = open(path, O_WRONLY);
		if (-1 == fd) {
			fprintf(stderr, "GPIO_write::Failed to open gpio value for writing!\n");
			return false;
		}
	 
		if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
			fprintf(stderr, "GPIO_write::Failed to write value!\n");
			return false;
		}
	 
		close(fd);
		return true;
	}
*/

bool setUpPins(){
	//export and set direction of control pins
		for(unsigned int a = 0; a < (sizeof(controlPins)/sizeof(*controlPins)); a++){
			if( !GPIO_export(controlPins[a]) ){std::cout << "- error: could not export control pin: " << a << std::endl; return false;}
			if( !GPIO_direction(controlPins[a],OUT) ){ std::cout << "- error: could not set direction of control pin: " << a << std::endl; return false; }
		}
    //export and set direction of input pins
		for(unsigned int a = 0; a < (sizeof(inputPins)/sizeof(*inputPins)); a++){
			if( !GPIO_export(inputPins[a]) ){std::cout << "- error: could not export input pin: " << a << std::endl; return false;}
			if( !GPIO_direction(inputPins[a],OUT) ){ std::cout << "- error: could not set direction of input pin: " << a << std::endl; return false; }
		}
    //export and set direction of output pins
		for(unsigned int a = 0; a < (sizeof(outputPins)/sizeof(*outputPins)); a++){
			if( !GPIO_export(outputPins[a]) ){std::cout << "- error: could not export output pin: " << a << std::endl; return false;}
			if( !GPIO_direction(outputPins[a],OUT) ){ std::cout << "- error: could not set direction of output pin: " << a << std::endl; return false; }
		}
		
	return true;
}

bool shutDownPins(){
	//unexport control pins
		for(unsigned int a = 0; a < (sizeof(controlPins)/sizeof(*controlPins)); a++){
			if( !GPIO_unexport(controlPins[a]) ){std::cout << "- error: could not unexport control pin: " << a << std::endl; return false;}
		}
    //unexport input pins
		for(unsigned int a = 0; a < (sizeof(inputPins)/sizeof(*inputPins)); a++){
			if( !GPIO_unexport(inputPins[a]) ){std::cout << "- error: could not unexport input pin: " << a << std::endl; return false;}
		}
    //unexport output pins
		for(unsigned int a = 0; a < (sizeof(outputPins)/sizeof(*outputPins)); a++){
			if( !GPIO_unexport(outputPins[a]) ){std::cout << "- error: could not unexport output pin: " << a << std::endl; return false;}
		}
		
	return true;	
}

std::string BINtoHEX(std::string binIn){
	//check if string can't be split into chunks of 4
	if(binIn.length() % 4 != 0){
		std::cout << "cannot turn bin string into hex string" << std::endl;
		std::cout << "bin string is: " << binIn.length() << std::endl;
		std::cout << "will only convert the first " << (unsigned int)(binIn.length()/4)*4 << " characters" << std::endl;
	}
	
	int value; 	std::string returnHex;
	for(unsigned int a = 0; a < (unsigned int)(binIn.length()/4)*4; a+=4){
		value = 0;
		if( binIn[a] == '1' ){value += 8;}
		if( binIn[a+1] == '1' ){value += 4;}
		if( binIn[a+2] == '1' ){value += 2;}
		if( binIn[a+3] == '1' ){value += 1;}
		
		switch(value){
			case 0:  returnHex += '0'; break;
			case 1:  returnHex += '1'; break;
			case 2:  returnHex += '2'; break;
			case 3:  returnHex += '3'; break;
			case 4:  returnHex += '4'; break;
			case 5:  returnHex += '5'; break;
			case 6:  returnHex += '6'; break;
			case 7:  returnHex += '7'; break;
			case 8:  returnHex += '8'; break;
			case 9:  returnHex += '9'; break;
			case 10: returnHex += 'a'; break;
			case 11: returnHex += 'b'; break;
			case 12: returnHex += 'c'; break;
			case 13: returnHex += 'd'; break;
			case 14: returnHex += 'e'; break;
			case 15: returnHex += 'f'; break;
		}
	}
	
	return returnHex;
}

std::string HEXtoBIN(std::string hexIn){
	std::string returnBin;
		
	for(unsigned int a = 0; a < hexIn.length(); a++){				
		switch(hexIn[a]){
			case '0': 			 returnBin += "0000"; break;
			case '1': 			 returnBin += "0001"; break;
			case '2': 			 returnBin += "0010"; break;
			case '3': 			 returnBin += "0011"; break;
			case '4': 			 returnBin += "0100"; break;
			case '5': 			 returnBin += "0101"; break;
			case '6': 			 returnBin += "0110"; break;
			case '7': 			 returnBin += "0111"; break;
			case '8': 			 returnBin += "1000"; break;
			case '9': 			 returnBin += "1001"; break;
			case 'a': case 'A':  returnBin += "1010"; break;
			case 'b': case 'B':  returnBin += "1011"; break;
			case 'c': case 'C':  returnBin += "1100"; break;
			case 'd': case 'D':  returnBin += "1101"; break;
			case 'e': case 'E':  returnBin += "1110"; break;
			case 'f': case 'F':  returnBin += "1111"; break;
			default: 
				std::cout << "unknown hex character: '" << hexIn[a] << "' inserting zero" << std::endl;
				returnBin += "0000";
			break;
		}
	}
	
	return returnBin;
}

std::string processMessage(bool mode, unsigned int method, std::string key, std::string message){
	setUpPins();
	
	
	
	shutDownPins();
	return "response";
}

int main(){
	std::cout << "Hello" << std::endl;
	
/*	//get mode
		std::string modeSel; bool mode;
		std::cout << "Select mode (enc or dec): "; std::cin >> modeSel;
		if(modeSel == "enc"){ mode = false;}
		else if(modeSel == "dec"){ mode = true; }
		else{ std::cout << "- error: mode '" << modeSel << "' is not available" << std::endl; return 0; }
	//get method	
		unsigned int method;
		std::cout << "Select Method (0-9): "; std::cin >> method;
		if(method > 9){ std::cout << "- error: method '" << method << "' is not available" << std::endl; return 0;}
	//get key
		std::string key; unsigned int keyLengthRequirement;
		std::cout << "Required key length: ";
		switch(method){
			case 0: keyLengthRequirement = 16; break;
			case 1: keyLengthRequirement = 18; break;
			case 2: keyLengthRequirement = 24; break;
			case 3: keyLengthRequirement = 24; break;
			case 4: keyLengthRequirement = 32; break;
			case 5: keyLengthRequirement = 24; break;
			case 6: keyLengthRequirement = 36; break;
			case 7: keyLengthRequirement = 32; break;
			case 8: keyLengthRequirement = 48; break;
			case 9: keyLengthRequirement = 64; break;
		}		
		std::cout << keyLengthRequirement << std::endl;
		std::cout << "	Insert key: "; std::cin >> key;
		if(key.length() != keyLengthRequirement){
			std::cout << "- error: provided key is the wrong length" << std::endl;
			std::cout << "	required: " << keyLengthRequirement << std::endl;
			std::cout << "	provided: " << key.length() << std::endl;
			return 0;
		}
		key = HEXtoBIN(key);
	//get message
		std::string message; unsigned int messageLengthRequirement;
		std::cout << "Required message length: ";
		switch(method){
			case 0: messageLengthRequirement = 8; break;
			case 1: messageLengthRequirement = 12; break;
			case 2: messageLengthRequirement = 12; break;
			case 3: messageLengthRequirement = 16; break;
			case 4: messageLengthRequirement = 16; break;
			case 5: messageLengthRequirement = 24; break;
			case 6: messageLengthRequirement = 24; break;
			case 7: messageLengthRequirement = 32; break;
			case 8: messageLengthRequirement = 32; break;
			case 9: messageLengthRequirement = 32; break;
		}		
		std::cout << messageLengthRequirement << std::endl;
		std::cout << "	Insert message: "; std::cin >> message;
		if(message.length() != messageLengthRequirement){
			std::cout << "- error: provided message is the wrong length" << std::endl;
			std::cout << "	required: " << messageLengthRequirement << std::endl;
			std::cout << "	provided: " << message.length() << std::endl;
			return 0;
		}
		message = HEXtoBIN(message);*/
		
		
		bool mode = false; unsigned int method = 0; std::string key = "1111222244445555"; std::string message = "11224455";
	//process
		std::string returnedMessage = processMessage(mode,method,key,message);
	
		std::cout << std::endl;
		std::cout << "Returned Message: " << returnedMessage << std::endl;
	
	return 0;
}