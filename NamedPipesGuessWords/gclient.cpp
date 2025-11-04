/*
Program Name: Named Pipes Guess Words
Date: 10/27/2025
Author: James Shaw
Purpose: This is a client-server word guessing game using named pipes,
				forked processes, separate executables and a shared word list
				list to provide randomized guess words.
*/
#include <iostream>
#include <string>
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

int main()
{
	try
	{
		// Client creates its private pipe for server writes[]
		string clientPipe = "client_read_fifo";
		mkfifo(clientPipe.c_str(), 0666);

		// Send client pipe name to server via known pipe.
		int reqFd = open(MAIN_PIPE.c_str(), O_WRONLY);
		if (reqFd < 0)
			throw runtime_error(LineInfo("open MAIN_PIPE failed", __FILE__, __LINE__));
		write(reqFd, clientPipe.c_str(), BUF_SIZE);
		close(reqFd);

		// Read from the client pipe.
		int clientReadFd = open(clientPipe.c_str(), O_RDONLY);
		if (clientReadFd < 0)
			throw runtime_error(LineInfo("open clientPipe failed", __FILE__, __LINE__));

		char buf[BUF_SIZE] = {0};
		read(clientReadFd, buf, BUF_SIZE);
		int tryCount = stoi(buf);

		read(clientReadFd, buf, BUF_SIZE);
		string randomWord(buf);
		read(clientReadFd, buf, BUF_SIZE);
		string serverReadPipe(buf);

		// Connect to server’s read pipe for sending guesses.
		int serverWriteFd = open(serverReadPipe.c_str(), O_WRONLY);
		if (serverWriteFd < 0)
			throw runtime_error(LineInfo("open serverReadPipe failed", __FILE__, __LINE__));

		cout << "\nGame Start\nYou have 12 letter guesses to win\n";

		string guessWord;
		while (true)
		{
			read(clientReadFd, buf, BUF_SIZE);
			guessWord = buf;

			if (guessWord == randomWord)
			{
				cout << "\nYou Win!\n";
				break;
			}
			if (tryCount > MAX_TRIES)
			{
				cout << "\nOut of tries : " << MAX_TRIES << " allowed\n";
				cout << "The word is: " << randomWord << endl;
				break;
			}

			cout << "\nCurrent try number : " << tryCount
					 << "\n(Guess) Enter a letter in the word : " << guessWord << endl;
			char letter;
			cin >> letter;
			write(serverWriteFd, &letter, 1);
			tryCount++;
		}

		close(clientReadFd);
		close(serverWriteFd);
		unlink(clientPipe.c_str());
	}
	catch (exception &e)
	{
		cerr << e.what() << endl;
	}
	return 0;
}
