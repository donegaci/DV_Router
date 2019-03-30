#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iomanip>

using namespace std;

class DVRouter{
private:
    char identifier;            //letter identifying the router
    int portnum;                //the router's UDP port number

    int least_costs[6];         //least cost paths to each destination router in the network

    char destinations[6];       //identifiers of all destination routers in the network
    char next_hop[6];           //identifier of the next-hop routers in the least-cost path
    int next_hop_port[6];       //port number of next-hop router in the least cost path

public:
    int neighbour_costs[6];     //link costs to the router's immediate neighbours

    DVRouter(char router_name);                         //create a router given its identifier

    int * getCosts();                                   //function to return a pointer to the least_cost array of the router

    void BellmanFord(char sender, int * in_costs);      //calculate the least cost path to each router in the network, given a neighbouring router's least cost array

    int queryPort(char dest_name);                      //find the UDP port number associated with the router's name

    void PrintRoutingTable();                           //print the router's routing/forwarding table to the terminal
    void StoreRoutingTable();                           //store the router's routing/forwarding table in a text file

};