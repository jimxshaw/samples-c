#include <string>
#include <iostream>
#include <sstream>

#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>

#include "LineInfo.h"

using namespace std;

const int    MESSAGE_MAX_SIZE = 1024;
const int    ERROR     = -1;
const int    SOCK_PORT = 8888;
const string LOCAL_HOST_ADDRESS = "127.0.0.1";

const int MAX_GUESS_LIMIT = 100;
const int MAX_NO_TRIES    =  12;

int main(int argc, char* argv[]) {

    int          socketServerId;
    sockaddr_in  server;
    string       messageStr;

  try {

    //Create socketServerId
    //$$

    server.sin_addr.s_addr = inet_addr(LOCAL_HOST_ADDRESS.c_str());
    server.sin_family      = AF_INET;
    server.sin_port        = htons(SOCK_PORT);

    //Connect to remote server socketServerId
    //$$

    // Prompt for a guess number in staed range

    int nooftries = 0;
    int guessno;

    // start game
    do {
        // prompt for guessno
        //$$

        stringstream guessnoss; 
        guessnoss << guessno;
        string guessNoStr = guessnoss.str();

        // write to socketServerId guessNoStr
        //$$
        
        // read from socketServerId the receiveMessage
        char receiveMessage[MESSAGE_MAX_SIZE]={0};
        //$$

        string receiveMessageStr = receiveMessage;
        cout << endl << "Server reply :" << endl << endl << receiveMessageStr << endl << endl;

        // Check if a Win Message
        if (receiveMessageStr.find("Win") != string::npos)
            break;

        // Processs if equal to max numbere of tries
        if (nooftries == MAX_NO_TRIES){
            // display done for max no of tries
            // write socketServerId Sned Random Number
            //$$
            
            // read from socketServerId randomNoMessage
            char randomNoMessage[MESSAGE_MAX_SIZE] = {0};
            //$$

            int randomno = stoi(randomNoMessage);
            
            //display the random no and state the game is over 
            //$$
            
            //leave the loop
            //$$
        }

    } while (true);

    if (close(socketServerId) == ERROR)
        throw domain_error(LineInfo("Socket close failure ", __FILE__, __LINE__));
  }
  //try
  catch (exception& e) {
      cout << e.what() << endl;
      exit(EXIT_FAILURE);
  }//catch

  exit(EXIT_SUCCESS);
        
}