#include "gtk/gtk.h"
class AbstractModule {
    protected:
        GtkWidget* container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    public:
        GtkWidget* getContainer() {
            return this->container;
        }
        
        virtual ~AbstractModule() = default;
};