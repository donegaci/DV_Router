#include "DataSegment.hpp"


// Overloaded constructor to create data segment
// status = 'D' for data packet
// status = 'C' for control packet
DataSegment::DataSegment(char status, int sourcePort, int destPort, int length, string payload){
    header.sourcePort = sourcePort;
    header.destPort = destPort;
    header.status = status;
    header.payloadLength = length;
    this->payload = payload;
    return;
}

// Overloaded constructor which takes a string and parses it into a data segment object
DataSegment::DataSegment(string dataSegment){

    // parse the string
    char delimitter = ' ';
    unsigned long position = string::npos;

    // set status 
    if ( (position = dataSegment.find(delimitter)) != string::npos ){
        header.status = dataSegment.substr(0, position)[0]; // index 0 because I am assinging string type to character
        dataSegment.erase(0, position + 1);
    }
    // set sourcePort 
    if ( (position = dataSegment.find(delimitter)) != string::npos ){
        header.sourcePort = stoi(dataSegment.substr(0, position)); // convert string representation of int to int type
        dataSegment.erase(0, position + 1);
    }
    // set destPort 
    if ( (position = dataSegment.find(delimitter)) != string::npos ){
        header.destPort = stoi(dataSegment.substr(0, position)); // convert string representation of int to int type
        dataSegment.erase(0, position + 1);
    }
    // set length
    delimitter = '\n'; 
    if ( (position = dataSegment.find(delimitter)) != string::npos ){
        header.payloadLength = stoi(dataSegment.substr(0, position)); // convert string representation of int to int type
        dataSegment.erase(0, position + 1);
    }

    // set payload
    if ( (position = dataSegment.find(delimitter)) != string::npos ){
        this->payload = dataSegment.substr(0, dataSegment.length() -1); // convert string representation of int to int type
        dataSegment.erase(0, position + 1);
    }
}

string DataSegment::constructDataSegment(){

    string out = "";
    out.push_back(header.status);
    out.append(" ");
    out.append(to_string(header.sourcePort));
    out.append(" ");
    out.append(to_string(header.destPort));
    out.append(" ");
    out.append(to_string(header.payloadLength));
    out.push_back('\n');
    out.append(payload);
    out.push_back('\n');

    return out;
}

void DataSegment::printDataSegment(){
    cout << " ---------------------------------------------------------------------------" << endl;
    cout << "|  Status " << header.status  
        << " |  SourcePort " << header.sourcePort 
        << " |  DestinationPort " << header.destPort 
        << " |  Length " << header.payloadLength  << "   |" << endl
        << " ---------------------------------------------------------------------------" << endl
        << "| " << payload << endl
        << " ---------------------------------------------------------------------------" << endl;
}