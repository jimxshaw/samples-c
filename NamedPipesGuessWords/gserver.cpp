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
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <random>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include "LineInfo.h"

using namespace std;
const int BUF_SIZE = 100;
const int MAX_TRIES = 12;
const string MAIN_PIPE = "server_request_fifo";

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cerr << "Usage: ./gserver words.txt\n";
		return 1;
	}

	try
	{
		// Load words into vector<string>.
		ifstream infile(argv[1]);
		if (!infile)
			throw runtime_error(LineInfo("Failed to open words.txt", __FILE__, __LINE__));
		vector<string> words;
		copy(istream_iterator<string>(infile), istream_iterator<string>(), back_inserter(words));
		infile.close();

		// Create known named pipe (ignore mkfifo return).
		mkfifo(MAIN_PIPE.c_str(), 0666);
		int reqFd = open(MAIN_PIPE.c_str(), O_RDONLY);
		if (reqFd < 0)
			throw runtime_error(LineInfo("open MAIN_PIPE failed", __FILE__, __LINE__));

		// Read client-created pipe name.
		char clientPipeName[BUF_SIZE] = {0};
		read(reqFd, clientPipeName, BUF_SIZE);
		close(reqFd);
		unlink(MAIN_PIPE.c_str());

		// Connect to client pipe (for writing to client).
		int clientFd = open(clientPipeName, O_WRONLY);
		if (clientFd < 0)
			throw runtime_error(LineInfo("open clientPipeName failed", __FILE__, __LINE__));

		// Pick random word.
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(0, words.size() - 1);
		string randomWord = words[dist(gen)];
		string guessWord(randomWord.size(), '-');
		int tryCount = 1;

		// Fork child to handle the game loop.
		pid_t pid = fork();
		if (pid < 0)
			throw runtime_error(LineInfo("fork failed", __FILE__, __LINE__));

		if (pid == 0)
		{
			// Child process.
			string tryStr = to_string(tryCount);
			write(clientFd, tryStr.c_str(), BUF_SIZE);
			sleep(3);
			write(clientFd, randomWord.c_str(), BUF_SIZE);

			// Create child’s unique pipe (for reading guesses)
			string serverReadPipe = "server_child_read_fifo";
			mkfifo(serverReadPipe.c_str(), 0666);
			write(clientFd, serverReadPipe.c_str(), BUF_SIZE);

			int readFd = open(serverReadPipe.c_str(), O_RDONLY);
			if (readFd < 0)
				throw runtime_error(LineInfo("open serverReadPipe failed", __FILE__, __LINE__));

			// Game loop.
			char guessBuf[BUF_SIZE];
			while (true)
			{
				write(clientFd, guessWord.c_str(), BUF_SIZE);
				memset(guessBuf, 0, BUF_SIZE);
				int n = read(readFd, guessBuf, BUF_SIZE);
				if (n <= 0)
					break;
				char letter = guessBuf[0];
				bool found = false;
				for (size_t i = 0; i < randomWord.size(); ++i)
					if (randomWord[i] == letter)
					{
						guessWord[i] = letter;
						found = true;
					}
				tryCount++;
				if (guessWord == randomWord || tryCount > MAX_TRIES)
					break;
			}

			close(readFd);
			close(clientFd);
			unlink(serverReadPipe.c_str());
			_exit(0);
		}
		else
		{
			// Parent process.
			close(clientFd);
			wait(nullptr);
		}
	}
	catch (exception &e)
	{
		cerr << e.what() << endl;
	}
	return 0;
}
