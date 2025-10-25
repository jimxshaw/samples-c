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

#include "LineInfo.h"

using namespace std;

static const char *const QUOTES_FILE_NAME = "quotes.txt";

const int MAX_PIPE_MESSAGE_SIZE = 1000;
const int MAX_QUOTE_LINE_SIZE = 1000;

const int READ = 0;
const int WRITE = 1;

const int PIPE_ERROR = -1;
const int FORK_ERROR = -1;

const int CHILD_PID = 0;

// Read lines from text file into the array and sets
// the total number of lines found.
void getQuotesArray(char *lines[], unsigned &noLines)
{
  // Open file in read mode.
  FILE *file = fopen(QUOTES_FILE_NAME, "r");

  if (!file)
  {
    throw domain_error(LineInfo("Error when trying to open file", __FILE__, __LINE__));
  }

  // Temp buffer for each line.
  char buffer[MAX_QUOTE_LINE_SIZE];
  noLines = 0;

  // Read each line until the end of file or we reach the limit.
  while (fgets(buffer, sizeof(buffer), file) != NULL)
  {
    // Remove trailing new line.
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
      buffer[len - 1] = '\0';
    }

    // Dynamically allocate memory for this line.
    lines[noLines] = new char[len + 1];

    // Copy into heap memory.
    strcpy(lines[noLines], buffer);

    noLines++;

    // Prevent overflows if file has too many lines.
    if (noLines >= MAX_QUOTE_LINE_SIZE)
    {
      break;
    }
  }

  // Always close the file.
  fclose(file);
}

// The parent sends messages to the child and reads responses from the child.
void executeParentProcess(int pipeParentWriteChildRead[], int pipeParentReadChildWrite[], int numParentMessagesToSend)
{
  // Close unused pipe ends.
  // Close read end of the parent to child pipe.
  close(pipeParentWriteChildRead[READ]);
  // Close write end of the child to parent pipe.
  close(pipeParentReadChildWrite[WRITE]);

  // Declare message buffers.
  // This is what the parent sends to the child.
  char sendMessage[MAX_PIPE_MESSAGE_SIZE];
  // This is what the parent receives from the child.
  char receiveMessage[MAX_PIPE_MESSAGE_SIZE];

  // Loop to request the quotes.
  for (int i = 0; i < numParentMessagesToSend; ++i)
  {
    // Send "Get Quote" message to the child.
    strcpy(sendMessage, "Get Quote");

    if (write(pipeParentWriteChildRead[WRITE], sendMessage, strlen(sendMessage) + 1) == -1)
    {
      throw domain_error(LineInfo("Parent failed to write 'Get Quote'", __FILE__, __LINE__));
    }

    cout << "In Parent: Write to pipe getQuoteMessage sent Message: " << sendMessage << endl;

    // Clear receive buffer before reading.
    memset(receiveMessage, 0, MAX_PIPE_MESSAGE_SIZE);

    // Read child's response.
    if (read(pipeParentReadChildWrite[READ], receiveMessage, MAX_PIPE_MESSAGE_SIZE) == -1)
    {
      throw domain_error(LineInfo("Parent failed to read quote", __FILE__, __LINE__));
    }

    cout << "In Parent: Read from pipe pipeParentReadChildMessage read Message:\n"
         << receiveMessage << endl
         << endl;

    cout << "-----------------------------------\n";

    // Send "Exit" message to child after done.
    strcpy(sendMessage, "Exit");

    if (write(pipeParentWriteChildRead[WRITE], sendMessage, strlen(sendMessage) + 1) == -1)
    {
      throw domain_error(LineInfo("Parent failed to write Exit message", __FILE__, __LINE__));
    }

    cout << "In Parent: Write to pipe ParentWriteChildExitMessage sent Message: " << sendMessage << endl;

    // Close the used pipe ends.
    close(pipeParentWriteChildRead[WRITE]);
    close(pipeParentReadChildWrite[READ]);

    cout << "Parent Done" << std::endl;
  }
}

// The child receives messages from the parent and responds with quotes.
void executeChildProcess(int pipeParentWriteChildRead[], int pipeParentReadChildWrite[], char *lines[], unsigned noLines)
{
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

    // Convert argument to integer, the number of quotes to request.
    int numQuotesToRequest = atoi(argv[1]);

    if (numQuotesToRequest <= 0)
    {
      throw domain_error(LineInfo("Quote request number must be 1 or more", __FILE__, __LINE__));
    }

    // Initialize pipes.
    // Parent writes, Child reads.
    int pipePWriteCRead[2];
    // Child writes, Parent reads.
    int pipePReadCWrite[2];

    if (pipe(pipePWriteCRead) == -1)
    {
      throw domain_error(LineInfo("Failed to create pipe for parent to child", __FILE__, __LINE__));
    }

    if (pipe(pipePReadCWrite) == -1)
    {
      throw domain_error(LineInfo("Failed to create pipe for child to parent", __FILE__, __LINE__));
    }

    // Load quotes from file.
    char *quotes[MAX_QUOTE_LINE_SIZE];
    unsigned quoteCount = 0;

    getQuotesArray(quotes, quoteCount);

    // Create child process.
    pid_t pid = fork();

    if (pid < CHILD_PID)
    {
      throw domain_error(LineInfo("Fork failed", __FILE__, __LINE__));
    }
    else if (pid == CHILD_PID)
    {
      // Child Process.
      executeChildProcess(pipePWriteCRead, pipePReadCWrite, quotes, quoteCount);
    }
    else
    {
      // Parent Process.
      executeParentProcess(pipePWriteCRead, pipePReadCWrite, numQuotesToRequest);
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
