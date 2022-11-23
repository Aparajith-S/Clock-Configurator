
#include "serialport.h"

/// \brief  : create a Handle to the serial port with a given port name.
SerialPort::SerialPort(const char* portName)
{
    this->connected = false;

    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (this->handler == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            std::cerr << "ERROR: Handle was not attached.Reason : " << portName << " not available\n";
        }
        else
        {
            std::cerr << "ERROR!!!\n";
        }
    }
    else
    {
        DCB dcbSerialParameters = { 0 };

        if (!GetCommState(this->handler, &dcbSerialParameters))
        {
            std::cerr << "Failed to get current serial parameters\n";
        }
        else
        {
            dcbSerialParameters.BaudRate = CBR_57600;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                std::cout << "ALERT: could not set serial port parameters\n";
            }
            else
            {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }
}

SerialPort::~SerialPort()
{
    if (this->connected)
    {
        this->connected = false;
        CloseHandle(this->handler);
    }
}

/// \brief  : Read a string from the open serial connection to the buffer.
int SerialPort::readSerialPort(const char* buffer, unsigned int buf_size)
{
    DWORD bytesRead{};
    unsigned int toRead = 0;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0)
    {
        if (this->status.cbInQue > buf_size)
        {
            toRead = buf_size;
        }
        else
        {
            toRead = this->status.cbInQue;
        }
    }

    memset((void*)buffer, 0, buf_size);

    if (ReadFile(this->handler, (void*)buffer, toRead, &bytesRead, NULL))
    {
        return bytesRead;
    }

    return 0;
}

/// \brief  : Write a string from the buffer to the open serial connection.
bool SerialPort::writeSerialPort(const char* buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, 0))
    {
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    }

    return true;
}

/// \brief  : check if a serial connection is established
bool SerialPort::isConnected()
{
    if (!ClearCommError(this->handler, &this->errors, &this->status))
    {
        this->connected = false;
    }

    return this->connected;
}

/// \brief : terminate an open serial connection
void SerialPort::closeSerial()
{
    CloseHandle(this->handler);
}