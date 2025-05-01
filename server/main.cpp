#include <boost/asio.hpp>
#include <fstream>
#include <iostream>

using boost::asio::ip::tcp;

uint64_t swapEndian64(uint64_t num) {
    return ((num & 0x00000000000000FFULL) << 56) |
           ((num & 0x000000000000FF00ULL) << 40) |
           ((num & 0x0000000000FF0000ULL) << 24) |
           ((num & 0x00000000FF000000ULL) << 8)  |
           ((num & 0x000000FF00000000ULL) >> 8)  |
           ((num & 0x0000FF0000000000ULL) >> 24) |
           ((num & 0x00FF000000000000ULL) >> 40) |
           ((num & 0xFF00000000000000ULL) >> 56);
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 44447));
        std::cout << "Server listening on port 44447.\n";

        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "Client connected properly.\n";
        
        uint32_t filenameLength;
        boost::asio::read(socket, boost::asio::buffer(&filenameLength, sizeof(filenameLength)));
        filenameLength = ntohl(filenameLength);
        
        std::vector<char> filenameBuffer(filenameLength);
        boost::asio::read(socket, boost::asio::buffer(filenameBuffer));
        std::string filename(filenameBuffer.begin(), filenameBuffer.end());
        
        uint64_t fileSize;
        boost::asio::read(socket, boost::asio::buffer(&fileSize, sizeof(fileSize)));
        fileSize = swapEndian64(fileSize);
        
        std::ofstream outFile("received_" + filename, std::ios::binary);
        if (!outFile)
            throw std::runtime_error("Error while trying to open output file");

        std::vector<char> buffer(4096);
        uint64_t bytesReceived = 0;
        while (bytesReceived < fileSize) {
            size_t bytes = socket.read_some(boost::asio::buffer(buffer));
            outFile.write(buffer.data(), bytes);
            bytesReceived += bytes;
        }

        std::cout << "File is ready under new name \"received_" << filename << "\" in build folder\n";
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
    }

    return 0;
}
