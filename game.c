// ---------------- role : logique de jeux------------

#include "game.h"

// =========================
// DONNÉES GLOBALES
// =========================

// Plateau logique 3x3 initialisé vide
char board[3][3] = {
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY}
};

// Tableau de boutons GTK correspondant aux cases du plateau
GtkWidget *buttons[3][3];

// Joueur courant (HUMAN commence)
char current_player = HUMAN;

// Indique si la partie est terminée
gboolean game_over = FALSE;

// ==========================================
// FONCTIONS UTILITAIRES POUR MINIMAX
// ===========================================

// Retourne le maximum entre deux entiers
static int max(int a, int b) { return (a > b) ? a : b; }

// Retourne le minimum entre deux entiers
static int min(int a, int b) { return (a < b) ? a : b; }

// Évalue le plateau et retourne un score selon l'état
// +10 si l'IA gagne, -10 si l'humain gagne, 0 sinon
static int evaluate_board(void) {
    char winner = check_winner();
    if (winner == AI) return 10;
    if (winner == HUMAN) return -10;
    return 0;
}

// Vérifie s'il reste des cases vides
static gboolean moves_left(void) {
    return !is_board_full();
}

// Fonction Minimax récursive
// depth : profondeur de l'arbre (pas utilisée pour l'instant, mais utile pour extensions)
// is_ai_turn : TRUE si c'est le tour de l'IA, FALSE si c'est le tour de l'humain
static int minimax(int depth, gboolean is_ai_turn) {
    int score = evaluate_board(); // Évalue le plateau actuel

    // Si quelqu'un a gagné, retourne le score
    if (score == 10 || score == -10) return score;

    // Si aucun coup possible, plateau plein => match nul
    if (!moves_left()) return 0;

    if (is_ai_turn) {
        int best = -1000; // Initialisation du meilleur score pour l'IA
        // Parcourt toutes les cases
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == EMPTY) { // Si case vide
                    board[i][j] = AI; // Simule le coup de l'IA
                    best = max(best, minimax(depth + 1, FALSE)); // Appel récursif pour l'humain
                    board[i][j] = EMPTY; // Annule le coup (backtracking)
                }
        return best; // Retourne le meilleur score pour l'IA

    } else {//human turn
        int best = 1000; // Initialisation du meilleur score pour l'humain
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == EMPTY) {
                    board[i][j] = HUMAN; // Simule le coup de l'humain
                    best = min(best, minimax(depth + 1, TRUE)); // Appel récursif pour l'IA
                    board[i][j] = EMPTY; // Annule le coup
                }
        return best; // Retourne le meilleur score pour l'humain
    }
}

// =================================================================
// CREATION DE LA GRILLE GTK
// =================================================================
void create_board(GtkWidget *window)
{
    GtkWidget *grid = gtk_grid_new(); // Création d'une grille GTK
    gtk_container_add(GTK_CONTAINER(window), grid); // Ajout de la grille à la fenêtre

    for (int i = 0; i < 3; i++) {       // Parcourt les lignes
        for (int j = 0; j < 3; j++) {   // Parcourt les colonnes

            buttons[i][j] = gtk_button_new_with_label(" "); // Création d'un bouton vide
            gtk_widget_set_size_request(buttons[i][j], 100, 100); // Taille du bouton

            int index = i * 3 + j; // Calcul d'un index unique pour passer au callback

            // Connexion du signal clic au bouton
            g_signal_connect(
                buttons[i][j],
                "clicked",
                G_CALLBACK(on_button_clicked),
                GINT_TO_POINTER(index) // Passe l'index comme argument
            );

            gtk_grid_attach(GTK_GRID(grid), buttons[i][j], j, i, 1, 1); // Ajout du bouton à la grille
        }
    }

    gtk_widget_show_all(window); // Affiche tous les widgets
}

