#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>

#include <unistd.h>
#include <pthread.h>

#include "LineInfo.h"

using namespace std;

pthread_mutex_t lock;
static unsigned totalLineCount;

void GetLyricLinesFromFileToVector(string lyricsFilenameStr,
                                   vector<string> &lyricLinesVector,
                                   unsigned &noOfLyricLines)
{

    //$$
}

struct SingLinesThreadInfoStruct
{
    unsigned *singerNoIdPtr;
    vector<string> lyricLinesVector;
    unsigned noOfLyricLines;
};

// cout is not thread safe, so all couts need to be locked and then unlocked
void *SingLinesThread(void *singLinesThreadInfoStructPtr)
{

    SingLinesThreadInfoStruct *threadInfoPtr = (SingLinesThreadInfoStruct *)singLinesThreadInfoStructPtr;
    static unsigned previousNoId;

    const unsigned maxSingLineNo = 10;
    unsigned *singersNoIdPtr = threadInfoPtr->singerNoIdPtr;
    vector<string> lyricLinesVector = threadInfoPtr->lyricLinesVector;
    unsigned noOfLyricLines = threadInfoPtr->noOfLyricLines;

    // $$ lock the mutex

    //$$ display start thread message

    // $$ unlock the mutex

    // sing up to 10 lines at a time
    unsigned singLimit = 0;

    for (unsigned lyricLineNo = 0; lyricLineNo < noOfLyricLines; lyricLineNo += maxSingLineNo)
    {

        unsigned currentLineSungNo = lyricLineNo;
        singLimit += maxSingLineNo;

        for (; (currentLineSungNo < singLimit) && (currentLineSungNo < noOfLyricLines); currentLineSungNo++)
        {

            // $$ lock the mutex

            if (previousNoId != *singersNoIdPtr) // for dipslay readabilty
                cout << endl;

            //$$ display singer no and lyric line

            // $$ unlock the mutex

            //$$ update previousNoId and increment totalLineCount;
        }

        //$$ get up time slice for 1 second
    }

    // $$ lock the mutex

    //$$ display done   thread with singersNoIdPtr

    // $$ lock the mutex

    //$$ exit the pthread
}

int main(int argc, char *argv[])
{

    try
    {

        if (argc != 3)
        {
            //$$string errorStr = "Usage : ";
        }

        string lyricsFilenameStr(argv[1]);
        unsigned noOfSingers = stoi(argv[2]);
        vector<string> lyricLinesVector;
        unsigned noOfLyricLines = 0;

        GetLyricLinesFromFileToVector(lyricsFilenameStr, lyricLinesVector, noOfLyricLines);

        cout << endl
             << endl;
        //$$
        cout << endl
             << endl;

        pthread_t *singersThreadIdPtr;
        unsigned *singersNoIdPtr;
        SingLinesThreadInfoStruct *singLinesThreadInfoStructPtr;

        singersThreadIdPtr = new pthread_t[noOfSingers];
        singersNoIdPtr = new unsigned[noOfSingers];
        singLinesThreadInfoStructPtr = new SingLinesThreadInfoStruct[noOfSingers];

        //$$ initialize the pthread mutex lock

        // Create independent threads each of which will execute the pthread function
        for (unsigned singerNo = 0; singerNo < noOfSingers; ++singerNo)
        {

            unsigned *singerNoIdPtr = new unsigned;
            *singerNoIdPtr = singerNo + 1;

            //$$ Fill the singLinesThreadInfoStructPtr[singerNo] with the corresponding variable values

            // $$ lock the mutex

            // display create thread with *singerNoIdPtr

            // $$ unlock the mutex

            //$$ create the thread using function SingLinesThread

        } // for

        // Wait until all threads have completed before main continues.
        // If main doesn't wait, main may continue to an exit which will terminate
        // the main process. When main terminates, all associated threads will also
        // terminate before the threads have completed.

        //$$ join to all the running threads

        cout << endl;
        cout << "Program done " << endl;
        cout << "Total Line count sung : " << totalLineCount << endl;
        cout << endl;

        //$$ pthread_mutex_destroy
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
        exit(EXIT_FAILURE);
    } // catch

    exit(EXIT_SUCCESS);
}