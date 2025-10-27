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

const string REQUEST_PIPE = "request.pipe"; // Well-known pipe clients write to.
const int MAX_TRIES = 12;                   // Max number of incorrect guesses.
const int BUFFER_SIZE = 100;                // For pipe I/O buffer.

vector<string> wordList;   // All words from words.txt.
string clientPipeName;     // Received from client.
string serverToClientPipe; // Named pipe used to send initial game data.
string clientToServerPipe; // Pipe used by child process to read guesses.


