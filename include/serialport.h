#pragma once

#define ARDUINO_WAIT_TIME 5000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <iostream>
 
class SerialPort
{
public:
	/// \brief  : create a Handle to the serial port with a given port name.
	/// \param  : portName - string specified "\\\\.\\COMX" X is the port number 
    explicit SerialPort( char const * const portName);
    ~SerialPort();
	
	/// \brief  : Read a string from the open serial connection to the buffer.
	/// \param  : buffer   : string buffer
	/// \param  : buf_size : buffer size
    /// \return : number of characters read from the open serial connection without including the null terminator.
	int readSerialPort(char const * buffer, unsigned int const buf_size);
    
	/// \brief  : Write a string from the buffer to the open serial connection.
	/// \param  : buffer : input string buffer
	/// \param  : buf_size : input buffer size
    /// \return : true when serial communication handler encounters no error
	/// \return : false when serial communication handler encounters an error
	bool writeSerialPort(char const * buffer, unsigned int const buf_size);
    
	/// \brief  : check if a serial connection is established
	/// \return : true when serial communication handler encounters no error
	/// \return : false when serial communication handler encounters an error
	bool isConnected();
    
	/// \brief : terminate an open serial connection
	void closeSerial();

private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
};