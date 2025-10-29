#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "LineInfo.h"

using namespace std;

const int BUFFER_SIZE = 128;
const int MAX_TRIES = 12;
const string SERVER_REQUEST_PIPE = "request.pipe"; // Known pipe.

string clientReadPipe;  // Client-created pipe for reading server responses.
string clientWritePipe; // Server-created pipe for reading client guesses.

int currentTry = 0;

// Generate a unique client read pipe name.
string createClientReadPipeName()
{
}

// Create the client's read pipe and open it.
void createAndOpenClientReadPipe(const string &pipeName)
{
}

// Send pipe name to server via known pipe.
void sendPipeNameToServer(const string &pipeName)
{
}

// Open the server-provided pipe to write guesses.
void openClientWritePipe(string &pipeName)
{
}

// Read initial game setup from server (tries, word, server pipe name).
void readGameInitFromServer(int &tryCount, string &wordToGuess, string &serverReadPipe)
{
}

// Game loop: display, get user input, send guess, update try count.
void playGameLoop(int tryCount, const string &randomWord, const string &serverReadPipe)
{
}

// Clean up all pipes.
void cleanUp()
{
}

int main()
{
  try
  {
    clientReadPipe = createClientReadPipeName();
    createAndOpenClientReadPipe(clientReadPipe);

    sendPipeNameToServer(clientReadPipe);

    string randomWord;

    readGameInitFromServer(currentTry, randomWord, clientWritePipe);

    openClientWritePipe(clientWritePipe);

    cout << "Game Start\n\nYou have " << MAX_TRIES << " letter guesses to win\n\n";

    playGameLoop(currentTry, randomWord, clientWritePipe);

    cleanUp();
  }
  catch (string err)
  {
    cerr << err << endl;
  }

  return 0;
}