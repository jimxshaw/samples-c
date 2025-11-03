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
int clientWriteFD = -1;								// Writing guesses to server.
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
	// Open the pipe for write-only access.
	if (open(pipeName.c_str(), O_WRONLY) == -1)
	{
		throw domain_error(LineInfo("Failed to open server pipe for writing guesses.", __FILE__, __LINE__));
	}
}

// Read initial game setup from server (tries, word, server pipe name).
void readGameInitFromServer(int &tryCount, string &wordToGuess, string &serverReadPipe)
{
	char buffer[BUFFER_SIZE] = {0};

	// Read data from the server.
	ssize_t bytesRead = read(clientReadFD, buffer, BUFFER_SIZE - 1); // leave room for null terminator
	if (bytesRead <= 0)
	{
		throw runtime_error(LineInfo("Client failed to read initial game data from server", __FILE__, __LINE__));
	}

	buffer[bytesRead] = '\0'; // Null-terminate just in case.

	// Split the input by lines.
	vector<string> lines;
	string currentLine;
	for (int i = 0; i < bytesRead; ++i)
	{
		if (buffer[i] == '\n')
		{
			lines.push_back(currentLine);
			currentLine.clear();
		}
		else
		{
			currentLine += buffer[i];
		}
	}

	// Push any remaining line.
	if (!currentLine.empty())
	{
		lines.push_back(currentLine);
	}

	// Expecting exactly 3 lines: word to guess, try count, server pipe name.
	if (lines.size() != 3)
	{
		throw runtime_error(LineInfo("Malformed initial game data received from server", __FILE__, __LINE__));
	}

	// Assign values.
	wordToGuess = lines[0];
	tryCount = stoi(lines[1]);
	serverReadPipe = lines[2];
}

// Game loop: display, get user input, send guess, update try count.
void playGameLoop(int tryCount, const string &randomWord, const string &serverReadPipe)
{
	string currentHiddenWord(randomWord.length(), '-'); // start with all dashes
	bool gameOver = false;

	while (!gameOver)
	{
		cout << "\nWord: " << currentHiddenWord << "\n";
		cout << "Tries: " << tryCount << "\n";

		cout << "Enter a letter guess: ";
		char guessChar;
		cin >> guessChar;

		// Send guess to server
		string guessMessage(1, guessChar);
		if (write(clientWriteFD, guessMessage.c_str(), guessMessage.length()) == -1)
		{
			throw runtime_error(LineInfo("Client failed to write guess to server", __FILE__, __LINE__));
		}

		// Read server response
		char buffer[BUFFER_SIZE] = {0};
		ssize_t bytesRead = read(clientReadFD, buffer, BUFFER_SIZE - 1);
		if (bytesRead <= 0)
		{
			throw runtime_error(LineInfo("Client failed to read response from server", __FILE__, __LINE__));
		}
		buffer[bytesRead] = '\0';

		// Parse response: format is 4 lines:
		// 1. updated hidden word
		// 2. result message (Correct or Incorrect)
		// 3. updated try count
		// 4. status (WIN, LOSE, CONTINUE)

		string response(buffer);
		vector<string> lines;
		string line;

		for (char ch : response)
		{
			if (ch == '\n')
			{
				lines.push_back(line);
				line.clear();
			}
			else
			{
				line += ch;
			}
		}
		if (!line.empty())
		{
			lines.push_back(line);
		}

		if (lines.size() != 4)
		{
			throw runtime_error(LineInfo("Malformed response from server", __FILE__, __LINE__));
		}

		currentHiddenWord = lines[0];
		string resultMsg = lines[1];
		tryCount = stoi(lines[2]);
		string status = lines[3];

		cout << resultMsg << "\n";

		if (status == "WIN")
		{
			cout << "🎉 You guessed the word! The word was: " << randomWord << "\n";
			gameOver = true;
		}
		else if (status == "LOSE")
		{
			cout << "💀 Game Over. The word was: " << randomWord << "\n";
			gameOver = true;
		}
	}
}

// Clean up all pipes.
void cleanUp()
{
	// Close client read pipe.
	if (clientReadFD != -1)
	{
		if (close(clientReadFD) == -1)
		{
			cerr << LineInfo("Failed to close clientReadFD", __FILE__, __LINE__) << endl;
		}
		clientReadFD = -1;
	}

	// Close client write pipe.
	if (clientWriteFD != -1)
	{
		if (close(clientWriteFD) == -1)
		{
			cerr << LineInfo("Failed to close clientWriteFD", __FILE__, __LINE__) << endl;
		}
		clientWriteFD = -1;
	}

	// Remove client's private read pipe
	if (!clientReadPipe.empty())
	{
		if (unlink(clientReadPipe.c_str()) == -1)
		{
			cerr << LineInfo("Failed to unlink client read pipe: " + clientReadPipe, __FILE__, __LINE__) << endl;
		}

		clientReadPipe.clear();
	}
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