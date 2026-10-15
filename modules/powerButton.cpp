#include "../headers/abstractHeaders.hpp"
#include "gdk/gdk.h"
#include "glib-object.h"
#include "gtk/gtk.h"

class PowerButton : public AbstractModule {
    static void onClick(GtkWidget* button) {
        gtk_widget_destroy(gtk_widget_get_toplevel(button));
    }

    static void setCursor(GtkWidget* eventBox) {
        GdkCursor* cursor = gdk_cursor_new_from_name(gtk_widget_get_display(eventBox), "pointer");
        gdk_window_set_cursor(gtk_widget_get_window(eventBox), cursor);

        g_object_unref(cursor);
    }

    public:
        GtkWidget* eventBox;
        GtkWidget* button;

        PowerButton() {
            this->eventBox = gtk_event_box_new();
            this->button = gtk_button_new_from_icon_name("system-shutdown", GTK_ICON_SIZE_BUTTON);

            gtk_container_add(GTK_CONTAINER(container), eventBox);
            gtk_container_add(GTK_CONTAINER(eventBox), button);
            gtk_widget_set_name(container, "power-button-module");

            g_signal_connect(eventBox, "realize", G_CALLBACK(setCursor), nullptr);
            g_signal_connect(button, "clicked", G_CALLBACK(onClick), nullptr);
        }
};