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
#include <cstdlib>		 // For rand(), srand()
#include <ctime>			 // For time()
#include <unistd.h>		 // For fork(), getpid()
#include <fcntl.h>		 // For open()
#include <sys/stat.h>	 // For mkfifo()
#include <sys/types.h> // For pid_t
#include <sys/wait.h>	 // For waitpid()

#include "LineInfo.h" // Provided error handling

using namespace std;

static const char *const WORDS_FILE_NAME = "words.txt";

const string SERVER_REQUEST_PIPE = "request.pipe"; // Well-known pipe clients write to.
const int MAX_TRIES = 12;													 // Max number of incorrect guesses.
const int BUFFER_SIZE = 100;											 // For pipe I/O buffer.
const string SERVER_PIPE_PREFIX = "server";				 // E.g. "server_child.pipe".
const mode_t PIPE_PERMISSIONS = 0600;							 // Read-Write for owners.

vector<string> wordList;	 // All words from words.txt.
string clientPipeName;		 // Received from client.
string serverToClientPipe; // Named pipe used to send initial game data.
string clientToServerPipe; // Pipe used by child process to read guesses.

// Reads words from a given file and populates the global wordList vector.
void loadWordsFromFile(const string &fileName)
{
	ifstream inFile(fileName);

	// Check if the file opened successfully.
	if (!inFile)
	{
		throw domain_error(LineInfo("Unable to open words file: " + fileName, __FILE__, __LINE__));
	}

	string line;

	while (getline(inFile, line))
	{
		// Skip blank lines, if there are any.
		if (!line.empty())
		{
			wordList.push_back(line);
		}
	}

	inFile.close();

	// Make sure we load at least one word.
	if (wordList.empty())
	{
		throw domain_error(LineInfo("No words loaded from file: " + fileName, __FILE__, __LINE__));
	}
}

// Creates the request pipe.
void createRequestPipe()
{
	// Try to create the FIFO file (named pipe).
	if (mkfifo(SERVER_REQUEST_PIPE.c_str(), PIPE_PERMISSIONS) == -1)
	{
		if (errno != EEXIST)
		{
			// Only throw an error if it's NOT due to pipe already existing.
			throw domain_error(LineInfo("Failed to create request pipe", __FILE__, __LINE__));
		}

		// If the pipe already exists then it's not a failure in this context.
	}
}

// Reads the client's private pipe name from the well-known pipe.
void receiveClientPipeName()
{
	char buffer[BUFFER_SIZE] = {0};

	// Open the well-known pipe in read-only mode.
	int fileDescriptor = open(SERVER_REQUEST_PIPE.c_str(), O_RDONLY);
	if (fileDescriptor == -1)
	{
		throw domain_error(LineInfo("Failed to open request pipe", __FILE__, __LINE__));
	}

	// Read the client's private pipe name into buffer.
	ssize_t bytesRead = read(fileDescriptor, buffer, sizeof(buffer));
	if (bytesRead <= 0)
	{
		// Close if the read fails.
		close(fileDescriptor);
		throw domain_error(LineInfo("Failed to read client pipe name", __FILE__, __LINE__));
	}

	// Store the pipe name into the global string variable.
	clientToServerPipe = string(buffer);

	// Done with this pipe.
	close(fileDescriptor);
}

// Sends the initial game setup to the client:
// - the word to guess
// - the blanked-out guess word
// - try count (0)
// - the name of the server’s pipe (to receive future guesses)
void sendInitialGameData(const string &chosenWord)
{
	// Create a string with dashes matching the word length.
	string hidden = createHiddenWord(chosenWord);

	// Create the message, which is dashes + new line + word length.
	string messageToClient = hidden + "\n" + to_string(chosenWord.length());

	// Convert message to C-style character array.
	const char *messageBuffer = messageToClient.c_str();
	ssize_t numOfBytesToWrite = messageToClient.length();

	// Open the client's private pipe in write-only mode.
	int pipeFileDescriptor = open(clientToServerPipe.c_str(), O_WRONLY);
	if (pipeFileDescriptor == -1)
	{
		throw domain_error(LineInfo("Failed to open client's pipe for writing", __FILE__, __LINE__));
	}

	ssize_t numOfBytesWritten = write(pipeFileDescriptor, messageBuffer, numOfBytesToWrite);
	if (numOfBytesWritten != numOfBytesToWrite)
	{
		// Close to avoid file descriptor leak.
		close(pipeFileDescriptor);
		throw domain_error(LineInfo("Failed to write complete message to client", __FILE__, __LINE__));
	}

	// Close pipe after writing.
	close(pipeFileDescriptor);
}

