#include "glib.h"
#include "gtk/gtk.h"
#include <fstream>
#include <string>

#include "../headers/abstractHeaders.hpp"

class Battery : public AbstractModule {
    static gboolean updateBattery(gpointer data) {
        Battery* batteryModule = static_cast<Battery*>(data);

        std::string capacity;
        std::ifstream MyReadFile("/sys/class/power_supply/BAT0/capacity");
        getline(MyReadFile, capacity);

        gtk_image_set_from_icon_name(
            GTK_IMAGE(batteryModule->batteryIcon),
            getBatteryIconName(std::stoi(capacity)).c_str(),
            GTK_ICON_SIZE_MENU
        );

        std::string batteryText = capacity + "%";

        gtk_label_set_text(GTK_LABEL(batteryModule->label), batteryText.c_str());

        return G_SOURCE_CONTINUE;
    }

    static bool isCharging() {
        std::string capacity;
        std::ifstream MyReadFile("/sys/class/power_supply/BAT0/status");
        getline(MyReadFile, capacity);

        return capacity == "Charging";
    }

    static std::string getBatteryIconName(int capacity) {
        std::string iconName =
            capacity == 100 ? "battery-level-100" :
            capacity >= 90 ? "battery-level-90" :
            capacity >= 80 ? "battery-level-80" :
            capacity >= 70 ? "battery-level-70" :
            capacity >= 60 ? "battery-level-60" :
            capacity >= 50 ? "battery-level-50" :
            capacity >= 40 ? "battery-level-40" :
            capacity >= 30 ? "battery-level-30" :
            capacity >= 20 ? "battery-level-20" :
            capacity >= 10 ? "battery-level-10" :
            "battery-level-0";

        if (isCharging()) {
            iconName.append(capacity == 100 ? "-charged" : "-charging");
        }

        iconName.append("-symbolic");

        return iconName;
    }

    public:
        GtkWidget* label;
        GtkWidget* batteryIcon;
        
        Battery() {
            std::string capacity;
            std::ifstream MyReadFile("/sys/class/power_supply/BAT0/capacity");
            getline(MyReadFile, capacity);

            this->batteryIcon = gtk_image_new_from_icon_name(
                getBatteryIconName(std::stoi(capacity)).c_str(),
                GTK_ICON_SIZE_MENU
            );

            std::string batteryText = capacity + "%";
            this->label = gtk_label_new(batteryText.c_str());

            gtk_container_add(GTK_CONTAINER(container), label);
            gtk_container_add(GTK_CONTAINER(container), batteryIcon);
            gtk_widget_set_name(container, "battery-module");

            g_timeout_add(1000, updateBattery, this);
        }
};