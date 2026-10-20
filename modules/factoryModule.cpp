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

        static AbstractModule* createVolume() {
            return new Volume;
        }

        static AbstractModule* createNetwork() {
            return new Network;
        }

        static AbstractModule* createRam() {
            return new Ram;
        }

        static AbstractModule* createCpu() {
            return new Cpu;
        }
};