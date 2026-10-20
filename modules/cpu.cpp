#include "glib.h"
#include "gtk/gtk.h"
#include <cstdio>
#include <iomanip>
#include <ios>
#include <string>

#include "../headers/abstractHeaders.hpp"

class Cpu : public AbstractModule {
    static std::string getCpuPercentage() {
        char buffer[256];
        std::string cpuData;
        FILE* resultCmd = popen("top -bn 1 | awk '/%Cpu/ {print $8}'", "r");
        while (fgets(buffer, sizeof(buffer), resultCmd)) {
            cpuData += buffer;
        }

        std::ostringstream stream;
        stream << std::fixed << std::setprecision(1) << 100 - std::stod(cpuData);

        std::string cpuPercentage = stream.str();

        pclose(resultCmd);
        return cpuPercentage;
    }

    static gboolean updateCpu(gpointer label) {
        std::string cpuText = getCpuPercentage() + "%";

        gtk_label_set_text(GTK_LABEL(label), cpuText.c_str());

        return G_SOURCE_CONTINUE;
    }

    public:
        GtkWidget* label;
        GtkWidget* cpuIcon = gtk_label_new("\uf4bc");
        
        Cpu() {
            std::string cpuText = getCpuPercentage() + "%";
            label = gtk_label_new(cpuText.c_str());

            gtk_container_add(GTK_CONTAINER(container), label);
            gtk_container_add(GTK_CONTAINER(container), cpuIcon);
            gtk_widget_set_name(container, "cpu-module");

            g_timeout_add(1000, updateCpu, label);
        }
};