#include "../headers/abstractHeaders.hpp"
#include "../headers/modulesHeaders.hpp"

class FactoryModule {
    public:
        static AbstractModule* createPowerButton() {
            return new PowerButton;
        }

        static AbstractModule* createClock() {
            return new Clock;
        }

        static AbstractModule* createBattery() {
            return new Battery;
        }
};