// Handles the word-guessing game in a child process.
// Listens for letter guesses and updates the game state.
void runGameInChildProcess(const string &word, const string &clientGuessPipe)
{
	// Generate the name of the server pipe, which is used to
	// receive guesses from the client.
	string serverReadPipeName = SERVER_PIPE_PREFIX + "_child.pipe";

	// Create the named pipe.
	if (mkfifo(serverReadPipeName.c_str(), PIPE_PERMISSIONS) == -1 && errno != EEXIST)
	{
		throw domain_error(LineInfo("Failed to create child server pipe", __FILE__, __LINE__));
	}

	// Send the pipe name to the client via the client's pipe.
	int clientWriteFd = open(clientGuessPipe.c_str(), O_WRONLY);
	if (clientWriteFd == -1)
	{
		throw domain_error(LineInfo("Failed to open client pipe for writing to send child pipe name.", __FILE__, __LINE__));
	}

	if (write(clientWriteFd, serverReadPipeName.c_str(), serverReadPipeName.length()) == -1)
	{
		close(clientWriteFd);
		throw domain_error(LineInfo("Failed to write child pipe name to client.", __FILE__, __LINE__));
	}

	close(clientWriteFd); // Done writing pipe name.

	// Open the new server pipe for reading guesses from client.
	int serverReadFd = open(serverReadPipeName.c_str(), O_RDONLY);
	if (serverReadFd == -1)
	{
		throw domain_error(LineInfo("Failed to open child server pipe for reading.", __FILE__, __LINE__));
	}

	// Initialize game variables.
	int tryCount = 1;

	string currentGuess = createHiddenWord(word); // e.g., "------".

	// Game loop: read guesses, update state, respond.
	while (tryCount <= MAX_TRIES)
	{
		// Send current guess word to client.
		int clientWriteFd = open(clientGuessPipe.c_str(), O_WRONLY);
		if (clientWriteFd == -1)
		{
			close(serverReadFd);
			throw domain_error(LineInfo("Failed to reopen client pipe for writing (sending updated word).", __FILE__, __LINE__));
		}

		string message = currentGuess;
		if (write(clientWriteFd, message.c_str(), message.length()) == -1)
		{
			close(clientWriteFd);
			close(serverReadFd);
			throw domain_error(LineInfo("Failed to write current guess word to client.", __FILE__, __LINE__));
		}
		close(clientWriteFd);

		// Read letter from client.
		char guessedLetter;
		int bytesRead = read(serverReadFd, &guessedLetter, 1);
		if (bytesRead == -1)
		{
			close(serverReadFd);
			throw domain_error(LineInfo("Failed to read guessed letter from client.", __FILE__, __LINE__));
		}

		// Update currentGuess with the guessed letter if it exists.
		bool matchFound = false;
		for (size_t i = 0; i < word.length(); ++i)
		{
			if (tolower(word[i]) == tolower(guessedLetter))
			{
				currentGuess[i] = word[i];
				matchFound = true;
			}
		}

		// Check for win.
		if (currentGuess == word)
		{
			// Send winning message to client.
			int winFd = open(clientGuessPipe.c_str(), O_WRONLY);
			if (winFd != -1)
			{
				string winMessage = word + "\nYou Win!\n";
				write(winFd, winMessage.c_str(), winMessage.length());
				close(winFd);
			}

			break;
		}

		tryCount++;
	}

	// If out of tries, send loss message.
	if (tryCount > MAX_TRIES)
	{
		int finalFd = open(clientGuessPipe.c_str(), O_WRONLY);
		if (finalFd != -1)
		{
			string lossMessage = "Out of tries: " + to_string(MAX_TRIES) + "\nThe word is: " + word + "\n";

			write(finalFd, lossMessage.c_str(), lossMessage.length());
			close(finalFd);
		}
	}

	// Clean up the child server pipe.
	close(serverReadFd);
	unlink(serverReadPipeName.c_str());
}

// Randomly selects a word from wordList.
string chooseRandomWord()
{
	// Make sure the word list is not empty.
	if (wordList.empty())
	{
		throw domain_error(LineInfo("Word list is empty", __FILE__, __LINE__));
	}

	// Seed the random number generator only once.
	static bool isRandomSeedInitialized = false;
	if (!isRandomSeedInitialized)
	{
		srand(static_cast<unsigned int>(time(nullptr)));
		isRandomSeedInitialized = true;
	}

	size_t randomIndex = static_cast<size_t>(rand()) % wordList.size();

	return wordList[randomIndex];
}

// Creates a blanked-out version of a word, such as "-----".
string createHiddenWord(const string &word)
{
	string hidden(word.length(), '-');
	return hidden;
}

// Cleans up the known pipe after the server is done.
void cleanUp()
{
	// Remove the known pipe.
	unlink(SERVER_REQUEST_PIPE.c_str());
}

int main()
{
	try
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