#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>

#include <unistd.h>
#include <pthread.h>

#include "LineInfo.h"

using namespace std;

pthread_mutex_t myLock;
static unsigned totalLineCount;

void GetLyricLinesFromFileToVector(string lyricsFilenameStr,
                                   vector<string> &lyricLinesVector,
                                   unsigned &noOfLyricLines)
{
    ifstream fileLyricsStream(lyricsFilenameStr);

    if (fileLyricsStream.fail())
    {
        throw domain_error(LineInfo("File Open Failure", __FILE__, __LINE__));
    }

    string line;

    while (getline(fileLyricsStream, line))
    {
        lyricLinesVector.push_back(line);
    }

    noOfLyricLines = lyricLinesVector.size();
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
    if ((pthread_mutex_lock(&myLock)) != 0)
    {
        throw domain_error(LineInfo("pthread_mutex_lock", __FILE__, __LINE__));
    }

    //$$ display start thread message
    cout << "start thread" << *singersNoIdPtr << endl;

    // $$ unlock the mutex
    if ((pthread_mutex_unlock(&myLock)) != 0)
    {
        throw domain_error(LineInfo("pthread_mutex_unlock", __FILE__, __LINE__));
    }

    // sing up to 10 lines at a time
    unsigned singLimit = 0;

    for (unsigned lyricLineNo = 0; lyricLineNo < noOfLyricLines; lyricLineNo += maxSingLineNo)
    {

        unsigned currentLineSungNo = lyricLineNo;
        singLimit += maxSingLineNo;

        for (; (currentLineSungNo < singLimit) && (currentLineSungNo < noOfLyricLines); currentLineSungNo++)
        {

            // $$ lock the mutex
            if ((pthread_mutex_lock(&myLock)) != 0)
            {
                throw domain_error(LineInfo("pthread_mutex_lock", __FILE__, __LINE__));
            }

            if (previousNoId != *singersNoIdPtr)
            {
                cout << endl; // for display readabilty
            }

            //$$ display singer no and lyric line
            cout << setw(3) << *singersNoIdPtr << " : " << setw(3) << currentLineSungNo
                 << " > " << lyricLinesVector[currentLineSungNo] << endl;

            // $$ unlock the mutex
            if ((pthread_mutex_unlock(&myLock)) != 0)
            {
                throw domain_error(LineInfo("pthread_mutex_unlock", __FILE__, __LINE__));
            }

            //$$ update previousNoId and increment totalLineCount;
            previousNoId = *singersNoIdPtr;
            ++totalLineCount;
        }

        //$$ get up time slice for 1 second
        sleep(1);
    }

    // $$ lock the mutex
    if ((pthread_mutex_lock(&myLock)) != 0)
    {
        throw domain_error(LineInfo("pthread_mutex_lock", __FILE__, __LINE__));
    }

    //$$ display done   thread with singersNoIdPtr
    cout << "done thread" << *singersNoIdPtr << endl;

    // $$ unlock the mutex
    if ((pthread_mutex_unlock(&myLock)) != 0)
    {
        throw domain_error(LineInfo("pthread_mutex_unlock", __FILE__, __LINE__));
    }

    //$$ exit the pthread
    pthread_exit(NULL);
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