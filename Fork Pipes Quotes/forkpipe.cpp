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

#include <LineInfo.h>

using namespace std;


