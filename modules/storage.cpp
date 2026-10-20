#include "glib.h"
#include "gtk/gtk.h"
#include <cstdio>
#include <string>

#include "../headers/abstractHeaders.hpp"

class Storage : public AbstractModule {
    static std::string getStoragePercentage() {
        char buffer[256];
        std::string storagePercentage;
        FILE* resultCmd = popen("df -h | awk \'/\\/dev\\/nvme0n1p5/ {print $5}\'", "r");
        while (fgets(buffer, sizeof(buffer), resultCmd)) {
            storagePercentage += buffer;
        }

        if (storagePercentage.back() == '\n') storagePercentage.pop_back();
        pclose(resultCmd);
        return storagePercentage;
    }

    static gboolean updateStorage(gpointer label) {
        gtk_label_set_text(GTK_LABEL(label), getStoragePercentage().c_str());

        return G_SOURCE_CONTINUE;
    }

    public:
        GtkWidget* label;
        GtkWidget* storageIcon = gtk_label_new("\uea83");
        
        Storage() {
            label = gtk_label_new(getStoragePercentage().c_str());

            gtk_container_add(GTK_CONTAINER(container), label);
            gtk_container_add(GTK_CONTAINER(container), storageIcon);
            gtk_widget_set_name(container, "cpu-module");

            g_timeout_add(60000, updateStorage, label);
        }
};