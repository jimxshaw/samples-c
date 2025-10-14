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

  for (int fileIndex = 1; fileIndex <= TEST_CASES; ++fileIndex)
  {
    try
    {
      // Build the input and output filenames.
      string inputFileName = string((fileIndex < 10 ? "0" : "")) + to_string(fileIndex) + "bisonsearchin.txt";
      string outputFileName = string((fileIndex < 10 ? "0" : "")) + to_string(fileIndex) + "bisonfoundin.txt";

      // Open the input file.
      ifstream inputFile(inputFileName);

      if (inputFile.fail())
      {
        throw domain_error(LineInfo("Failed to open the input file: " + inputFileName, __FILE__, __LINE__));
      }

      // Read the string pattern from the input file.
      string pattern;
      getline(inputFile, pattern);
      inputFile.close();

      // Start the timer.
      auto startTime = steady_clock::now();

      // Logic to count the bison tracks.
      int bisonPatternCount = 0;
      int backLegCount = 0;

      for (size_t i = 1; i < pattern.length(); ++i)
      {
        if (pattern[i - 1] == '(' && pattern[i] == '(')
        {
          ++backLegCount;
        }
        else if (pattern[i - 1] == ')' && pattern[i] == ')')
        {
          bisonPatternCount += backLegCount;
        }
      }

      // Stop the timer.
      auto elapsedTime = duration_cast<nanoseconds>(steady_clock::now() - startTime);

      // Open the output file.
      ofstream outputFile(outputFileName);

      if (outputFile.fail())
      {
        throw domain_error(LineInfo("Failed to open output file: " + outputFileName, __FILE__, __LINE__));
      }

      outputFile.imbue(locale(""));

      // Write to the output file.
      outputFile << "Time Elapsed (nano) : " << elapsedTime.count() << endl;
      outputFile << "Found Pattern Count : " << bisonPatternCount << endl;
      outputFile << "Searched Pattern    : " << endl
                 << endl;
      outputFile << pattern << endl;

      outputFile.close();
    }
    catch (exception &ex)
    {
      cout << ex.what() << endl;
      cout << endl
           << "Press Enter twice to exit..." << endl;
      cin.ignore();
      cin.get();
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
