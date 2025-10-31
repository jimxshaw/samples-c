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
	// Read-Write for owners.
	const mode_t PIPE_PERMISSIONS = 0600;

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