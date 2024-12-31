#include "Measurement.h"
#include "Temperature.h"


void readTemperature(Temperature* temp) {
    while(true) {
        double temperature = 0;
        temperature = temp->readValue();
        temp->logValue(temperature);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void displayTemperature(Temperature* temp) {

    while(true) {
        temp->displayValue();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::unique_ptr<Temperature>  temperature = std::make_unique<Temperature>();
    

    std::thread readerThread(readTemperature, temperature.get());
    std::thread displayThread(displayTemperature, temperature.get());

    std::this_thread::sleep_for(std::chrono::seconds(10));

    readerThread.detach();
    displayThread.detach();

    return 0;
}