#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"
#include "gtk/gtk.h"
#include <cstddef>
#include <gtk-layer-shell/gtk-layer-shell.h>

class Bar {
    static const int BAR_HEIGHT = 40;

    Bar(GtkWidget* window) {
        this->window = window;
        gtk_layer_init_for_window(GTK_WINDOW(this->window));

        gtk_window_set_title(GTK_WINDOW(this->window), "Archibar");

        gtk_layer_set_exclusive_zone(GTK_WINDOW(this->window), BAR_HEIGHT);

        gtk_layer_set_anchor(GTK_WINDOW(this->window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
        gtk_layer_set_anchor(GTK_WINDOW(this->window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
        gtk_layer_set_anchor(GTK_WINDOW(this->window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);

        gtk_widget_set_size_request(GTK_WIDGET(this->window), 0, BAR_HEIGHT);
        gtk_window_resize(GTK_WINDOW(this->window), 1, 1);

    }

    // static gboolean updateCountdown(gpointer labelData) {
    //     GtkWidget* label = GTK_WIDGET(labelData);
    //     static int remaining = 5;

    //     if (remaining > 0) {
    //         char textLabel[15];
    //         g_snprintf(textLabel, sizeof(textLabel), "Closing in %ds", remaining);
    //         gtk_label_set_text(GTK_LABEL(label), textLabel);
    //         remaining--;

    //         return G_SOURCE_CONTINUE;
    //     }

    //     gtk_widget_destroy(gtk_widget_get_toplevel(label));
    //     return G_SOURCE_REMOVE;
    // }

    static void updateLabel(Bar* bar) {
        // GtkWidget* label = gtk_label_new("Closing in 5s");
        // gtk_container_add(GTK_CONTAINER(bar->window), label);

        // g_timeout_add(1000, updateCountdown, label);
    }

    static void attachActions(Bar* bar) {
        updateLabel(bar);

        g_signal_connect(bar->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    }

    public:
        GtkWidget* window;

        void start() {
            gtk_widget_show_all(window);
            gtk_main();
        }

        void shutdown() {
            gtk_widget_destroy(this->window);
        }

        static Bar* copy(Bar* oldBar) {
            Bar* newBar = new Bar(oldBar->window);
            attachActions(newBar);

            return newBar;
        }

        static Bar* Create() {
            Bar* newBar = new Bar(gtk_window_new(GTK_WINDOW_TOPLEVEL));
            attachActions(newBar);

            return newBar;
        }
};