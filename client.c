#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#define BUF_SIZE 2048

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char greeting[BUF_SIZE];
    char reply[BUF_SIZE];
    char filename[256];

    // finds UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("[Error] No socket");
        exit(EXIT_FAILURE);
    }

    // connects to local host server (same machine)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(1500); 

    // greeting + filename
    printf("Write 'Greeting': ");
    fgets(greeting, sizeof(greeting), stdin);
    greeting[strcspn(greeting, "\n")] = 0;  // remove newline character

    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;  // remove newline character

    // concat to send to server
    snprintf(buffer, sizeof(buffer), "%s %s", greeting, filename);

    // send greeting and filename
    if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // if success, gets OK from server
    if (recvfrom(sockfd, reply, BUF_SIZE, 0, NULL, NULL) < 0) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }
    printf("Server: %s\n", reply);
// ------send file content to server's side-------
FILE *file = fopen(filename, "rb");
if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
}
while (1) {
    size_t bytes_read = fread(buffer, 1, BUF_SIZE, file);
    if (bytes_read <= 0) {
        break;
    }
    if (sendto(sockfd, buffer, bytes_read, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
}
// --------------file read and transfer finish-----
if (fclose(file)==0){

bool fileClosed = true; // flag

if (fileClosed) {
    
    strcpy(buffer, "Finish");
    if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    printf("Client: Finish\n");

    // receive WellDone response from server
    if (recvfrom(sockfd, reply, BUF_SIZE, 0, NULL, NULL) < 0) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: %s\n", reply);

    printf("[Server ended the connection]\n");
}

}else{
	perror("[Error] File did not closed properly");
	exit(EXIT_FAILURE);
}
close(sockfd);
return 0;
}
