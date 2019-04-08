#include "DV_router.cpp"
#include "DataSegment.cpp"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 8192 // This is the udp receive buffer size

void trafficGenerator();

// usage: ./my-router [routerID]
int main(int argc, char* argv[]) {

    if (argc != 2){
        std::cout << "Bad function call \nUsage: ./my-router [routerID] " << std::endl;
        return 0;
    }

    char routerID = *argv[1];

    // If routerID = 'H' then this router is the traffic generator
    // It will send one data packet and then exit. It will not participate in DV routing.
    if(routerID == 'H'){
        trafficGenerator();
        return 0;
    }


    DVRouter router(routerID); // create DV router object
    time_t start = time(0); // time = 0 when router is created
    double seconds_since_start = 0;
    cout << "Router " << routerID << " is running" << endl;
    cout << "Router " << routerID << "'s Initial Routing Table:\n";
    router.PrintRoutingTable(seconds_since_start, NULL, '\0'); // print out the initial routing table

    int portNumber = router.queryPort(routerID);
    string host = "localhost"; // all routers run on localhost i.e. 127.0.0.1
    

    try {
        // Create io service
        boost::asio::io_service io_service;
        
        // create a UDP socket object on portNumber
        boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), portNumber));

        // distance vector routing
        int pid = fork(); // spawn a new process
        if (pid < 0){
            perror("fork failed");
            return 0;
        }

        // This is child process - send DV advertisementsto each neighbour periodically
        else if (pid==0){ 
            while(1){
                string destPort;
                for (int i=0; i<6; i++){
                    // if the routers are direct neighbours
                    if(router.neighbour_costs[i] != 99 && router.neighbour_costs[i] != 0){
                        destPort = to_string(router.queryPort( i + 'A'));
                        // resolver object to find the correct remote endpoint to use based on the host and service names
                        boost::asio::ip::udp::resolver resolver(io_service);
                        // Make query to the UDP (ipv4) host server
                        boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), host, destPort);
                        // Will return at least one endpoint in the list if it does not fail.
                        // This means it is safe to dereference the return value directly.
                        boost::asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(query);
                        // Using datagram socket, not stream socket.
                        boost::asio::ip::udp::socket socket(io_service);

                        // initiate contact with the remote endpoint
                        socket.open(boost::asio::ip::udp::v4());
                        router.ReadCosts();
                        string message = router.ConstructMessage();

                        // create data segment object
                        DataSegment packetOut('C', portNumber, stoi(destPort), message.length(), message);
                        // construct a string of data segment and send over udp
                        socket.send_to(boost::asio::buffer(packetOut.constructDataSegment()), receiver_endpoint);
                    }
                }
                sleep(5); // wait 5 seconds
            }
        }
        // This is parent process - listen for advertisements
        else { 
            while(1) { // forever
                boost::array<char, BUFFER_SIZE> recv_buf;
                // The remote_endpoint object will be populated by boost::asio::ip::udp::socket::receive_from().
                boost::asio::ip::udp::endpoint remote_endpoint;
                // Receive message
                boost::system::error_code error;
                size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);
                if (len > 0){
                    // create a data segment object from data of udp message
                    DataSegment packetIn(recv_buf.data());
                    // Check if the packet is a control message
                    if (packetIn.getStatus() == 'C'){
                        int * neighbour_table = router.ParseMessage(packetIn.getPayload());
                        char neighbour;
                        // update neighbour to reflect what table we just recieved
                        for (int i=0; i<6; i++){
                            if (neighbour_table[i] == 0)
                                neighbour = i + 'A';
                        }
                        seconds_since_start = difftime( time(0), start);
                        if(router.BellmanFord(neighbour, neighbour_table)){ //Use the Bellman-Ford algorithm to update the routing table
                            router.PrintRoutingTable(seconds_since_start, neighbour_table, neighbour);
                            router.StoreRoutingTable(seconds_since_start, neighbour_table, neighbour);
                        }
                    }
                    // must be a data packet that we have to route through the network
                    else if (packetIn.getStatus() == 'D'){
                        int destPort = packetIn.getDestPort(); // what is the ultimate destination of the data packet
                        // if it is this router, job done!
                        if (destPort == portNumber){
                            packetIn.printDataSegment();
                        }
                        else { // otherwise forward it on to the next hop in shortest path
                            int destID = router.queryID(destPort);
                            int nextHop = router.getNextHopPort(destID);
                            cout << "Next Hop: " << nextHop << endl;

                            // forward on to next hop router
                            // resolver object to find the correct remote endpoint to use based on the host and service names
                            boost::asio::ip::udp::resolver resolver(io_service);
                            // Make query to the UDP (ipv4) host server
                            boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), host, to_string(nextHop));
                            boost::asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(query);
                            // Using datagram socket, not stream socket.
                            boost::asio::ip::udp::socket socket(io_service);
                            // initiate contact with the remote endpoint
                            socket.open(boost::asio::ip::udp::v4());
                            // construct a string of data segment and send over udp
                            socket.send_to(boost::asio::buffer(packetIn.constructDataSegment()), receiver_endpoint);
                        }
                        
                    }
                }
            }
        }
    }
    
    catch (exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}


void trafficGenerator(){

    string payload = "This is a data packet. Let's see if it can make it's way from router A to D";
    // create a data segment object with status = 'D' to denote a data packet
    int originPort = 10000; // A
    int destinationPort = 10003; // D

    DataSegment dataPacket('D', originPort, destinationPort, payload.length(), payload);
    // print the datapacket to console
    dataPacket.printDataSegment();
    int portNo = 10010;
    string host = "localhost"; // all routers run on localhost i.e. 127.0.0.1
    string initialPort = to_string(originPort); // start at router A on port 10000

    try{
        // Create io service
        boost::asio::io_service io_service;
        // create a UDP socket object on portNumber
        boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), portNo));
        // resolver object to find the correct remote endpoint to use based on the host and service names
        boost::asio::ip::udp::resolver resolver(io_service);
        // Make query to the UDP (ipv4) host server
        boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), host, initialPort);
        // Will return at least one endpoint in the list if it does not fail.
        // This means it is safe to dereference the return value directly.
        boost::asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(query);

        // construct a string of data segment and send over udp
        socket.send_to(boost::asio::buffer(dataPacket.constructDataSegment()), receiver_endpoint);
    }
    catch (exception &e) {
        cerr << "Exception in trafficGenerator(): " << e.what() << endl;
    }

    return;
}
