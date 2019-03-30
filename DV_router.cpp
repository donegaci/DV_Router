#include "DV_router.hpp"

//create a DVRouter object
DVRouter::DVRouter(char router_name){
    identifier = router_name;        //set the router's name given the input character
    portnum = queryPort(identifier); //find the router's port number using its name

    //initialise each of the arrays to default values 
    string alphabet = "ABCDEF";
    for (int i=0; i<6; i++){
        destinations[i] = alphabet[i];  //set each of the destination router identifiers

        //array values for router's own entry
        if(i == identifier-65){
            neighbour_costs[i] = 0;         //cost to itself = 0
            least_costs[i] = 0;             //cost to itself = 0
            next_hop[i] = identifier;       //next hop = itself
            next_hop_port[i] = portnum;     //port number itself
        }
        //array values for all other routers in the network
        else{
            neighbour_costs[i] = 99;    //neighbour distances unknown
            least_costs[i] = 99;        //least costs unknown
            next_hop[i] = 'X';          //next hop unknown
            next_hop_port[i] = 00000;   //port number of next hop unknown
        }
    }

    //open the topology file
    ifstream topologyfile;
    topologyfile.open("Initial_Topology.txt");
    if(!topologyfile){
        cout << "Unable to open topology file\n";
        exit(1);
    }

    //read the contents of the file line by line, storing the appropriate values in the appropriate class members
    string linebuffer; //buffer string for each line

    while(getline(topologyfile, linebuffer)){ //get a line from the file

        if(linebuffer[0] == identifier){ //if the line is for the router reading the file, linebuffer[0] = the router's identifier

            for (int i=0; i<6; i++){ //find the index in the router's arrays corresponding to the destination router in linebuffer[2]
                if(destinations[i] == linebuffer[2]){
                    next_hop[i] = linebuffer[2];                     //store the identifier of the destination router as the next hop
                    next_hop_port[i] = stoi(linebuffer.substr(4,5)); //store the port number of the destination router
                    neighbour_costs[i] = linebuffer[10] - '0';       //store the link cost to the destination router
                    least_costs[i] = linebuffer[10] - '0';           //store the link cost to the destination router
                }
            }
        }
    }
    return;
}

//function to return a pointer to the least_cost array of the router
int * DVRouter::getCosts(){
    return least_costs;
}

//calculate the least cost path to each router in the network, given a neighbouring router's least cost array
void DVRouter::BellmanFord(char sender, int * in_costs){
    int index = sender - 65;
    int link = neighbour_costs[index];
    int newcost;
    for(int i=0; i<6; i++){
        newcost = link + in_costs[i];
        if(newcost < least_costs[i]){
            least_costs[i] = newcost;
            next_hop[i] = sender;
            next_hop_port[i] = queryPort(sender);
        }
    }
}

//find the UDP port number associated with the router identifier
int DVRouter::queryPort(char dest_name){
    if(dest_name == 'X'){
        return 0;
    }
    else if(dest_name == 'A'){
        return 10000;
    }
    else if(dest_name == 'B'){
        return 10001;
    }
    else if(dest_name == 'C'){
        return 10002;
    }
    else if(dest_name == 'D'){
        return 10003;
    }
    else if(dest_name == 'E'){
        return 10004;
    }
    else if(dest_name == 'F'){
        return 10005;
    }
    else return 0;
}

//print the router's routing/forwarding table to the terminal
void DVRouter::PrintRoutingTable(){
    cout << "----------------------------------------------------------------------\n";
    cout << "|Destination    |Cost    |Outgoing Port   |Next Hop    |Next Hop Port|\n";
    for (int i=0; i<6; i++){
        cout << "|" << setw(15) << left << destinations[i] << "|" << setw(8) << left << least_costs[i] << "|" << setw(16) << left << portnum << "|" << setw(12) << left << next_hop[i] << "|" << setw(13) << left << next_hop_port[i] << "|\n";
    }
    cout << "----------------------------------------------------------------------\n";
    cout << endl;
    return;
}

//store the router's routing/forwarding table in a text file
void DVRouter::StoreRoutingTable(){
    ofstream writefile; //open file using ofstream

    string n[1];
    n[0] = identifier;
    string filename = "routing-output" + n[0] + ".txt"; //use the router's name to open the correct file

    writefile.open(filename, ios_base::app); //open and append
    writefile << "----------------------------------------------------------------------\n";
    writefile << "|Destination    |Cost    |Outgoing Port   |Next Hop    |Next Hop Port|\n";
    for (int i=0; i<6; i++){
        writefile << "|" << setw(15) << left << destinations[i] << "|" << setw(8) << left << least_costs[i] << "|" << setw(16) << left << portnum << "|" << setw(12) << left << next_hop[i] << "|" << setw(13) << left << next_hop_port[i] << "|\n";
    }
    writefile << "----------------------------------------------------------------------\n";
    writefile << endl;
    
    writefile.close(); //close the file
    return;
}

// returns a string with DV table of the format
// destination,cost
string DVRouter::ConstructMessage(){

    string s = "";
    s.push_back(identifier);
    s.append("'s DV Table\n");
    for (int i=0; i<6; i++){
        s.push_back(destinations[i]);
        s.push_back(',');
        s.append(to_string(least_costs[i]));
        s.append("\n");
    }
    return s;
}

// returns a pointer to array of neighbours DV table
int* DVRouter::ParseMessage(string dvTable) {

    int *arr = new int(6);
    string s;
    int pos1, pos2;

    // trim off the header
    dvTable.erase(0, dvTable.find("\n"));

    for (int i=0; i<6; i++){
        pos1 = dvTable.find(',');
        pos2 = dvTable.find('\n');
        if (pos1 != string::npos && pos2 != string::npos){ // ensure the delimiters are found
            s = dvTable.substr(pos1+1, pos2-2);
            arr[i] = stoi(s); // convert string to int
            dvTable.erase(0, pos2+1);
        }
        else
            arr[i] = 99;
    }

    return arr;
}