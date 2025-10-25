/*
Program Name: Fork Pipes Quotes
Date: 10/22/2025
Author: James Shaw
Purpose: Loads quotes from a file. Create two pipes, one for
        messages from parent to child, one for messages
        from child to parent. Forks a new process. The parent
        sends requests like "Get Quote" or "Exit" to the child
        through the pipe. The child reads those requests,
        responds with quotes and sends them back. After a
        certain number of quotes, the parent tells the child
        to exit. Parent and child terminate gracefully.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <stdexcept>

#include <LineInfo.h>

using namespace std;

static const char *const QUOTES_FILE_NAME = "quotes.txt";

const int MAX_BUFFER_SIZE = 1000;
const int MAX_QUOTES = 1000;

const int READ = 0;
const int WRITE = 1;

const int PIPE_ERROR = -1;
const int FORK_ERROR = -1;

const int CHILD_PID = 0;

const char GET_QUOTE_MESSAGE[] = "Get Quote";
const char EXIT_MESSAGE[] = "Exit";

// Read lines from text file into the array and sets
// the total number of lines found.
void getQuotesArray(char *quotesArray[], unsigned &numOfLines)
{
  // TODO: Implement.
}

// The parent sends messages to the child and reads responses from the child.
void executeParentProcess(int pipeParentWriteChildRead[2], int pipeParentReadChildWrite[2], int numQuotesToRequest)
{
  // TODO: Implement.
}

// The child receives messages from the parent and responds with quotes.
void executeChildProcess(int pipeParentWriteChildRead[2], int pipeParentReadChildWrite[2], char *quotesArray[], unsigned numOfLines)
{
  // TODO: Implement.
}

int main(int argc, char *argv[])
{
  try
  {
    // Validate the command line argument.
    // Check the user provides exactly one argument,
    // in addition to the program name.
    if (argc != 2)
    {
      throw domain_error(LineInfo("Usage: ./forkpipe <number of quotes to request>", __FILE__, __LINE__));
    }

    // Convert argument to integer, atoi is ascii to integer.
    // argv[0] is the program name and argv[1] is the number.
    int numQuotesToRequest = atoi(argv[1]);
    char *lines[1000];
    unsigned numOfLines;

    // Retrieve the quotes array from the file.
    getQuotesArray(lines, numOfLines);

    // Process ID.
    int pid;

    // Declare file descriptor pipe arrays.
    // Child will read from [0], parent will write to [1].
    int pipeParentWriteChildRead[2];

    // Parent will read from [0], child will write to [1].
    int pipeParentReadWhileWrite[2];

    // TODO: Create the pipes. Throw if fail.
    if (pipe(pipeParentWriteChildRead) == PIPE_ERROR)
    {
      throw domain_error(LineInfo("Error: could not create pipe for pipeParentWriteChildRead", __FILE__, __LINE__));
    }

    if (pipe(pipeParentReadWhileWrite) == PIPE_ERROR)
    {
      throw domain_error(LineInfo("Error: could not create pipe for pipeParentReadWhileWrite", __FILE__, __LINE__));
    }

    cout << endl
         << endl;

    // TODO: Load quotes from the file into memory.

    // TODO: Fork a child process.

    // TODO: Run either the parent or child based on PID.

    // TODO: Clean up allocated quote memory.
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