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
int counterA, counterB;

int main(void)
{
  // Outer loop is for each game.
  for (counterB = 1; counterB <= GAMES; counterB++)
  {
    printf("\nGetting scoring totals for Game #%d.\n", counterB);

    // Inner loop is for each player in the specific game.
    for (counterA = 1; counterA <= PLAYERS; counterA++)
    {
      printf("What did player #%d score in the game\? ", counterA);

      scanf("%d", &scores[counterA][counterB]);
    }
  }

  // Loop through the array to calculate scoring average per player.
  for (counterA = 1; counterA <= PLAYERS; counterA++)
  {
    point_total = 0;

    for (counterB = 1; counterB <= GAMES; counterB++)
    {
      point_total += scores[counterA][counterB];
    }

    score_average[counterA] = (float)point_total / GAMES;
  }

  best_average = 0;

  for (counterA = 1; counterA <= PLAYERS; counterA++)
  {
    if (score_average[counterA] > best_average)
    {
      best_average = score_average[counterA];
      best_player = counterA;
    }
  }

  printf("\nPlayer #%d had the best scoring average,\n", best_player);
  printf("at %.2f points per game.\n", score_average[best_player]);

  return 0;
}
