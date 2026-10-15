#include <cstdlib>
#include <gtk-layer-shell/gtk-layer-shell.h>
#include "gtk/gtk.h"

#include "headers/barHeaders.hpp"

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    Bar* bar = Bar::Create();

    bar->start();

    free(bar);
    return 0;
}