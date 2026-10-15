#include "../headers/abstractHeaders.hpp"
#include "gtk/gtk.h"

class LabelTest : public AbstractModule {
    public:
        GtkWidget* label = gtk_label_new("Test Label.");
        
        LabelTest() {
            gtk_container_add(GTK_CONTAINER(container), label);
        }
};