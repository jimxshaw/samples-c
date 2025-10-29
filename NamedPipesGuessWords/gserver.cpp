/*
Program Name: Named Pipes Guess Words
Date: 10/27/2025
Author: James Shaw
Purpose: This is a client-server word guessing game using named pipes,
        forked processes, separate executables and a shared word list
        list to provide randomized guess words.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>     // For rand(), srand()
#include <ctime>       // For time()
#include <unistd.h>    // For fork(), getpid()
#include <fcntl.h>     // For open()
#include <sys/stat.h>  // For mkfifo()
#include <sys/types.h> // For pid_t
#include <sys/wait.h>  // For waitpid()

#include "LineInfo.h" // Provided error handling

using namespace std;

static const char *const WORDS_FILE_NAME = "words.txt";

const string SERVER_REQUEST_PIPE = "request.pipe"; // Well-known pipe clients write to.
const int MAX_TRIES = 12;                          // Max number of incorrect guesses.
const int BUFFER_SIZE = 128;                       // For pipe I/O buffer.

vector<string> wordList;   // All words from words.txt.
string clientPipeName;     // Received from client.
string serverToClientPipe; // Named pipe used to send initial game data.
string clientToServerPipe; // Pipe used by child process to read guesses.

// Reads words from a given file and populates the global wordList vector.
void loadWordsFromFile(const string &fileName)
{
}

// Creates the request pipe.
void createRequestPipe()
{
}

// Reads the client's private pipe name from the well-known pipe.
void receiveClientPipeName()
{
}

// Sends the initial game setup to the client:
// - the word to guess
// - the blanked-out guess word
// - try count (0)
// - the name of the server’s pipe (to receive future guesses)
void sendInitialGameData(const string &chosenWord)
{
}

// Handles the word-guessing game in a child process.
// Listens for letter guesses and updates the game state.
void runGameInChildProcess(const string &word, const string &clientGuessPipe)
{
}

// Randomly selects a word from wordList.
string chooseRandomWord()
{
}

// Creates a blanked-out version of a word, such as "-----".
string createHiddenWord(const string &word)
{
}

int main()
{
        loadWordsFromFile(WORDS_FILE_NAME);

        createRequestPipe();

        receiveClientPipeName();

        string word = chooseRandomWord();

        sendInitialGameData(word);

        // Ford child to handle game loop.
        pid_t pid = fork();

        if (pid == 0)
        {
                // Child process.
                runGameInChildProcess(word, clientToServerPipe);
        }
        else
        {
                // Parent process.
                waitpid(pid, nullptr, 0);
        }

        return 0;
}