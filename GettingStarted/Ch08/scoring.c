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
int scores[6][5];
float score_average[6], best_average;
int point_total, best_player;
int counter1, counter2;

int main(void)
{
  // Outer loop is for each game.
  for (counter2 = 1; counter2 <= GAMES; counter2++)
  {
    printf("\nGetting scoring totals for Game #%d.\n", counter2);

    // Inner loop is for each player in the specific game.
    for (counter1 = 1; counter1 <= PLAYERS; counter1++)
    {
      printf("What did player #%d score in the game\? ", counter1);

      scanf("%d", &scores[counter1][counter2]);
    }
  }

  // Loop through the array to calculate scoring average per player.
  for (counter1 = 1; counter1 <= PLAYERS; counter1++)
  {
    point_total = 0;

    for (counter2 = 1; counter2 <= GAMES; counter2++)
    {
      point_total += scores[counter1][counter2];
    }

    score_average[counter1] = (float)point_total / GAMES;
  }

  best_average = 0;

  for (counter1 = 1; counter1 <= PLAYERS; counter1++)
  {
    if (score_average[counter1] > best_average)
    {
      best_average = score_average[counter1];
      best_player = counter1;
    }
  }

  printf("\nPlayer #%d had the best scoring average,\n", best_player);
  printf("at %.2f points per game.\n", score_average[best_player]);

  return 0;
}
