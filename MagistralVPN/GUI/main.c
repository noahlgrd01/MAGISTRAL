#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define TAILLE_ON_OFF 70
#define LARGEUR_FENETRE 400
#define HAUTEUR_FENETRE 800

static void printTESTZIZI(){
    printf("Zizi");
}

// Ensemble de structure et de fonction pour lire le fichier de paramètre
typedef struct {
    char **keys;
    char **values;
    int count;
} ConfigData;

ConfigData lireFichier(const char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "r");
    if (!f) {
        perror("Erreur ouverture fichier");
        exit(EXIT_FAILURE);
    }

    ConfigData data = {NULL, NULL, 0};
    char ligne[MAX_LINE];

    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0;

        char *egal = strchr(ligne, '=');
        if (egal) {
            *egal = '\0';
            char *cle = ligne;
            char *valeur = egal + 1;

            if (*cle && *valeur) {
                data.keys = realloc(data.keys, (data.count + 1) * sizeof(char *));
                data.values = realloc(data.values, (data.count + 1) * sizeof(char *));
                
                data.keys[data.count] = strdup(cle);
                data.values[data.count] = strdup(valeur);
                data.count++;
            }
        }
    }

    fclose(f);
    return data;
}

void libererMemoireConfigData(ConfigData *data) {
    for (int i = 0; i < data->count; i++) {
        free(data->keys[i]);
        free(data->values[i]);
    }
    free(data->keys);
    free(data->values);
    data->count = 0;
}

// Fonction appelée lors du clic sur le bouton (activer-désactiver VPN)
static void buttonONOFF(GtkWidget *button, gpointer label) {
    gtk_label_set_text(GTK_LABEL(label), "Bouton cliqué !");
}

static void loadMagistralVPNGUI(GtkApplication *app, gpointer profildefaut) {
        // Cast des paramètres de l'utilisateur
    char *profil_defaut = (char *)profildefaut;
    g_print("Profil par défaut : %s\n", profil_defaut);
        // Création de la fenêtre
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MagistralVPN GUI");
    gtk_window_set_default_size(GTK_WINDOW(window), LARGEUR_FENETRE, HAUTEUR_FENETRE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

        // Création d'une boîte verticale
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // --------------- Barre de menu -------------------------
    GtkWidget *menubar = gtk_menu_bar_new();

        // Menu "Préférences"
    GtkWidget *preferences_menu = gtk_menu_new();
    GtkWidget *preferences_item = gtk_menu_item_new_with_label("Préférences");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(preferences_item), preferences_menu);

            // Création du sous-menu "Taille fenêtre"
    GtkWidget *window_size_menu = gtk_menu_new();
    GtkWidget *window_size_item = gtk_menu_item_new_with_label("Taille fenêtre");

    g_signal_connect(window_size_item, "activate", G_CALLBACK(printTESTZIZI), window);

            // Ajouter l'élément "Taille fenêtre" au menu "Préférences"
    gtk_menu_shell_append(GTK_MENU_SHELL(preferences_menu), window_size_item);

            // Ajouter l'élément "Préférences" à la barre de menu
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), preferences_item);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
    // -------------------------------------------------------

        // Création du label
    GtkWidget *label = gtk_label_new("Cliquez sur l'image !");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

        // Ajouter un espace flexible pour pousser le bouton en bas
    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_box_pack_start(GTK_BOX(vbox), spacer, TRUE, TRUE, 0);

    // -------------------- BOUTON ON-OFF -------------------- 
        // Charger image
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size("image/on-off.png", TAILLE_ON_OFF, TAILLE_ON_OFF, NULL);
    GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);

        // Créer bouton et insérer image à l'intérieur
    GtkWidget *button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button), image);
    gtk_widget_set_size_request(button, TAILLE_ON_OFF, TAILLE_ON_OFF);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    
        // Ajout d'un espace en bas
    gtk_widget_set_margin_bottom(button, 10);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    // -------------------------------------------------------

        // Connexion du signal
    g_signal_connect(button, "clicked", G_CALLBACK(buttonONOFF), label);

        // Afficher tous les widgets
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    // Chargement des paramètres
    ConfigData config = lireFichier("/etc/MagistralVPN/vpn.conf");
    char *profil_defaut = config.values[0];
    libererMemoireConfigData(&config);

    // Chargement de l'application
    GtkApplication *app = gtk_application_new("com.example.GTKApp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(loadMagistralVPNGUI), (gpointer)profil_defaut);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}