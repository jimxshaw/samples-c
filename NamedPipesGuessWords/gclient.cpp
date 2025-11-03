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
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "LineInfo.h"

using namespace std;

const int BUFFER_SIZE = 100;
const int MAX_TRIES = 12;
const string SERVER_REQUEST_PIPE = "request.pipe";		 // Known pipe.
const string CLIENT_PIPE_PREFIX = "client_read_pipe_"; // Will be used to name our read pipe.

string clientWritePipe; // Server-created pipe for reading client guesses.
string clientReadPipe;
string wordToGuess; // The word we're trying to guess.

int currentTry = 0;
int clientReadFD = -1;								// Read game data from the server.
const mode_t PIPE_PERMISSIONS = 0600; // Read-Write for owners.

// Generate a unique client read pipe name.
string createClientReadPipeName()
{
	// Construct the pipe name: e.g., "client_read_pipe_12345"
	string uniquePipeName = CLIENT_PIPE_PREFIX + to_string(getpid());

	return uniquePipeName;
}

// Create the client's read pipe and open it.
void createAndOpenClientReadPipe(const string &pipeName)
{
	// Create the named pipe (FIFO) with read/write permissions.
	if (mkfifo(pipeName.c_str(), PIPE_PERMISSIONS) == -1)
	{
		if (errno != EEXIST)
		{
			cerr << "Error: Failed to create named pipe '" << pipeName << "': "
					 << strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		// If pipe already exists, that's okay, continue.
	}

	// Open the named pipe for reading.
	clientReadFD = open(pipeName.c_str(), O_RDONLY);
	if (clientReadFD == -1)
	{
		cerr << "Error: Failed to open pipe '" << pipeName << "' for reading: "
				 << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Client: Successfully created and opened read pipe '" << pipeName << "'" << endl;
}

// Send pipe name to server via known pipe.
void sendPipeNameToServer(const string &pipeName)
{
	// Open the well-known server pipe for writing, it's write-only.
	int serverWriteFD = open(SERVER_REQUEST_PIPE.c_str(), O_WRONLY);

	if (serverWriteFD == -1)
	{
		throw domain_error(LineInfo("Failed to open server request pipe", __FILE__, __LINE__));
		exit(EXIT_FAILURE);
	}

	// Write the client's pipe name to the server.
	ssize_t bytesWritten = write(serverWriteFD, pipeName.c_str(), pipeName.length());

	if (bytesWritten == -1)
	{
		throw domain_error(LineInfo("Failed to write pipe name to server", __FILE__, __LINE__));
		close(serverWriteFD);
		exit(EXIT_FAILURE);
	}

	close(serverWriteFD);
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
	catch (exception &e)
	{
		cout << e.what() << endl;
		cout << "\nPress Enter once or twice to exit..." << endl;
		cin.ignore();
		cin.get();
		exit(EXIT_FAILURE);
	}

	return 0;
}