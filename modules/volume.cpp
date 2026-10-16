#include "glib.h"
#include "gtk/gtk.h"
#include <cstdio>
#include <string>

#include "../headers/abstractHeaders.hpp"

class Volume : public AbstractModule {
    static std::string getVolumePercentage() {
        char buffer[256];
        std::string volumePercentage;
        FILE* resultCmd = popen("amixer get Master | grep -m 1 \'%\' | awk \'gsub(/[\\[\\]%]/, \"\", $5) {print $5}\'", "r");
        while (fgets(buffer, sizeof(buffer), resultCmd)) {
            volumePercentage += buffer;
        }

        if (volumePercentage.back() == '\n') volumePercentage.pop_back();
        pclose(resultCmd);
        return volumePercentage;
    }

    static bool isMuted() {
        char buffer[256];
        std::string volumeActif;
        FILE* resultCmd = popen("amixer get Master | grep -m 1 \'%\' | awk \'gsub(/[\\[\\]]/, \"\", $6) {print $6}\'", "r");
        while (fgets(buffer, sizeof(buffer), resultCmd)) {
            volumeActif += buffer;
        }

        pclose(resultCmd);
        return volumeActif == "off\n";
    }

    static std::string getVolumeIconName(int volume) {
        if (isMuted() || volume == 0) {
            return "audio-volume-muted-symbolic";
        }

        return
            volume <= 33 ? "audio-volume-low-symbolic" :
            volume <= 66 ? "audio-volume-medium-symbolic" :
            "audio-volume-high-symbolic";
    }

    static gboolean updateVolume(gpointer data) {
        Volume* volumeModule = static_cast<Volume*>(data);
        std::string volume = getVolumePercentage();

        gtk_image_set_from_icon_name(
            GTK_IMAGE(volumeModule->volumeIcon),
            getVolumeIconName(std::stoi(volume)).c_str(),
            GTK_ICON_SIZE_MENU
        );

        std::string volumeText = volume + "%";

        gtk_label_set_text(GTK_LABEL(volumeModule->label), volumeText.c_str());

        return G_SOURCE_CONTINUE;
    }

    public:
        GtkWidget* label;
        GtkWidget* volumeIcon;
        
        Volume() {
            std::string volume = getVolumePercentage();

            this->volumeIcon = gtk_image_new_from_icon_name(
                getVolumeIconName(std::stoi(volume)).c_str(),
                GTK_ICON_SIZE_MENU
            );

            std::string volumeText = volume + "%";
            this->label = gtk_label_new(volumeText.c_str());

            gtk_container_add(GTK_CONTAINER(container), label);
            gtk_container_add(GTK_CONTAINER(container), volumeIcon);
            gtk_widget_set_name(container, "volume-module");

            g_timeout_add(100, updateVolume, this);
        }
};