#include "glib.h"
#include "gtk/gtk.h"
#include <cstdio>
#include <iomanip>
#include <ios>
#include <string>

#include "../headers/abstractHeaders.hpp"

class Ram : public AbstractModule {
    static std::string getRamPercentage() {
        char buffer[256];
        std::string ramData;
        FILE* resultCmd = popen("free -h | awk \'/^Mem:/ {gsub(/Gi/, \"\", $2); gsub(/Gi/, \"\", $3); print $2, \";\", $3}\'", "r");
        while (fgets(buffer, sizeof(buffer), resultCmd)) {
            ramData += buffer;
        }

        float ramTotal = std::stof(ramData.substr(0, ramData.find(";")));

        std::string ramUsed = (ramData.substr(ramData.find(";") + 2, ramData.size()));
        if (ramUsed.back() == '\n') ramUsed.pop_back();

        double ramCalcul = (std::stof(ramUsed) * 100) / ramTotal;

        std::ostringstream stream;
        stream << std::fixed << std::setprecision(1) << ramCalcul;

        std::string ramPercentage = stream.str();

        pclose(resultCmd);
        return ramPercentage;
    }

    static gboolean updateRam(gpointer label) {
        std::string ramText = getRamPercentage() + "%";

        gtk_label_set_text(GTK_LABEL(label), ramText.c_str());

        return G_SOURCE_CONTINUE;
    }

    public:
        GtkWidget* label;
        GtkWidget* ramIcon = gtk_label_new("\uEFC5");
        
        Ram() {
            std::string volumeText = getRamPercentage() + "%";
            label = gtk_label_new(volumeText.c_str());

            gtk_container_add(GTK_CONTAINER(container), label);
            gtk_container_add(GTK_CONTAINER(container), ramIcon);
            gtk_widget_set_name(container, "ram-module");

            g_timeout_add(100, updateRam, label);
        }
};