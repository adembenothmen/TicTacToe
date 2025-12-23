#ifndef GAME_H
#define GAME_H

#include <gtk/gtk.h>

/*
 game.h :
 Ce fichier définit l’interface du jeu.
 Il permet à main.c de communiquer avec game.c
 sans connaître les détails de l’implémentation.

 game.h est le contrat du jeu :il dit ce que le jeu sait faire, sans expliquer comment.
 main.c → utilise les fonctions
 game.c → les implémente
*/


/* =========================
   CONSTANTES DU JEU
   ========================= */

// Joueurs
#define HUMAN 'X'   // Joueur humain
#define AI    'O'   // Ordinateur (IA)
#define EMPTY ' '   // Case vide

/* =========================
   DONNÉES GLOBALES
   ========================= */

// Plateau logique 3x3 (état du jeu)
extern char board[3][3];

// Boutons GTK associés au plateau
extern GtkWidget *buttons[3][3];

// Joueur courant (HUMAN ou AI)
extern char current_player;

// Indique si la partie est terminée
extern gboolean game_over;

/* =========================
   INTERFACE GRAPHIQUE (GTK)
   ========================= */

// Crée la grille du jeu (boutons)
void create_board(GtkWidget *window);

// Fonction appelée lors d’un clic sur un bouton
void on_button_clicked(GtkWidget *button, gpointer data);

// Affiche un message de fin de partie
void show_game_over(GtkWidget *parent, const char *message);

/* =========================
   LOGIQUE DU JEU (SANS GTK)
   ========================= */

// Vérifie s’il y a un gagnant
char check_winner(void);

// Vérifie si le plateau est plein
gboolean is_board_full(void);

// Coup du PC (utilise min max)
void pc_play(GtkWidget *window);


// Joue un coup (ligne, colonne, joueur)
gboolean play_move(int row, int col, char player);

// Annule un coup (utile pour Minimax plus tard)
void undo_move(int row, int col);

#endif
