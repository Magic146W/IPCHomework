#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: client <file_path>\n";
        return false;
    }

    std::string file_path = argv[1];
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << file_path << "\n";
        return false;
    }

    std::string filename = std::filesystem::path(file_path).filename().string();
    file.seekg(0, std::ios::end);
    uint64_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "44447");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);
        std::cout << "Connected to server properly.\n";
        
        uint32_t filenameSizeNetworkOrder = htonl(static_cast<uint32_t>(filename.size()));
        boost::asio::write(socket, boost::asio::buffer(&filenameSizeNetworkOrder, sizeof(filenameSizeNetworkOrder)));
        boost::asio::write(socket, boost::asio::buffer(filename));
        
        uint64_t fileSizeNetworkOrder = swapEndian64(file_size);
        boost::asio::write(socket, boost::asio::buffer(&fileSizeNetworkOrder, sizeof(fileSizeNetworkOrder)));
        
        std::vector<char> buffer(4096);
        while (file) {
            file.read(buffer.data(), buffer.size());
            std::streamsize bytes = file.gcount();
            if (bytes > 0) {
                boost::asio::write(socket, boost::asio::buffer(buffer.data(), bytes));
            }
        }

        std::cout << "File with name: " << filename << "\t sent.\n";
    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << "\n";
    }

    return 0;
}
