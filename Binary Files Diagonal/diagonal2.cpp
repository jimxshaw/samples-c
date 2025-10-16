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
    if (argc < 2)
    {
      throw domain_error(LineInfo("Usage: diagonal < textstring>", __FILE__, __LINE__));
    }

    // Define variables.
    char const dot = '.';
    int blockCounter = 0;

    ofstream ofilestreamobj("diagonal2.bin", ios::binary | ios::out);

    if (!ofilestreamobj.is_open())
    {
      throw domain_error(LineInfo("Open Failed File: diagonal.bin", __FILE__, __LINE__));
    }

    // Replace all 0 with the dot in the binary file.
    for (int col = 0; col < LINESIZE * (argc + 1); col++)
    {
      for (int col = 0; col < LINESIZE; col++)
      {
        ofilestreamobj.write(reinterpret_cast<const char *>(&dot), sizeof(dot));

        if (ofilestreamobj.fail())
        {
          throw domain_error(LineInfo("write() to Output File Failed", __FILE__, __LINE__));
        }
      }
    }

    // Each line of od outputs 16 characters.
    // To make the diagonal output, we will use 0, 15, 30, etc.
    for (int argumentNumber = 1; argumentNumber < argc; argumentNumber++)
    {
      // This is the word's length that we will loop over.
      int wordLength = strlen(argv[argumentNumber]);

      // Even argument number will go from top right to buttom left.
      if (argumentNumber % 2 == 0)
      {
        // The adjustment moves to where the next letter will be.
        int adjustment = 15;

        for (int i = 0; i < wordLength; i++)
        {
          ofilestreamobj.seekp(blockCounter + (adjustment * (i + 1)), ios::beg);

          if (ofilestreamobj.fail())
          {
            throw domain_error(LineInfo("seekp() to Output File Failed", __FILE__, __LINE__));
          }

          ofilestreamobj.write(&argv[argumentNumber][i], 1);

          if (ofilestreamobj.fail())
          {
            throw domain_error(LineInfo("write() to Output File Failed", __FILE__, __LINE__));
          }
        }
      }
      else
      {
        // Odd argument number will go from top left to bottom right.

        // The adjustment moves to where the next letter will be.
        int adjustment = 17;

        for (int i = 0; i < wordLength; i++)
        {
          ofilestreamobj.seekp(blockCounter + (adjustment * (i + 1)), ios::beg);

          if (ofilestreamobj.fail())
          {
            throw domain_error(LineInfo("seekp() to Output File Failed", __FILE__, __LINE__));
          }

          ofilestreamobj.write(&argv[argumentNumber][i], 1);

          if (ofilestreamobj.fail())
          {
            throw domain_error(LineInfo("write() to Output File Failed", __FILE__, __LINE__));
          }
        }
      }

      blockCounter += 256;
    }

    ofilestreamobj.close();

    cout << endl
         << "diagonal2.bin has been created." << endl
         << endl
         << "Use od -c diagonal2.bin to see the contents." << endl
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
