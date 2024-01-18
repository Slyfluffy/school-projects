/*
** server.c -- a stream socket server demo
**
** This is a modified of code from http://beej.us/guide/bgnet/
** The code was modified to take out the "fork" to start a new process and
** the signal handler.
**
** It was modified to partly conform to BYU-Idaho style.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
//  #include <signal.h>

using namespace std;

#define BACKLOG 10   // how many pending connections queue will hold
#define MAXDATASIZE 100 // max number of bytes we can get at once

/******************************************
 * RPSSERVER :: VALIDATEPLAYERCHOICES
 * INPUTS  :: socket, buf, playerNumber
 * OUTPUTS :: NONE
 * validates the choice sent by the clients
 *****************************************/
char validatePlayerChoices(int socket, char buf[MAXDATASIZE], int playerNumber) {
   read(socket, buf, MAXDATASIZE);
   
   // Any of the valid options?
   if (buf[0] == 'r' || buf[0] == 'p' || buf[0] == 's' || buf[0] == 'q')
      printf("Player %d's choice is OK.\n", playerNumber);
   else {
      printf("Player %d's choice is invalid.\n", playerNumber);
      buf[0] = 'e';
   }
   
   return buf[0];
}

/******************************************
 * RPSSERVER :: RESULTS STRUCTURE
 * Easy way to store the results of the two
 * players.
 *****************************************/
struct Results {
   char player1;
   char player2;
};

/******************************************
 * RPSSERVER :: COMPARESELECTIONS
 * INPUTS  :: player1, player2 (char)
 * OUTPUTS :: r
 * compares the selections and returns the
 * correct results of the game.
 *****************************************/
Results compareSelections(char player1, char player2) {
   Results r;
   
   // First check for any errors
   if (player1 == 'e' || player2 == 'e') {
      r.player1 = 'e';
      r.player2 = 'e';
   }
   
   // Did anyone quit?
   else if (player1 == 'q' || player2 == 'q') {
      r.player1 = 'q';
      r.player2 = 'q';
   }
   
   // Tie Scenario?
   else if (player1 == player2) {
      r.player1 = 't';
      r.player2 = 't';
   }
   
   // Check Player1 if they chose r, p, or s
   // All are in the context of player 1
   else if (player1 == 'r') { // R's turn
      if (player2 == 's') {   // Win
         r.player1 = 'w';
         r.player2 = 'l';
      } else {                // Lose
         r.player1 = 'l';
         r.player2 = 'w';
      }
   }
   else if (player1 == 'p') { // P's turn
      if (player2 == 'r') {   // win
         r.player1 = 'w';
         r.player2 = 'l';
      } else {                // Lose
         r.player1 = 'l';
         r.player2 = 'w';
      }
   }
   else {                    // S's turn
      if (player2 == 'p') {  // Win
         r.player1 = 'w';
         r.player2 = 'l';
      } else {               // Lose
         r.player1 = 'l';
         r.player2 = 'w';
      }
   }
   
   return r;
}

/**************************************************
 * RPSSERVER :: SENDRESULTS
 * INPUTS  :: player1FD, player2FD, player1Result
 * OUTPUTS :: NONE
 * Sends the results according to player1's results
 *************************************************/
void sendResults(int player1FD, int player2FD, char player1Result) {
   if (player1Result == 't') {
      send(player1FD, "t", strlen("t"), 0);
      send(player2FD, "t", strlen("t"), 0);
   }
   else if (player1Result == 'w') {
      send(player1FD, "w", strlen("w"), 0);
      send(player2FD, "l", strlen("l"), 0);
   }
   else if (player1Result == 'l') {
      send(player1FD, "l", strlen("l"), 0);
      send(player2FD, "w", strlen("w"), 0);
   }
   else if (player1Result == 'q') {
      send(player1FD, "q", strlen("q"), 0);
      send(player2FD, "q", strlen("q"), 0);
   }
   else {
      send(player1FD, "er", strlen("er"), 0);
      send(player2FD, "er", strlen("er"), 0);
   }
}

/*************************************
 * RPSSERVER :: GET_IN_ADDR
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

/******************************
 * RPSSERVER :: MAIN
 * INPUTS  :: argc, argv
 * OUTPUTS :: NONE
 * Runs the rps server program.
 *****************************/
int main(int argc, char * argv[])
{
   char * port = NULL;
   if (argc < 2) {
      printf("ERROR: Please enter a port number.\n");
      exit(1);
   }
   else
      port = argv[1];
   
   // Lines 182-253 were from source code.
   int sockfd = NULL;  // listen on sock_fd
   int player1FD;      // new connection on player1
   int player2FD;
   
   char buffer[MAXDATASIZE];
   struct addrinfo hints;
   struct addrinfo *servinfo;
   struct addrinfo *p;
   
   struct sockaddr_storage player1Addr; // connector's address information
   struct sockaddr_storage player2Addr; // connector's address information
   
   socklen_t sinSize1;
   socklen_t sinSize2;
   
   //struct sigaction sa;
   int yes=1;
   char s[INET6_ADDRSTRLEN];
   int rv;
   
   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE; // use my IP

   if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
   {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
   }

   // loop through all the results and bind to the first we can
   for(p = servinfo; p != NULL; p = p->ai_next)
   {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1)
      {
         perror("server: socket");
         continue;
      }

      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
            sizeof(int)) == -1)
      {
         perror("setsockopt");
         exit(1);
      }

      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
      {
         close(sockfd);
         perror("server: bind");
         continue;
      }

      break;
   }

   freeaddrinfo(servinfo); // all done with this structure

   if (p == NULL)
   {
      fprintf(stderr, "server: failed to bind\n");
      exit(1);
   }

   if (listen(sockfd, BACKLOG) == -1)
   {
      perror("listen");
      exit(1);
   }

   printf("server: waiting for connections...\n");

   sinSize1 = sizeof player1Addr;
   player1FD = accept(sockfd, (struct sockaddr *)&player1Addr, &sinSize1);
   if (player1FD == -1)
      perror("accept");

   inet_ntop(player1Addr.ss_family, get_in_addr((struct sockaddr *)&player1Addr),
             s, sizeof s);
   printf("server: got connection from player 1\n");
   
   send(player1FD, "wait", strlen("wait"), 0);
      
   sinSize2 = sizeof player2Addr;
   player2FD = accept(sockfd, (struct sockaddr *)&player2Addr, &sinSize2);
   
   if (player2FD == -1)
      perror("accept");

   inet_ntop(player2Addr.ss_family, get_in_addr((struct sockaddr *)&player2Addr),
             s, sizeof s);
   printf("server: got connection player 2\n");

   send(player1FD, "ready", strlen("ready"), 0);
   send(player2FD, "ready", strlen("ready"), 0);
   printf("server: sent player's ready message\n");
   
   bool done = false;
   
   // Go until a player quits
   while (!done) {
      char player1Choice = validatePlayerChoices(player1FD, buffer, 1);
      char player2Choice = validatePlayerChoices(player2FD, buffer, 2);
      printf("Validated Player choices.\n");
      
      Results r = compareSelections(player1Choice, player2Choice);
      char player1Result = r.player1;
      
      sendResults(player1FD, player2FD, player1Result);
      
      if (r.player1 == 'q' || r.player2 == 'e')
         done = true;
   }
   
   close(player1FD);
   close(player2FD);
   close(sockfd);

   return 0;
}

