#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

#define PORT 8080
#define MAX_LEN 1024

struct Client {
    sockaddr_in addr;
    string username;
};

bool same_endpoint(sockaddr_in &a, sockaddr_in &b) {
    return a.sin_addr.s_addr == b.sin_addr.s_addr && a.sin_port == b.sin_port;
}

int main() {
    int sockfd;
    char buffer[MAX_LEN];
    vector<Client> clients;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in servaddr{}, cliaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    cout << "[Server] UDP chat running on port " << PORT << endl;

    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, MAX_LEN, 0, (sockaddr *)&cliaddr, &len);
        if (n <= 0) continue;
        buffer[n] = '\0';
        string msg(buffer);

        // Check if client exists
        auto it = find_if(clients.begin(), clients.end(),
                          [&](Client &c) { return same_endpoint(c.addr, cliaddr); });

        if (it == clients.end()) {
            // First message = username
            clients.push_back({cliaddr, msg});
            cout << "[Server] New client joined: " << msg << endl;
            string join_msg = msg + " joined the chat";
            for (auto &c : clients) {
                sendto(sockfd, join_msg.c_str(), join_msg.size(), 0,
                       (sockaddr *)&c.addr, sizeof(c.addr));
            }
        } else {
            // Normal chat message
            string tagged_msg = it->username + ": " + msg;
            cout << tagged_msg << endl;
            for (auto &c : clients) {
                sendto(sockfd, tagged_msg.c_str(), tagged_msg.size(), 0,
                       (sockaddr *)&c.addr, sizeof(c.addr));
            }
        }
    }

    close(sockfd);
    return 0;
}
