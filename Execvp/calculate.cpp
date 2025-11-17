#include <string>
#include <fstream>
#include <iostream>

#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>

#include "LineInfo.h"

using namespace std;

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
