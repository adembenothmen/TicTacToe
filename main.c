#include <gtk/gtk.h>
#include "game.h"



// --------------- role:lancement GTK -------------------------

int main(int argc, char *argv[])
{
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Création de la fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tic Tac Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Quitter proprement l'application
    g_signal_connect(
        window,
        "destroy",
        G_CALLBACK(gtk_main_quit),
        NULL
    );

    // Création du plateau
    create_board(window);

    // Lancer la boucle GTK
    gtk_main();

    return 0;
}
