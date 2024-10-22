/* Shows how to use 2 dimensional arrays */
#include <stdio.h>

#define PLAYERS 5
#define GAMES 4

// The [0] elements will be ignored for simplicity.
// {
// {},
// {{}, 10, 20, 15, 25},  // Player 1
// {{}, 30, 35, 40, 45},  // Player 2
// {{}, 22, 26, 28, 30},  // Player 3
// {{}, 18, 25, 27, 20},  // Player 4
// {{}, 25, 20, 35, 40}   // Player 5
// }
int scores[PLAYERS + 1][GAMES + 1];
float score_average[PLAYERS + 1], best_average;
int point_total, best_player;
int ctrPlayers, ctrGames;

int main(void)
{
  // Outer loop is for each game.
  for (ctrGames = 1; ctrGames <= GAMES; ctrGames++)
  {
    printf("\nGetting scoring totals for Game #%d.\n", ctrGames);

    // Inner loop is for each player in the specific game.
    for (ctrPlayers = 1; ctrPlayers <= PLAYERS; ctrPlayers++)
    {
      printf("What did player #%d score in the game\? ", ctrPlayers);

      scanf("%d", &scores[ctrPlayers][ctrGames]);
    }
  }

  // Loop through the array to calculate scoring average per player.
  for (ctrPlayers = 1; ctrPlayers <= PLAYERS; ctrPlayers++)
  {
    point_total = 0;

    for (ctrGames = 1; ctrGames <= GAMES; ctrGames++)
    {
      point_total += scores[ctrPlayers][ctrGames];
    }

    score_average[ctrPlayers] = (float)point_total / GAMES;
  }

  best_average = 0;

  for (ctrPlayers = 1; ctrPlayers <= PLAYERS; ctrPlayers++)
  {
    if (score_average[ctrPlayers] > best_average)
    {
      best_average = score_average[ctrPlayers];
      best_player = ctrPlayers;
    }
  }

  printf("\nPlayer #%d had the best scoring average,\n", best_player);
  printf("at %.2f points per game.\n", score_average[best_player]);

  return 0;
}
