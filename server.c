#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#define BUF_SIZE 1024

int main() {
    int server;
    int portNum = 1500;
    int bufsize = BUF_SIZE;
    char buffer[BUF_SIZE];
    //connects to IPv4 Internet domain socket addr
    struct sockaddr_in server_addr, cli_addr;
    // server info:- IPv4 ( internet protocol ver4) and uses UDP socket aka default (0)
    server = socket(AF_INET, SOCK_DGRAM, 0);
    //SOCKET SEARCHING !
    if (server < 0) {
        printf("[Error] Unable to find  socket.\n");
        exit(1);
    }

    printf("[Success] Socket server found :) \n");
    //htons() & hton1() for correct data interpretation 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(portNum);
    //BINDING PROCESS !
    if (bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("[Error] Binding failed.\n");
        return -1;
    }

    printf("[Success] Server socket binding done :) \n");
    socklen_t clilen = sizeof(cli_addr);
    
    //accepting data while it lasts..
    while (1) {
        int bytes_received = recvfrom(server, buffer, bufsize, 0, (struct sockaddr*)&cli_addr, &clilen);
        if (bytes_received < 0) {
            printf("[Error] Receiving data from client failed.\n");
            continue;
        }

        buffer[bytes_received] = '\0';
        printf("Client: %s\n", buffer);
        
	//start with Greeting from client. say OK after it
        if (strncmp(buffer, "Greeting", 8) == 0) {
            printf("Server: OK\n");
            sendto(server, "OK", 3, 0, (struct sockaddr*)&cli_addr, clilen); //sendto() is UDP data sending technique

            // Receive file content from client and save it
            FILE *file = fopen("copy_client.txt", "wb");
            if (file == NULL) {
                perror("[Error] Unable to create file");
                break;
            }

            int doneTransfer = 0; // Flag to track whether Finish has been received

            while (!doneTransfer) {
                bytes_received = recvfrom(server, buffer, bufsize, 0, (struct sockaddr*)&cli_addr, &clilen);
                if (bytes_received <= 0) {
                    printf("=> [Error] Unable to receive data from client.\n");
                    break;
                } else if (strncmp(buffer, "Finish", 6) == 0) { //client should send Finish after sending file
                    doneTransfer = 1; // Set the flag indicating Finish has been received
                    printf("Client: Finish\n");
                } else {
                    fwrite(buffer, 1, bytes_received, file);
                }
            }

            fclose(file);

            // Respond to the client
            if (doneTransfer) {
                printf("Server: WellDone\n");
                sendto(server, "WellDone", 9, 0, (struct sockaddr*)&cli_addr, clilen);
                printf("[Connection ended]\n");
            } else {
                // Handle the case was not received
                printf("=> [Error] File unable to be reach Server.\n");
            }
            close(server);
            return 0; // Exit the loop and end the server program
        }
    }
}
