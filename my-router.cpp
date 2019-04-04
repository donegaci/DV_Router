#include "DV_router.cpp"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 2000 // This is the udp receive buffer size

// usage: ./my-router [routerID]
int main(int argc, char* argv[]) {

    if (argc != 2){
        std::cout << "Bad function call \nUsage: ./my-router [routerID] " << std::endl;
        return 0;
    }

    char routerID = *argv[1];
    DVRouter router(routerID); // create DV router object
    time_t start = time(0); // time = 0 when router is created
    double seconds_since_start;
    cout << "Router " << routerID << " is running" << endl;
    cout << "Router " << routerID << "'s Initial Routing Table:\n";
    router.PrintRoutingTable(seconds_since_start, NULL, NULL); // print out the initial routing table

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
                        string message = router.ConstructMessage();
                        socket.send_to(boost::asio::buffer(message), receiver_endpoint);
                    }
                }
                sleep(5); // wait 5 seconds
            }
        }
        // This is parent process - listen for advertisements
        else { 
            //cout << "Router " << routerID << " is running" << endl;
            while(1) { // forever
                boost::array<char, BUFFER_SIZE> recv_buf;
                // The remote_endpoint object will be populated by boost::asio::ip::udp::socket::receive_from().
                boost::asio::ip::udp::endpoint remote_endpoint;
                // Receive message
                boost::system::error_code error;
                size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);
                //cout.write(recv_buf.data(), len) << endl;
                int * neighbour_table = router.ParseMessage(recv_buf.data());
                char neighbour;
                // update neighbour to reflect what table we just recieved
                for (int i=0; i<6; i++){
                    if (neighbour_table[i] == 0)
                        neighbour = i + 'A';
                }
                seconds_since_start = difftime( time(0), start);
                if(router.BellmanFord(neighbour, neighbour_table)){ //Use the Bellman-Ford algorithm to update the routing table
                    router.PrintRoutingTable(seconds_since_start, neighbour_table, neighbour);
                    //router.StoreRoutingTable(seconds_since_start, neighbour_table, neighbour);
                }
            }
        }
    }
    
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }



    // //create a DVRouter object for each router in the network
    // DVRouter A ('A');
    // DVRouter B ('B');
    // DVRouter C ('C');
    // DVRouter D ('D');
    // DVRouter E ('E');
    // DVRouter F ('F');
    
    // //print out the initial routing table for each router
    // cout << "--------------------INITIAL ROUTING TABLES--------------------\n";
    // A.PrintRoutingTable();
    // B.PrintRoutingTable();
    // C.PrintRoutingTable();
    // D.PrintRoutingTable();
    // E.PrintRoutingTable();
    // F.PrintRoutingTable();

    // //store the initial routing table for each router
    // A.StoreRoutingTable();
    // B.StoreRoutingTable();
    // C.StoreRoutingTable();
    // D.StoreRoutingTable();
    // E.StoreRoutingTable();
    // F.StoreRoutingTable();

    // //This simulates the an exchange of messages between each router in the network and its neighbours
    // //Each router will receive distance vectors (least cost arrays) from each of its direct neighbours and use the values in these arrays to find a new least cost path
    // //as more rounds of messages are exchanged, the routing table for each router will converge to a steady state
    // for(int i=0; i<3; i++){
    //     //A receives messages from its neighbours A and E
    //     A.BellmanFord('B', B.getCosts());
    //     A.BellmanFord('E', E.getCosts());

    //     //B receives distance vectors from A, C, E and F
    //     B.BellmanFord('A', A.getCosts());
    //     B.BellmanFord('C', C.getCosts());
    //     B.BellmanFord('E', E.getCosts());
    //     B.BellmanFord('F', F.getCosts());

    //     //C receives messages from its neighbours B, D and F
    //     C.BellmanFord('B', B.getCosts());
    //     C.BellmanFord('D', D.getCosts());
    //     C.BellmanFord('F', F.getCosts());

    //     //D receives messages from its neighbours C and F
    //     D.BellmanFord('C', C.getCosts());
    //     D.BellmanFord('F', F.getCosts());

    //     //E receives messages from its neighbours A, B and F
    //     E.BellmanFord('A', A.getCosts());
    //     E.BellmanFord('B', B.getCosts());
    //     E.BellmanFord('F', F.getCosts());

    //     //F receives messages from its neighbours B, C, D and E
    //     F.BellmanFord('B', B.getCosts());
    //     F.BellmanFord('C', C.getCosts());
    //     F.BellmanFord('D', D.getCosts());
    //     F.BellmanFord('E', E.getCosts());

    //     //print out the routing table for each router after each exchange of messages
    //     cout << "--------------------ROUTING TABLES AFTER EXCHANGE " << i+1 << "--------------------\n";
    //     A.PrintRoutingTable();
    //     B.PrintRoutingTable();
    //     C.PrintRoutingTable();
    //     D.PrintRoutingTable();
    //     E.PrintRoutingTable();
    //     F.PrintRoutingTable();

    //     //store the routing table for each router
    //     A.StoreRoutingTable();
    //     B.StoreRoutingTable();
    //     C.StoreRoutingTable();
    //     D.StoreRoutingTable();
    //     E.StoreRoutingTable();
    //     F.StoreRoutingTable();
    // }

}
