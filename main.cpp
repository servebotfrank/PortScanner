

#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Network.hpp>
using std::cout;
using std::endl;


//port_is_open
//Checks if the port is open at the address.
//TcpSocket: SFML function, connects to remote socket.
//Connects to the address and port through TcpSocket() and then returns true or false depending on whether the connection succeeded.
//To check connections on the local computer, just input "localhost" as the string address.
static bool port_is_open(const std::string& address, int port)
{
    return (sf::TcpSocket().connect(address, port) == sf::Socket::Done);
}


//Split
//Splits a string into tokens around a delimiter (White Space)
//Empty tokens accepted
static std::vector<std::string> split(const std::string& string, char delimiter= ' ', bool allow_empty = false)
{
    std::vector<std::string> tokens;
    std::stringstream sstream(string);
    std::string token;
    while(std::getline(sstream, token, delimiter))
    {
        if(allow_empty ||  token.size() > 0)
        {
            tokens.push_back(token);
        }
    }
    return tokens;
    
}
//string_to_int
//Converts string into an integer
static int string_to_int(const std::string& string)
{
    std::stringstream sstream(string);
    int i;
    sstream >> i;
    return i;
}
//Swap
//Swaps two values of type T.
template <typename T>
static void swap(T& a, T& b)
{
    T c = a;
    a = b;
    b = c;
}
//range
//Generates a vector that contains a range of values.
template <typename T>
static std::vector<T> range(T min, T max)
{
    if (min > max)
        swap(min, max);
    if (min == max)
        return std::vector<T>(1, min);
    std::vector<T> values;
    for (; min <= max; ++min)
        values.push_back(min);
    return values;
}
//parse_ports_list
//Parses the list of ports of ports using various functions.
//Preconditions:
//Must have range, swap, and string_to_int defined.
//Requests port should be a valid port.
//Postconditions:
//Returns a vector of ints containing the list of ports.
static std::vector<int> parse_ports_list(const std::string& list)
{
    std::vector<int> ports;
    for (const std::string& token : split(list, ',')) {
        std::vector<std::string> strrange = split(token, '-');
        switch (strrange.size()) {
                //One value(add to ports)
            case 0: ports.push_back(string_to_int(token));       break;
            case 1: ports.push_back(string_to_int(strrange[0])); break;
                //Two values (range-items within range)
            case 2:
            {
                int min = string_to_int(strrange[0]),
                max = string_to_int(strrange[1]);
                for (int port : range(min, max))
                    ports.push_back(port);
                break;
            }
            default:
                break;
        }
    }
    return ports;
}



int main()
{
    std::string address;
    std::string port_list;
    std::vector<int> ports;
    std::cout << "Address: " << std::flush;
    std::getline(std::cin, address);
    std::cout << "Port: " << std::flush;
    std::getline(std::cin, port_list);
    ports = parse_ports_list(port_list);
    std::cout << "Scanning " << address << "...\n";
    

        //Commented code includes ALL ports, open or closed.
        //Uncommented code only shows open ports.
//    for (int port : ports) {
//        std::cout << "Port " << port << " : ";
//        if (port_is_open(address, port))
//            std::cout << "OPEN\n";
//        else
//            std::cout << "CLOSED\n";
//    }
    for (int port : ports) {
        if (port_is_open(address, port))
            std::cout << "Port " << port << " : OPEN\n";
    }
    std::cout << std::flush;
    return 0;

}
