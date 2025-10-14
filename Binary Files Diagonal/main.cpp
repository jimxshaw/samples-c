/*
Program Name: Binary Files Diagonal
Date: 10/14/2025
Author: James Shaw
Purpose: Take multiple word arguments from the command line
          and writes each word diagonally into a binary file (diagonal.bin),
          so that when viewed using `od -c`, each word appears in its own
          16x16 block, alternating direction:
          - First word: top-left to bottom-right
          - Second word: top-right to bottom-left
          - Third word: top-left to bottom-right
          ... and so on.
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

#include "LineInfo.h"

using namespace std;

const int LINESIZE = 16;

// use one command line argument
int main(int argc, char *argv[])
{
  try
  {
    if (argc != 2)
    {
      throw domain_error(LineInfo("Usage: diagonal < textstring>", __FILE__, __LINE__));
    }

    // create a file with 16 empty rows, required by with od -c command

    ofstream ofilestreamobj("diagonal.bin", ios::binary | ios::out);
    if (!ofilestreamobj.is_open())
      throw domain_error(LineInfo("Open Failed File: diagonal.bin", __FILE__, __LINE__));

    char const space = '.';

    for (int line = 0; line < LINESIZE; line++)
      for (int column = 0; column < LINESIZE; column++)
      {
        ofilestreamobj.write(reinterpret_cast<const char *>(&space), sizeof(space));
        if (ofilestreamobj.fail())
          throw domain_error(LineInfo("write() failed", __FILE__, __LINE__));
      }

    // Each line of od outputs is 16 characters
    // So, to make the output diagonal, we will use 0, 17, 34, ....

    int n = strlen(argv[1]);
    for (int i = 0; i < n; i++)
    {
      ofilestreamobj.seekp((static_cast<long long>(LINESIZE) + 1) * static_cast<basic_ostream<char, char_traits<char>>::off_type>(i),
                           ios::beg);
      if (ofilestreamobj.fail())
        throw domain_error(LineInfo("seekp() failed", __FILE__, __LINE__));

      ofilestreamobj.write(reinterpret_cast<const char *>(&argv[1][i]), sizeof(argv[1][i]));
      if (ofilestreamobj.fail())
        throw domain_error(LineInfo("write() failed", __FILE__, __LINE__));
    }

    ofilestreamobj.close();

    cout << endl
         << "diagonal.bin has been created." << endl
         << endl
         << "Use od -c diagonal.bin to see the contents." << endl
         << endl;
  }
  catch (exception &e)
  {
    cout << e.what() << endl;
    cout << endl
         << "Press the enter key once or twice to leave..." << endl;
    cin.ignore();
    cin.get();
    exit(EXIT_FAILURE);
  } // catch

  exit(EXIT_SUCCESS);
}