// =================================================================
// GESTION DU CLIC SUR UNE CASE
// =================================================================
void on_button_clicked(GtkWidget *button, gpointer data)
{
    if (game_over || current_player != HUMAN) // Ignorer si partie terminée ou pas le tour de l'humain
        return;

    int index = GPOINTER_TO_INT(data); // Récupère l'index de la case
    int row = index / 3; // Calcul de la ligne
    int col = index % 3; // Calcul de la colonne

    if (!play_move(row, col, HUMAN)) // Si la case est occupée, ne rien faire
        return;

    gtk_button_set_label(GTK_BUTTON(button), "X"); // Mettre "X" sur le bouton

    char winner = check_winner(); // Vérifie si quelqu'un a gagné
    if (winner == HUMAN) {
        show_game_over(gtk_widget_get_toplevel(button), "You win!");
        game_over = TRUE;
        return;
    }

    if (is_board_full()) { // Si plateau plein, match nul
        show_game_over(gtk_widget_get_toplevel(button), "Draw!");
        game_over = TRUE;
        return;
    }

    current_player = AI; // Passer au tour de l'IA
    pc_play(gtk_widget_get_toplevel(button)); // Lancer le coup de l'IA
}

// ===========================================================
// LOGIQUE DU JEU (SANS GTK)
// ===========================================================
gboolean play_move(int row, int col, char player)
{
    if (board[row][col] != EMPTY) // Vérifie que la case est vide
        return FALSE;

    board[row][col] = player; // Place le symbole du joueur
    return TRUE;
}

// Annule un coup (utile pour Minimax)
void undo_move(int row, int col)
{
    board[row][col] = EMPTY;
}

// ==================================================================
// IA PC AVEC MINIMAX
// ===================================================================
void pc_play(GtkWidget *window)
{
    if (game_over) return; // Ne rien faire si la partie est terminée

    int best_val = -1000;  // Meilleur score trouvé
    int best_row = -1, best_col = -1; // Coordonnées du meilleur coup

    // Parcourt toutes les cases
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) { // Case vide ?
                board[i][j] = AI; // Simule le coup
                int move_val = minimax(0, FALSE); // Évalue le coup avec Minimax
                board[i][j] = EMPTY; // Annule le coup

                // Si ce coup est meilleur que le précédent
                if (move_val > best_val) {
                    best_val = move_val;
                    best_row = i;
                    best_col = j;
                }
            }
        }
    }

    // Joue le meilleur coup trouvé
    if (best_row != -1 && best_col != -1) {
        play_move(best_row, best_col, AI);
        gtk_button_set_label(GTK_BUTTON(buttons[best_row][best_col]), "O");

        char winner = check_winner();
        if (winner == AI) { // L'IA gagne ?
            show_game_over(window, "PC wins!");
            game_over = TRUE;
            return;
        }

        if (is_board_full()) { // Match nul ?
            show_game_over(window, "Draw!");
            game_over = TRUE;
            return;
        }

        current_player = HUMAN; // Retour au joueur humain
    }
}

// =============================================================================
// VERIFICATION DU GAGNANT
// ============================================================================
char check_winner(void)
{
    for (int i = 0; i < 3; i++) {
        // Vérifie lignes
        if (board[i][0] != EMPTY &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];

        // Vérifie colonnes
        if (board[0][i] != EMPTY &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }

    // Vérifie diagonales
    if (board[0][0] != EMPTY &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];

    if (board[0][2] != EMPTY &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];

    return EMPTY; // Pas de gagnant
}

// =========================
// PLATEAU PLEIN ?
gboolean is_board_full(void)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == EMPTY) // Case vide trouvée ?
                return FALSE;

    return TRUE; // Plateau plein
}

// =========================
// MESSAGE FIN DE JEU
void show_game_over(GtkWidget *parent, const char *message)
{
    // Crée une boîte de dialogue GTK pour afficher le message
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", message
    );

    gtk_dialog_run(GTK_DIALOG(dialog)); // Affiche la boîte de dialogue
    gtk_widget_destroy(dialog);         // Détruit la boîte de dialogue après fermeture
}
