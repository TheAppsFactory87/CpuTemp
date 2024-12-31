#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#ifdef _WIN32
    #include <windows.h>
#endif
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <future>
#include <memory>
#include <string>

template <typename T>
class Measurement {
public:
    Measurement()  {};
    virtual T readValue() = 0;
    void logValue(T value);
    void displayValue();

private:
    T value;
    std::string getCurrentTimestamp();
};

template <typename T>
void Measurement<T>::logValue(T value)
{
    std::ofstream file("temp_log.txt", std::ios::app);
    if (file.is_open()) {
        file << getCurrentTimestamp() << " " << value << std::endl;
        file.close();
    } else {
        std::cerr << "Unable to open log file.\n";
    }
}

template <typename T>
void Measurement<T>::displayValue()
{
  std::ifstream file("temp_log.txt");
    std::string lastLine, line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            lastLine = line;
        }
        file.close();
        std::cout << "Last logged value: " << lastLine << std::endl;
    } else {
        std::cerr << "Unable to open log file.\n";
    }
}

template <typename T>
std::string Measurement<T>::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &now_time);
    std::string timestamp(buffer);
    timestamp.pop_back(); // Remove trailing newline
    return timestamp;
}

#endif