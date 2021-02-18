#include <iostream>
#include <chrono>
#include <ctime>

#include "Logger.h"

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));
    return(output);
}

void Logger::Log(const std::string &message)
{
    LogEntry newEntry;
    newEntry.type = LOG_INFO;
    
    newEntry.message = "LOG [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[32m" << newEntry.message << "\033[0m" << std::endl;

    messages.push_back(newEntry);
}

void Logger::Err(const std::string &message)
{
    LogEntry newEntry;
    newEntry.type = LOG_ERROR;
    newEntry.message = "ERR: [" + CurrentDateTimeToString() + "]: " + message;
    std::cerr << "\x1B[91m" << newEntry.message << "\033[0m" << std::endl;
    messages.push_back(newEntry);
}