#include "game.h"

/* =========================
   DONNÉES GLOBALES
   ========================= */

// Plateau logique
char board[3][3] = {
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY}
};

// Boutons GTK
GtkWidget *buttons[3][3];

// Joueur courant (humain commence)
char current_player = HUMAN;

// État de la partie
gboolean game_over = FALSE;

/* =========================
   CRÉATION DE LA GRILLE
   ========================= */
void create_board(GtkWidget *window)
{
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            GtkWidget *button = gtk_button_new_with_label(" ");
            gtk_widget_set_size_request(button, 100, 100);

            buttons[i][j] = button;

            // Sauvegarder la position du bouton
            int *pos = g_malloc(sizeof(int) * 2);
            pos[0] = i;
            pos[1] = j;

            g_signal_connect(
                button,
                "clicked",
                G_CALLBACK(on_button_clicked),
                pos
            );

            gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
        }
    }

    gtk_widget_show_all(window);
}

/* =========================
   GESTION DU CLIC
   ========================= */
void on_button_clicked(GtkWidget *button, gpointer data)
{
    if (game_over)
        return;

    int *pos = (int *)data;
    int row = pos[0];
    int col = pos[1];

    // Jouer le coup
    if (!play_move(row, col, current_player))
        return;

    gtk_button_set_label(
        GTK_BUTTON(button),
        (current_player == HUMAN) ? "X" : "O"
    );

    // Vérifier victoire
    char winner = check_winner();
    if (winner != EMPTY) {
        char msg[50];
        sprintf(msg, "Le joueur %c a gagné !", winner);
        game_over = TRUE;
        show_game_over(gtk_widget_get_toplevel(button), msg);
        return;
    }

    // Vérifier égalité
    if (is_board_full()) {
        game_over = TRUE;
        show_game_over(
            gtk_widget_get_toplevel(button),
            "Match nul !"
        );
        return;
    }

    // Changer de joueur
    current_player = (current_player == HUMAN) ? AI : HUMAN;
}

/* =========================
   LOGIQUE DU JEU (PURE)
   ========================= */
gboolean play_move(int row, int col, char player)
{
    if (board[row][col] != EMPTY)
        return FALSE;

    board[row][col] = player;
    return TRUE;
}

void undo_move(int row, int col)
{
    board[row][col] = EMPTY;
}

/* =========================
   VÉRIFICATION GAGNANT
   ========================= */
char check_winner(void)
{
    for (int i = 0; i < 3; i++) {
        // Lignes
        if (board[i][0] != EMPTY &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];

        // Colonnes
        if (board[0][i] != EMPTY &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }

    // Diagonales
    if (board[0][0] != EMPTY &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];

    if (board[0][2] != EMPTY &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];

    return EMPTY;
}

/* =========================
   PLATEAU PLEIN ?
   ========================= */
gboolean is_board_full(void)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == EMPTY)
                return FALSE;

    return TRUE;
}

/* =========================
   MESSAGE FIN DE JEU
   ========================= */
void show_game_over(GtkWidget *parent, const char *message)
{
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", message
    );

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
