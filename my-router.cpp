#include "DV_router.cpp"

int main(){
    //create a DVRouter object for each router in the network
    DVRouter A ('A');
    DVRouter B ('B');
    DVRouter C ('C');
    DVRouter D ('D');
    DVRouter E ('E');
    DVRouter F ('F');
    
    //print out the initial routing table for each router
    cout << "--------------------INITIAL ROUTING TABLES--------------------\n";
    A.PrintRoutingTable();
    B.PrintRoutingTable();
    C.PrintRoutingTable();
    D.PrintRoutingTable();
    E.PrintRoutingTable();
    F.PrintRoutingTable();

    //store the initial routing table for each router
    A.StoreRoutingTable();
    B.StoreRoutingTable();
    C.StoreRoutingTable();
    D.StoreRoutingTable();
    E.StoreRoutingTable();
    F.StoreRoutingTable();

    //This simulates the an exchange of messages between each router in the network and its neighbours
    //Each router will receive distance vectors (least cost arrays) from each of its direct neighbours and use the values in these arrays to find a new least cost path
    //as more rounds of messages are exchanged, the routing table for each router will converge to a steady state
    for(int i=0; i<3; i++){
        //A receives messages from its neighbours A and E
        A.BellmanFord('B', B.getCosts());
        A.BellmanFord('E', E.getCosts());

        //B receives distance vectors from A, C, E and F
        B.BellmanFord('A', A.getCosts());
        B.BellmanFord('C', C.getCosts());
        B.BellmanFord('E', E.getCosts());
        B.BellmanFord('F', F.getCosts());

        //C receives messages from its neighbours B, D and F
        C.BellmanFord('B', B.getCosts());
        C.BellmanFord('D', D.getCosts());
        C.BellmanFord('F', F.getCosts());

        //D receives messages from its neighbours C and F
        D.BellmanFord('C', C.getCosts());
        D.BellmanFord('F', F.getCosts());

        //E receives messages from its neighbours A, B and F
        E.BellmanFord('A', A.getCosts());
        E.BellmanFord('B', B.getCosts());
        E.BellmanFord('F', F.getCosts());

        //F receives messages from its neighbours B, C, D and E
        F.BellmanFord('B', B.getCosts());
        F.BellmanFord('C', C.getCosts());
        F.BellmanFord('D', D.getCosts());
        F.BellmanFord('E', E.getCosts());

        //print out the routing table for each router after each exchange of messages
        cout << "--------------------ROUTING TABLES AFTER EXCHANGE " << i+1 << "--------------------\n";
        A.PrintRoutingTable();
        B.PrintRoutingTable();
        C.PrintRoutingTable();
        D.PrintRoutingTable();
        E.PrintRoutingTable();
        F.PrintRoutingTable();

        //store the routing table for each router
        A.StoreRoutingTable();
        B.StoreRoutingTable();
        C.StoreRoutingTable();
        D.StoreRoutingTable();
        E.StoreRoutingTable();
        F.StoreRoutingTable();
    }

}