#include"serialport.h"
#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<array>
#include<memory>
#include<ctime>

void setParameter(int const &dtparam , std::string& msg,const int start_indx)
{
    if (dtparam < 10)
    {
        msg.at(start_indx) = '0';
        msg.at(start_indx+1) = '0' + dtparam;
    }
    else
    {
        msg.at(start_indx) = '0' + (dtparam / 10);
        msg.at(start_indx+1) = '0' + (dtparam % 10);
    }
}
struct serialParams 
{
    unsigned long baudrate;
    std::string port;
    serialParams() : baudrate(BAUD_57600), port("COM8") {};
    ~serialParams() {};
};
serialParams readConfigFile()
{
    std::ifstream f;
    f.open("config.ini", std::ios::in);
    serialParams param;
    if (f.good())
    {
        std::string parser;
        std::getline(f,parser,';');
        const std::string com = "COM";
        auto pos = parser.find(com);
        auto startpos_com_number=pos + com.length();
        if (startpos_com_number < parser.length())
        {
            param.port = std::string("\\\\.\\") + com + parser.substr(startpos_com_number, parser.length() - startpos_com_number);
            std::cout << "read port from file : " << param.port << std::endl;
        }
        f.close();
    }
    return param;
}
int main()
{
    auto config = readConfigFile();
    SerialPort s{ config.port.data() };
    std::string msg1 = "DATE12:12:2022";
    std::string msg2 = "TIME12:12:12";
    int choice;
    bool stop_menu=false;
    do
    {
    std::cout << "Menu" << std::endl;
    std::cout << "****" << std::endl;
    std::cout << "[1] Synchronize system date and time to clock" << std::endl;
    std::cout << "[2] Update custom date to clock" << std::endl;
    std::cout << "[3] Update custom time to clock" << std::endl;
    std::cout << "[4] Exit" << std::endl;
    std::cout << "Choose an option" << std::endl;
    std::cin >> choice;

        switch (choice)
        {
        case 1:
        {// current date/time based on current system
            tm gmtm;
            time_t now = time(0);
            localtime_s(&gmtm, &now);
            setParameter(gmtm.tm_hour, msg2, 4);
            setParameter(gmtm.tm_min, msg2, 7);
            setParameter(gmtm.tm_sec, msg2, 10);
            setParameter(gmtm.tm_mday, msg1, 4);
            setParameter(1 + gmtm.tm_mon, msg1, 7);
            setParameter((1900 + gmtm.tm_year) / 100, msg1, 10);
            setParameter((1900 + gmtm.tm_year) % 100, msg1, 12);
        }
        break;
        case 3:
        {   tm gmtm;
            std::cout << "Enter Hours in range of [0-23] : ";
            std::cin >> gmtm.tm_hour;
            std::cout << "Enter Mins in range of [0-59] : ";
            std::cin >> gmtm.tm_min;
            std::cout << "Enter Sec in range of [0-59] : ";
            std::cin >> gmtm.tm_sec;
            setParameter(gmtm.tm_hour, msg2, 4);
            setParameter(gmtm.tm_min, msg2, 7);
            setParameter(gmtm.tm_sec, msg2, 10);
        }
        break;
        case 2:
        { tm gmtm;
        std::cout << "Enter day of the month in range of [0-31] : ";
        std::cin >> gmtm.tm_mday;
        std::cout << "Enter month of the year in range of [1-12] : ";
        std::cin >> gmtm.tm_mon;
        std::cout << "Enter year in YYYY format : ";
        std::cin >> gmtm.tm_year;
        setParameter(gmtm.tm_mday, msg1, 4);
        setParameter(gmtm.tm_mon, msg1, 7);
        setParameter((gmtm.tm_year) / 100, msg1, 10);
        setParameter((gmtm.tm_year) % 100, msg1, 12);
        }
        break;
        default:
            stop_menu = true;
            break;
        }

    if (choice == 1 || choice == 2)
    {
        std::cout << "date string" << msg1 << std::endl;
        if (s.isConnected())
        {
            s.writeSerialPort(msg1.c_str(), msg1.length());
            std::cout << "date set!" << std::endl;
        }
    }
    Sleep(ARDUINO_WAIT_TIME);
    if (choice == 1 || choice == 3)
    {
        std::cout << "time string" << msg2 << std::endl;
        if (s.isConnected())
        {
            s.writeSerialPort(msg2.c_str(), msg2.length());
            std::cout << "time set!" << std::endl;
        }
        else
        {
            std::cout << "error device not found. check config file for correct port or ensure that the clock is connected." << std::endl;
        }
    }
    } while (!stop_menu);
#ifdef READ_ENABLED
    std::unique_ptr<std::array<char, MAX_DATA_LENGTH>> buffer{ std::make_unique<std::array<char, MAX_DATA_LENGTH>>() };
    if (s.isConnected())
    {
        auto bytesRead = s.readSerialPort(buffer.get()->data(), MAX_DATA_LENGTH);
        if (bytesRead > 0)
        {
            buffer.get()->at(bytesRead - 1) = '\0';
            puts(buffer.get()->data());
        }
    }
    else
    {
        std::cout << "error device not found. check config file for correct port or ensure that the clock is connected." << std::endl;
    }
#endif
}

