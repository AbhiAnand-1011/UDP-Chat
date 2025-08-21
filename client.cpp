#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
using namespace std;

#define PORT 8080
#define MAX_LEN 1024

void receive_messages(int sockfd) {
    char buffer[MAX_LEN];
    while (true) {
        int n = recv(sockfd, buffer, MAX_LEN, 0);
        if (n > 0) {
            buffer[n] = '\0';
            cout << "\n" << buffer << endl;
            cout << "You: ";
            cout.flush();
        }
    }
}

int main() {
    int sockfd;
    sockaddr_in servaddr{};

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    string server_ip;
    cout << "Enter Server IP (127.0.0.1 for local): ";
    getline(cin, server_ip);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, server_ip.c_str(), &servaddr.sin_addr) <= 0) {
        cerr << "Invalid server IP" << endl;
        return 1;
    }

    if (connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
        return 1;
    }

    cout << "Enter your username: ";
    string username;
    getline(cin, username);

    // Register username with server
    send(sockfd, username.c_str(), username.size(), 0);

    thread recv_thread(receive_messages, sockfd);
    recv_thread.detach();

    cout << "Connected to chat. Type messages below.\n";

    string msg;
    while (true) {
        cout << "You: ";
        getline(cin, msg);
        send(sockfd, msg.c_str(), msg.size(), 0);
    }

    close(sockfd);
    return 0;
}
