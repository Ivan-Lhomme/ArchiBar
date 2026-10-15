#include "glib.h"
#include "gtk/gtk.h"

#include "../headers/abstractHeaders.hpp"

class Clock : public AbstractModule {
    static gboolean updateClock(gpointer clockLabel) {
        time_t now = time(nullptr);
        struct tm* localTime = localtime(&now);
        char currentTime[10];

        strftime(currentTime, 10, "%H:%M:%S", localTime);

        gtk_label_set_text(GTK_LABEL(clockLabel), currentTime);

        return G_SOURCE_CONTINUE;
    }

    public:
        GtkWidget* label;
        
        Clock() {
            time_t now = time(nullptr);
            struct tm* localTime = localtime(&now);
            char currentTime[10];

            strftime(currentTime, 10, "%H:%M:%S", localTime);

            this->label = gtk_label_new(currentTime);

            gtk_container_add(GTK_CONTAINER(container), label);
            gtk_widget_set_name(container, "clock-module");

            g_timeout_add(1000, updateClock, label);
        }
};