#############################################################################
# Program:
#   Lab TCP_2client_server, Computer Networking
#   Brother Jones, CSE 354
# Author:
#   Journey Curtis
# Summary:
#   This is a simple 2 client server program. It will take turns from both
#   clients to save messages to respective files. These will always be new
#   files. The server shuts down once one of them says ".end"
##############################################################################
# Note Take 2: Took out ".txt" which saved them as text files. Also made it 
# able to overwrite the old ones. Couple comment edits
##############################################################################

from genericpath import exists
from sqlite3 import connect
import sys
from socket import *

# Default values for this program
END_CONNECTION = '.end'
SIZE = 1024 # Don't expect anything bigger than this

###########################################
# 2CLIENT_SERVER :: HANDLE_FILE_REQUEST
# INPUTS  :: socket, client_number
# OUTPUTS :: file (.end or file)
# Handles the file request from the client.
###########################################
def handle_file_request(socket, client_number):
   file_ok_message = 'ERROR'
   file = ""
   while file_ok_message == 'ERROR':
      filename = socket.recv(SIZE).decode('ascii')
      
      if filename == END_CONNECTION: # What if client sends ".end" right at the beginning?
         file_ok_message = 'FOK'
         file = filename
      else: # Normal procedure.
         file_ok_message = 'FOK'
         file = open(filename, "w")
         print("File name received from client" + str(client_number) + ": " + filename)

      # Send whatever message we have back to the client
      socket.send(file_ok_message.encode('ascii'))

   return file

#######################################################
# 2CLIENT_SERVER :: HANDLE_MESSAGES
# INPUTS  :: connection_socket1, connection_socket2, 
#            file1, file2
# OUTPUTS :: file1, connection_socket1, file2
#            connection_socket2, client_number (1 or 2)
# Handles the trading off between clients for messages.
# Calls handle_message_request to handle the actual
# messages.
#######################################################
def handle_messages(connection_socket1, connection_socket2, file1, file2):
   client_selector = 1 # Determine's which client's turn it is
   message_done = False
   client_1_done = False
   client_2_done = False

   while not message_done: 
      if client_selector == 1: # Client 1's turn
         if not client_1_done: # If client 1 has not said .end
            client_1_done = handle_message_request(file1, connection_socket1, 1)
         if not client_2_done: # Is client 2 still active?
            client_selector = 2

      elif client_selector == 2: # Client 2's turn
         if not client_2_done: # If client 2 has not said .end
            client_2_done = handle_message_request(file2, connection_socket2, 2)
         if not client_1_done: # Is client 1 still active?
            client_selector = 1

      if client_1_done and client_2_done: # Are both finished?
         message_done = True
         print("Server shutting down")

######################################################
# 2CLIENT_SERVER :: HANDLE_MESSAGE_REQUEST
# INPUTS  :: file, socket, client_number
# OUTPUTS :: False or True
# Handles the actual message request. It will save the
# message to the file or close the file.
# Returns a bool to tell handle_messages that client
# is done messaging.
######################################################
def handle_message_request(file, socket, client_number):
   message = socket.recv(SIZE).decode('ascii')
   print("Message from client" + str(client_number) + ": " + message)

   if message != END_CONNECTION: # As long as it isn't ".end"
      file.write(message + "\n")
      socket.send('OK'.encode('ascii'))
      return False # They aren't done
   else:
      file.close() # Don't forget to close the file1
      socket.send('OK'.encode('ascii'))
      return True # They are done

######################################################
# 2CLIENT_SERVER :: MAIN
# INPUTS  :: NONE
# OUTPUTS :: connection_socket1, connection_socket2,
#            client_number, file1, file2
# Main runs the server program. It calls the functions
# needed and passes the variables they require.
######################################################
server_port = int(sys.argv[1]) if len(sys.argv) == 2 else 0

if server_port != 0:
   # Initial setup
   server_socket = socket(AF_INET, SOCK_STREAM)
   server_socket.bind(('localhost', server_port))
   server_socket.listen(2)
   print("The Server is listening ...")

   try:
      print("Server waiting for two clients to connect ...")

      # First Client
      connection_socket1, addr1 = server_socket.accept()
      file1 = handle_file_request(connection_socket1, 1)

      print("Server waiting for another client to connect ...")
   
      # Second Client
      connection_socket2, addr2 = server_socket.accept()

      # What if the first client said .end for filename?
      if file1 != END_CONNECTION:
         file2 = handle_file_request(connection_socket2, 2)
      else:
         connection_socket2.send('Error'.encode('ascii'))

      # Handle the messages as long as one of them didn't say ".end" for filename
      if file1 != END_CONNECTION and file2 != END_CONNECTION:
         handle_messages(connection_socket1, connection_socket2, file1, file2)
      else:
         print("Server shutting down")

      # Closing down server
      print("Closing connections and exiting server")
      connection_socket1.close()
      connection_socket2.close()
      done = True

   except KeyboardInterrupt:
      print("\nClosing Server")
      server_socket.close()
else:
   print("ERROR: Please enter a port number in argument line")