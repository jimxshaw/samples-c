/*
Program Name: Find the Bison
Date: 10/12/2025
Author: James Shaw
Purpose: Count the number of open "((" and closed "))" pairs of parentheses,
          which represents Bison legs.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <locale>
#include <iomanip>
#include <stdexcept>

#include "LineInfo.h"

using namespace std;
using namespace chrono;

int main()
{
  const int TEST_CASES = 10;

  for (int index = 1; index <= TEST_CASES; ++index)
  {
    try
    {
    }
    catch (exception &ex)
    {
      cout << ex.what() << endl;
      cout << endl
           << "Press Enter once or twice to exit..." << endl;
      cin.ignore();
      cin.get();
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
