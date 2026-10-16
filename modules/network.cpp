#include "glib.h"
#include "gtk/gtk.h"
#include <cstdio>
#include <string>

#include "../headers/abstractHeaders.hpp"

class Network : public AbstractModule {
    static std::string getWifiName() {
        char buffer[256];
        std::string wifiName;
        FILE* resultCmd = popen("iwctl station wlan0 show | grep \"network\" | awk \'{print $3}\'", "r");
        while (fgets(buffer, sizeof(buffer), resultCmd)) {
            wifiName += buffer;
        }

        if (wifiName.back() == '\n') wifiName.pop_back();
        pclose(resultCmd);
        return wifiName;
    }

    static int getSignalStrength() {
        char buffer[256];
        std::string signalStrength;
        FILE* resultCmd = popen("iwctl station wlan0 show | grep \"AverageRSSI\" | awk \'{print $2}\'", "r");
        while (fgets(buffer, sizeof(buffer), resultCmd)) {
            signalStrength += buffer;
        }

        pclose(resultCmd);
        return std::stoi(signalStrength);
    }

    static bool isConnected() {
        char buffer[256];
        std::string connectionStat;
        FILE* resultCmd = popen("iwctl station wlan0 show | grep \"State\" | awk \'{print $2}\'", "r");
        while (fgets(buffer, sizeof(buffer), resultCmd)) {
            connectionStat += buffer;
        }

        pclose(resultCmd);
        return connectionStat == "connected\n";
    }

    static std::string getNetworkIconName() {
        int signalStrength = getSignalStrength();

        if (!isConnected()) {
            return "network-wireless-offline-symbolic";
        }

        return
            signalStrength >= -50 ? "network-wireless-signal-excellent-symbolic" :
            signalStrength >= -60 ? "network-wireless-signal-good-symbolic" :
            signalStrength >= -70 ? "network-wireless-signal-ok-symbolic" :
            signalStrength >= -80 ? "network-wireless-signal-weak-symbolic" :
            "network-wireless-none-symbolic";
    }

    static gboolean updateNetwork(gpointer data) {
        Network* networkModule = static_cast<Network*>(data);

        gtk_image_set_from_icon_name(
            GTK_IMAGE(networkModule->networkIcon),
            getNetworkIconName().c_str(),
            GTK_ICON_SIZE_MENU
        );

        gtk_label_set_text(GTK_LABEL(networkModule->label), getWifiName().c_str());

        return G_SOURCE_CONTINUE;
    }

    public:
        GtkWidget* label;
        GtkWidget* networkIcon;
        
        Network() {
            this->networkIcon = gtk_image_new_from_icon_name(
                getNetworkIconName().c_str(),
                GTK_ICON_SIZE_MENU
            );

            this->label = gtk_label_new(getWifiName().c_str());

            gtk_container_add(GTK_CONTAINER(container), label);
            gtk_container_add(GTK_CONTAINER(container), networkIcon);
            gtk_widget_set_name(container, "network-module");

            g_timeout_add(10000, updateNetwork, this);
        }
};