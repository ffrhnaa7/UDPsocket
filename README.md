## UDPsocket File Transfer
__This project on UDP Socket programming between a client and the server. The client interacts 
with the user, gets the user command and sends it to the server.__
NAME: 파하나

# Instruction: 
Your code must be written in C/C++ and must be able to work on Linux environment.
The sender program is tasked with sending a large file, while the receiver will receive the data and store in a file. (You can test your program with any file created by yourself)
Before sending a file, the sender should send texts of "Greeting" and the file name. When the receiver receives those texts, it must respond with "OK".
When receiving OK, the sender starts to send the file.
When all data is sent, the sender sends a text of "Finish". When the receiver receives that text, it should response with a text of "WellDone".

# Build & Run

Project building steps
1. Open folder -> cd UDPsocket
2. Compile using GNU Compiler Collection aka GCC 
3. Compile 'client' -> gcc -o client client.c
4. Compile 'server' -> gcc -o server server.c

Run steps
For SERVER : 
1) ./server

   __You don't have to do anything on the server side__
   It will automatically create a copy file of after receiving file from CLIENT named 'copy_client.txt'

For CLIENT :
1) ./client
2) When "Write 'Greeting': " pops up, you will have to write -> Greeting
3) When "Enter filename: " pops up, you will have to write -> client.txt

   __It will then send "Greeting client.txt" to server__

Then, connection ends after file transfer is done.
   
