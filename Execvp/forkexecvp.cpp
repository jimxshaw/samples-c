#include <string>
#include <fstream>
#include <iostream>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include <stdexcept>
#include <sstream>

#include "LineInfo.h"

using namespace std;

int const READ = 0;
int const WRITE = 1;
int const PIPE_ERROR = -1;
int const CHILD_PID = 0;
int const MAX_PIPE_MESSAGE_SIZE = 1000;
int const EXECVP_FAILED = -1;

void CreateFileFromArgs(string randFileName, int randNums, int randRange)
{
  ofstream outfileStream(randFileName);

  if (outfileStream.fail())
  {
    stringstream s;

    s << "Error opening Random Numbers file " << randFileName << endl;

    throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
  }

  time_t t;
  srand((int)time(&t));

  for (int i = 0; i < randNums; ++i)
  {
    outfileStream << ((rand() % randRange) + 1) << endl;
  }

  outfileStream.close();
}

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 4)
    {
      stringstream s;

      s << endl
        << endl
        << "Wrong arguments" << endl
        << "Usage: " << endl
        << "forkexecvp <file to create> <number of random numbers> <random number range> " << endl
        << "Example:" << endl
        << "forkexecvp randomnumber 500 50 " << endl
        << endl;

      throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
    }

    string randFileStr = argv[1];
    unsigned numRandNumbers = atoi(argv[2]);
    unsigned randRange = atoi(argv[3]);

    cout << endl
         << argv[0] << " " << randFileStr << " " << numRandNumbers << " " << randRange << endl;

    CreateFileFromArgs(randFileStr, numRandNumbers, randRange);

    pid_t pid;
    int pipeParentWriteChildRead[2];

    string messages[] = {"sum", "average", "greatest", "least"};

    int numMessages = sizeof(messages) / sizeof(messages[0]);

    for (int childProcessNum = 0; childProcessNum < numMessages; ++childProcessNum)
    {
      if (pipe(pipeParentWriteChildRead) == PIPE_ERROR)
      {
        stringstream s;

        s << "Failed to create pipe pipeParentWriteChildRead";

        throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
      }

      // Initiate the fork.
      pid_t forkpid = fork();

      if (forkpid < 0)
      {
        stringstream s;

        s << "Failed to fork";

        throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
      }

      if (forkpid != CHILD_PID)
      {
        // Parent Process.
        close(pipeParentWriteChildRead[READ]);

        cout << "Parent pid: " << getpid() << " to Child Process number: " << childProcessNum << endl
             << "Send Message: " << messages[childProcessNum] << endl;

        if (write(pipeParentWriteChildRead[WRITE],
                  messages[childProcessNum].c_str(),
                  sizeof(messages[childProcessNum].c_str())) == PIPE_ERROR)
        {
          stringstream s;

          s << "Pipe write failed";

          throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
        }
      }
      else
      {
        // Child Process.
        close(pipeParentWriteChildRead[WRITE]);

        char pipeReadMessage[MAX_PIPE_MESSAGE_SIZE] = {0};

        if (read(pipeParentWriteChildRead[READ],
                 pipeReadMessage,
                 sizeof(pipeReadMessage)) == PIPE_ERROR)
        {
          stringstream s;

          s << "Pipe read failed";

          throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
        }

        cout << "Child pid: " << getpid() << " Child Process number: " << childProcessNum
             << " Received Message: " << pipeReadMessage << endl;

        // Execvp.
        char *arglist[] = {(char *)"./calculate", pipeReadMessage, (char *)randFileStr.c_str(), NULL};

        cout << "Child pid: " << getpid() << " Child Process number: " << childProcessNum << endl
             << "execvp(" << arglist[0] << ", ./calculate, " << randFileStr.c_str() << ", NULL)" << endl;

        string outputFileName = "output";

        outputFileName += pipeReadMessage;
        outputFileName += ".txt";

        freopen(outputFileName.c_str(), "w", stdout);

        close(pipeParentWriteChildRead[READ]);

        if (execvp(arglist[0], arglist) == EXECVP_FAILED)
        {
          stringstream s;

          s << "Execvp failed";

          throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
        }
      }
    }

    cout << "Parent pid: " << getpid() << " Start - Wait for all calculate children to finish" << endl;

    while (wait(NULL) > 0)
    {
      // Block here and repeatedly call wait() until it returns -1, meaning no children.
      // Don’t run any code inside the loop.
    }

    cout << "Parent pid: " << getpid() << " Done - Wait for all calculate children to finish"
         << endl
         << endl;

    close(pipeParentWriteChildRead[WRITE]);

    cout << "Parent pid: " << getpid() << " Use execvp() cat to display output files: " << endl;

    for (int childProcessNum = 0; childProcessNum < numMessages; ++childProcessNum)
    {
      pid_t forkpid = fork();

      if (forkpid < 0)
      {
        stringstream s;

        s << "Failed to fork";

        throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
      }

      if (forkpid == CHILD_PID)
      {
        // Child Process.
        string outputFileName = "output";

        outputFileName += messages[childProcessNum];
        outputFileName += ".txt";

        char *arglist[] = {(char *)"cat", (char *)outputFileName.c_str(), NULL};

        cout << "Child pid: " << getpid() << endl
             << "cat " << outputFileName << endl;

        if (execvp("cat", arglist) == EXECVP_FAILED)
        {
          stringstream s;

          s << "Execvp failed";

          throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
        }
      }
    }

    cout << "Parent pid: " << getpid() << " Start - Wait for all calculate children to finish" << endl;

    while (wait(NULL) > 0)
    {
      // Block here and repeatedly call wait() until it returns -1, meaning no children.
      // Don’t run any code inside the loop.
    }

    cout << "Parent pid: " << getpid() << " Done - Wait for all calculate children to finish"
         << endl
         << endl;
  }
  catch (exception &ex)
  {
    cout << ex.what() << endl
         << endl;
    cout << "Press the enter key once or twice to leave..." << endl
         << endl;
    cin.ignore();
    cin.get();

    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}