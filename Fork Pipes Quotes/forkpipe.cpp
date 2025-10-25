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
  FILE *filePointer = fopen(QUOTES_FILE_NAME, "r");

  if (filePointer == NULL)
  {
    throw domain_error(LineInfo("Error when trying to open file", __FILE__, __LINE__));
  }

  char input[MAX_BUFFER_SIZE];

  int size = sizeof(input);

  while (fgets(input, size, filePointer) != NULL)
  {
    quotesArray[numOfLines] = strdup(input);
    numOfLines++;
  }
}

// The parent sends messages to the child and reads responses from the child.
void executeParentProcess(int pipeParentWriteChildRead[2], int pipeParentReadChildWrite[2], int numQuotesToRequest)
{
  if (close(pipeParentWriteChildRead[READ]) == PIPE_ERROR)
  {
    throw domain_error(LineInfo("Error when trying to close pipe", __FILE__, __LINE__));
  }

  if (close(pipeParentReadChildWrite[WRITE]) == PIPE_ERROR)
  {
    throw domain_error(LineInfo("Error when trying to close pipe", __FILE__, __LINE__));
  }

  for (int i = 0; i < numQuotesToRequest; i++)
  {
    cout << "In Parent: Write to pipe getQuoteMessage sent Message: Get Quote" << endl;

    if (write(pipeParentWriteChildRead[WRITE], GET_QUOTE_MESSAGE, sizeof(GET_QUOTE_MESSAGE)) == PIPE_ERROR)
    {
      throw domain_error(LineInfo("Error when trying to write pipe", __FILE__, __LINE__));
    }

    char ParentReadChildMessage[MAX_BUFFER_SIZE] = {0};

    if (read(pipeParentReadChildWrite[READ], ParentReadChildMessage, sizeof(ParentReadChildMessage)) == PIPE_ERROR)
    {
      throw domain_error(LineInfo("Error when trying to read pipe", __FILE__, __LINE__));
    }

    cout << "In Parent: Read from pipe for pipeParentReadChildMessage read Message: " << endl
         << ParentReadChildMessage << endl
         << endl
         << "---------------------------" << endl;
  }

  if (write(pipeParentWriteChildRead[WRITE], EXIT_MESSAGE, sizeof(EXIT_MESSAGE)) == PIPE_ERROR)
  {
    throw domain_error(LineInfo("Error when trying to write pipe", __FILE__, __LINE__));
  }

  cout << "In Parent: Write to pipe for pipeParentWriteChildExitMessage sent Message: Exit" << endl;

  if (close(pipeParentWriteChildRead[WRITE]) == PIPE_ERROR)
  {
    throw domain_error(LineInfo("Error when trying to close write pipe", __FILE__, __LINE__));
  }

  if (close(pipeParentReadChildWrite[READ]) == PIPE_ERROR)
  {
    throw domain_error(LineInfo("Error when trying to close read pipe", __FILE__, __LINE__));
  }

  cout << "Parent Done" << endl;
}

// The child receives messages from the parent and responds with quotes.
void executeChildProcess(int pipeParentWriteChildRead[2], int pipeParentReadChildWrite[2], char *quotesArray[], unsigned numOfLines)
{
  if (close(pipeParentReadChildWrite[WRITE]) == PIPE_ERROR)
  {
    throw domain_error(LineInfo("Error when trying to close pipe", __FILE__, __LINE__));
  }

  if (close(pipeParentWriteChildRead[READ]) == PIPE_ERROR)
  {
    throw domain_error(LineInfo("Error when trying to close pipe", __FILE__, __LINE__));
  }

  time_t t;
  srand((unsigned)time(&t));

  do
  {
    char receivedMessage[MAX_BUFFER_SIZE] = {0};

    if (read(pipeParentWriteChildRead[READ], receivedMessage, sizeof(receivedMessage)) == PIPE_ERROR)
    {
      throw domain_error(LineInfo("Error when trying to read pipe", __FILE__, __LINE__));
    }

    cout << "In Child: Read from pipe for pipeParentWriteChildMessage read Message: " << receivedMessage << endl;

    // Check the message.
    char *indexPointer = strstr(receivedMessage, EXIT_MESSAGE);

    // Exit message is found.
    if (indexPointer != NULL)
    {
      break;
    }

    indexPointer = strstr(receivedMessage, GET_QUOTE_MESSAGE);

    // Get Quote message is found.
    if (indexPointer != NULL)
    {
      // Get random quote line choice 0 to < numOfLines.
      int randomLineChoice = (rand() % numOfLines);

      char quoteMessage[1000] = {0};

      int size = strlen(quotesArray[randomLineChoice]);

      for (int i = 0; i < size; i++)
      {
        quoteMessage[i] == *(quotesArray[randomLineChoice] + i);

        cout << "In Child: Write to pipe for pipeParentReadChildMessage sent Message: " << endl
             << quoteMessage << endl;

        if (write(pipeParentReadChildWrite[WRITE], quoteMessage, sizeof(quoteMessage)) == PIPE_ERROR)
        {
          throw domain_error(LineInfo("Error when trying to write pipe", __FILE__, __LINE__));
        }
      }
    }
    else
    {
      // Invalid Message.
      cout << "In Child: Invalid message received: " << endl
           << receivedMessage << endl;

      cout << "In Child: Write to pipe pipeParentReadChildMessage sent" << endl
           << "Message: " << receivedMessage;

      if (write(pipeParentReadChildWrite[WRITE], receivedMessage, sizeof(receivedMessage)) == PIPE_ERROR)
      {
        throw domain_error(LineInfo("Error when trying to write pipe", __FILE__, __LINE__));
      }
    }
  } while (true);

  if (close(pipeParentReadChildWrite[READ]) == PIPE_ERROR)
  {
    throw domain_error(LineInfo("Error when trying to close read pipe", __FILE__, __LINE__));
  }

  if (close(pipeParentWriteChildRead[WRITE]) == PIPE_ERROR)
  {
    throw domain_error(LineInfo("Error when trying to close write pipe", __FILE__, __LINE__));
  }

  cout << "Child Done" << endl;
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
    char *quotesArray[1000];
    unsigned numOfLines;

    // Retrieve the quotes array from the file.
    getQuotesArray(quotesArray, numOfLines);

    // Process ID.
    int pid;

    // Declare file descriptor pipe arrays.
    // Child will read from [0], parent will write to [1].
    int pipeParentWriteChildRead[2];

    // Parent will read from [0], child will write to [1].
    int pipeParentReadChildWrite[2];

    // TODO: Create the pipes. Throw if fail.
    if (pipe(pipeParentWriteChildRead) == PIPE_ERROR)
    {
      throw domain_error(LineInfo("Error: could not create pipe for pipeParentWriteChildRead", __FILE__, __LINE__));
    }

    if (pipe(pipeParentReadChildWrite) == PIPE_ERROR)
    {
      throw domain_error(LineInfo("Error: could not create pipe for pipeParentReadChildWrite", __FILE__, __LINE__));
    }

    cout << endl
         << endl;

    pid = fork();

    if (pid == FORK_ERROR)
    {
      throw domain_error(LineInfo("Error when trying to fork", __FILE__, __LINE__));
    }
    else if (pid == CHILD_PID)
    {
      // Child Process.
      executeChildProcess(pipeParentWriteChildRead, pipeParentReadChildWrite, quotesArray, numOfLines);
    }
    else
    {
      // Parent Process.
      executeParentProcess(pipeParentWriteChildRead, pipeParentReadChildWrite, numQuotesToRequest);
    }
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