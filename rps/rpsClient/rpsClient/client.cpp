/*
** client.c -- a stream socket client demo
**
** This is code from http://beej.us/guide/bgnet/
** It was modified to partly conform to BYU-Idaho style.
*/

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

#define MAXDATASIZE 100 // max number of bytes we can get at once

/**********************************
 * RPSCLIENT :: GETPLAYERCHOICE
 * INPUTS  :: NONE
 * OUTPUTS :: choice
 * Validates and returns the 
 * choice (r,p,s,q) from the player
 *********************************/
char getPlayerChoice() {
   while (1) {
      char choice;
      cout << "Enter choice: ";
      cin >> choice;
      
      // If it is any of the valid choices!
      if (choice == 'r' || choice == 'p' || choice == 's' || choice == 'q') {
         if (choice != 'q') // If they didn't choose quit option
            cout << "You have selected " << choice << "." << endl;
         else
            cout << "Leaving game..." << endl;
         
         return choice;
      }
      else
         cout << "Invalid selection. Please try again.\n";
   }
}

/*********************************
 * RPSCLIENT :: SENDPLAYERCHOICE
 * INPUTS  :: socket, choice
 * OUTPUTS :: NONE
 * sends the choice to the server.
 ********************************/
void sendPlayerChoice(int socket, char choice) {
   if (choice == 'r')
      send(socket, "r", strlen("r"), 0);
   else if (choice == 'p')
      send(socket, "p", strlen("p"), 0);
   else if (choice == 's')
      send(socket, "s", strlen("s"), 0);
   else if (choice == 'q')
      send(socket, "q", strlen("q"), 0);
   else
      send(socket, "er", strlen("er"), 0);
}

/***********************************
 * RPSCLIENT :: DISPLAYRESULTS
 * INPUTS  :: results
 * OUTPUTS :: NONE
 * displays the results to the user.
 **********************************/
void displayResults(char results) {
   if (results == 't')
      cout << "It was a tie!\n";
   else if (results == 'w')
      cout << "You won the round!\n";
   else if (results == 'l')
      cout << "You lost the round!\n";
}

/*************************************
 * RPSCLIENT :: GET_IN_ADDR
 * INPUTS  :: results
 * OUTPUTS :: NONE
 * gets the socket address in IPv4 
 * or IPv6. Provided from source code.
 ************************************/
void *get_in_addr(struct sockaddr *sa)
{
   if (sa->sa_family == AF_INET) {
      return &(((struct sockaddr_in*)sa)->sin_addr);
   }

   return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/***********************************
 * RPSCLIENT :: RUNGAME
 * INPUTS  :: socket
 * OUTPUTS :: NONE
 * runs the client side game portion
 **********************************/
void runGame(int socket) {
   bool done = false;
   
   // Go until somebody quits or an error occurs.
   while (!done) {
      char choice = getPlayerChoice();
      cout << choice << endl;
      sendPlayerChoice(socket, choice);
      
      // Read the results
      char results[MAXDATASIZE];
      read(socket, results, MAXDATASIZE);
      
      // As long as it isn't a quit or an error
      if (results[0] != 'q' && results[0] != 'e')
         displayResults(results[0]);
      // Someone quit!
      else if (results[0] == 'q') {
         done = true;
         if (results[0] != 'q')
            cout << "A player has quit. leaving game...\n";
      }
      // Error!
      else {
         done = true;
         cout << "An error has occured somewhere. Shutting down game.\n";
      }
   }
}

/***********************
 * RPSCLIENT :: MAIN
 * INPUTS  :: argc, argv
 * OUTPUTS :: NONE
 * Runs the program
 **********************/
int main(int argc, char *argv[])
{
   char * address = NULL;
   char * port = NULL;
   
   if (argc < 2) {
      fprintf(stderr,"ERROR: Please enter host address and port number\n");
      exit(1);
   }
   else if (argc < 3) {
      fprintf(stderr, "ERROR: Please enter host port number\n");
      exit(1);
   }
   else {
      address = argv[1];
      port = argv[2];
   }
   
   // Line 163 to 205 was given in source code.
   int sockfd = NULL;
   char buf[MAXDATASIZE];
   struct addrinfo hints;
   struct addrinfo *servinfo;
   struct addrinfo *p;
   int rv;
   char s[INET6_ADDRSTRLEN];

   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;

   if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
   }

   // loop through all the results and connect to the first we can
   for (p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {
         perror("client: socket");
         continue;
      }

      if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
         perror("client: connect");
         close(sockfd);
         continue;
      }

      break;
   }

   if (p == NULL) {
      fprintf(stderr, "Failed to connect!\n");
      return 2;
   }

   inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
         s, sizeof s);

   freeaddrinfo(servinfo); // all done with this structure
   
   // Code from here onwards was added to make the RPS client
   cout << "\tRock, Paper, Scissors\n"
        << "Options:\n\trock: r\n\tpaper: p\n\tscissors: s\n\tquit: q\n";
   
   string playerAvailable = "wait";
   while (playerAvailable != "ready") {
      read(sockfd, buf, MAXDATASIZE);
      playerAvailable = buf;
      
      if (playerAvailable != "ready")
         cout << "\nWaiting for another player to connect.\n";
      else
         cout << "\nPlayer connected. Let the game begin!\n";
   }

   runGame(sockfd);
   close(sockfd);

   return 0;
}
