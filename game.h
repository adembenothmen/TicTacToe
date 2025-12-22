#ifndef GAME_H
#define GAME_H

#include <gtk/gtk.h>

/* =========================
   CONSTANTES DU JEU
   ========================= */

// Joueurs (important pour l'IA plus tard)
#define HUMAN 'X'
#define AI    'O'
#define EMPTY ' '

/* =========================
   DONNÉES GLOBALES
   ========================= */

// Plateau logique 3x3
extern char board[3][3];

// Boutons GTK (interface)
extern GtkWidget *buttons[3][3];

// Joueur courant
extern char current_player;

// Indique si la partie est terminée
extern gboolean game_over;

/* =========================
   INTERFACE GRAPHIQUE
   ========================= */

// Crée la grille GTK
void create_board(GtkWidget *window);

// Fonction appelée lors d'un clic
void on_button_clicked(GtkWidget *button, gpointer data);

// Message de fin
void show_game_over(GtkWidget *parent, const char *message);

/* =========================
   LOGIQUE DU JEU (SANS GTK)
   ========================= */

// Vérifie s'il y a un gagnant
char check_winner(void);

// Vérifie si le plateau est plein
gboolean is_board_full(void);

// Joue un coup (ligne, colonne, joueur)
gboolean play_move(int row, int col, char player);

// Annule un coup (pour Minimax plus tard)
void undo_move(int row, int col);

#endif
