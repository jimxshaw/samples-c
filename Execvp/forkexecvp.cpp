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