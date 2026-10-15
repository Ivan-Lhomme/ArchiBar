#include "glib-object.h"
#include "glibconfig.h"
#include "gtk/gtk.h"
#include "gtk/gtkcssprovider.h"
#include <cstddef>
#include <gtk-layer-shell/gtk-layer-shell.h>

#include "headers/abstractHeaders.hpp"
#include "headers/factoryHeaders.hpp"

class Bar {
    static const int BAR_HEIGHT = 40;
    GtkWidget* window;
    GtkWidget* mainContainer;

    Bar(GtkWidget* window, GtkWidget* mainContainer) {
        this->window = window;
        this->mainContainer = mainContainer;

        gtk_layer_init_for_window(GTK_WINDOW(this->window));

        gtk_window_set_title(GTK_WINDOW(this->window), "Archibar");

        gtk_layer_set_exclusive_zone(GTK_WINDOW(this->window), BAR_HEIGHT);

        gtk_layer_set_anchor(GTK_WINDOW(this->window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
        gtk_layer_set_anchor(GTK_WINDOW(this->window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
        gtk_layer_set_anchor(GTK_WINDOW(this->window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);

        gtk_widget_set_size_request(GTK_WIDGET(this->window), 0, BAR_HEIGHT);
        gtk_window_resize(GTK_WINDOW(this->window), 1, 1);

        GtkCssProvider* cssProvider = gtk_css_provider_new();

        gtk_css_provider_load_from_path(cssProvider, "style.css", nullptr);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        gtk_container_add(GTK_CONTAINER(this->window), GTK_WIDGET(this->mainContainer));

        g_object_ref(cssProvider);
    }

    void attachActions() {
        g_signal_connect(this->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    }

    void attachModule(AbstractModule* module) {
        gtk_box_pack_start(GTK_BOX(this->mainContainer), module->getContainer(), FALSE, FALSE, 0);
    }

    void addModules() {
        AbstractModule* powerButton = FactoryModule::createPowerButton();
        this->attachModule(powerButton);
    }

    public:
        GtkWidget* getWindow() {
            return this->window;
        }

        void start() {
            gtk_widget_show_all(window);
            gtk_main();
        }

        void shutdown() {
            gtk_widget_destroy(this->window);
        }



        static Bar* copy(Bar* oldBar) {
            Bar* newBar = new Bar(oldBar->window, oldBar->mainContainer);
            
            newBar->attachActions();
            newBar->addModules();

            return newBar;
        }

        static Bar* Create() {
            Bar* newBar = new Bar(gtk_window_new(GTK_WINDOW_TOPLEVEL), gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));

            newBar->attachActions();
            newBar->addModules();

            return newBar;
        }
};