#include "main.h"

#include "linkedList.h"
#include "floor.h"

/**
 * @brief SIGINT handler.
 *
 * Sets the global running bool to false to initiate server shutdown.
 *
 * @param signum The signal integer.
 */
void signalHandler(int signum)
{
    std::cout << "\nCaught signal " << signum << ". Shutting down server..." << std::endl;
    running.store(false);
}

/**
 * @brief Client handler.
 *
 * Handles a client on a separate thread, listening on client socket
 * for data sent from client and processes the client's response.
 *
 * @param client_fd The client's file descriptor as an integer.
 *
 * @note The client handler does and should not handle more logic than
 * simply receiving the request and replying with a response.
 */
void handleClient(int client_fd, std::shared_ptr<Floor> floor)
{
    char order[1024];

    // Start listening for data sent from the client
    while (running)
    {
        // Set the poll to listen to both write and reads on client_fd
        pollfd clientSocketFD = {};
        clientSocketFD.fd = client_fd;
        clientSocketFD.events = POLLRDNORM | POLLWRNORM;
        clientSocketFD.revents = 0; // Set the response event as the 0 mask

        int activity = poll(&clientSocketFD, 1, 1);

        // Check for error
        if (activity < 0)
        {
            std::cerr << "Failed to poll() from client" << std::endl;
            break;
        }

        // Check to see if ready to read or write
        if (activity > 0)
        {
            if (clientSocketFD.revents & POLLRDNORM) // Check that socket is ready to read
            {
                int valread = read(client_fd, order, sizeof(order) - 1);
                if (valread <= 0)
                {
                    std::cerr << "Read from client failed or connection closed" << std::endl;
                    break;
                }

                // Null-terminate the buffer based on bytes read
                order[valread] = '\0';

                // Print out receival in server log
                std::cout << "Received: " << order << std::endl;

                std::string response = floor->process(order);

                if (clientSocketFD.revents & POLLWRNORM) // Check that socket is ready to write
                {
                    int valsent = send(client_fd, response.c_str(), response.size(), 0);
                    if (valsent <= 0)
                    {
                        std::cerr << "Failed to send feedback to client" << std::endl;
                    }
                }
            }
        }
        // Poll timed out, repeat poll
    }

    std::cout << "Served closed, closing client..." << std::endl;
    close(client_fd);
}

int main()
{
    // Instantiate a thread pool
    std::vector<std::thread> client_threads;

    signal(SIGINT, signalHandler);

    // Create a main socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Instantialize the server address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Binding failed" << std::endl;
        close(server_fd);
        return -1;
    }

    // Listen for any connections
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return -1;
    }

    // TODO: Maybe use weak_ptr and unique_ptr?
    std::shared_ptr<Floor> floor = std::make_shared<Floor>();

    // Start accepting connections via poll
    while (running)
    {
        // Set the poll to listen to server_fd and perform reads
        pollfd listeningSocketFD = {};
        listeningSocketFD.fd = server_fd;
        listeningSocketFD.events = POLLRDNORM;
        listeningSocketFD.revents = 0; // Set response event as 0 mask

        int activity = poll(&listeningSocketFD, 1, 1);

        // Check for error
        if (activity < 0)
        {
            std::cerr << "Failed to poll()" << std::endl;
            break;
        }

        // Check to see if ready to read
        if (activity > 0)
        {
            int client_fd = accept(server_fd, nullptr, nullptr);
            if (client_fd >= 0)
            {
                // Handle the new client connection in a different thread
                client_threads.emplace_back(std::thread(handleClient, client_fd, floor));
                std::cout << "Accepted new client\n";
            }
        }
        // Poll timed out, restart poll
    }
    std::cout << "Running is false, cleaning up now." << std::endl;

    // Wait for all threads to finish
    for (auto &thread : client_threads)
    {
        if (thread.joinable())
        {
            thread.join(); // Wait for each thread to finish
        }
    }

    close(server_fd);
    std::cout << "Server shut down gracefully." << std::endl;
    return 0;
}