#include <string>
#include <iostream>
using namespace std;

// Header will be used to direct the message to its destination and provide information about its purpose
typedef struct Header Header;
struct Header {
    char status; // distinguish between a data packet (status='D') and a control message (status='C')
    int sourcePort;
    int destPort;
    int payloadLength; // length of payload message
};


/*         Structure of data segment       
*  -----------------------------------------
* | status | sourcePort | destPort | length |
*  -----------------------------------------
* |                                         |
* |             payload                     |
* |                                         |
*  -----------------------------------------
*/

class DataSegment{
    private:
        Header header;
        string payload;
    public:
        // Overloaded constructor to create data segment
        // Will be used on the sending side
        DataSegment(char status, int sourcePort, int destPort, int length, string payload);

        // Overloaded constructor which takes a string representing a data segment 
        // (i.e. a string of the data segment structute above) and parses it into a data segment object
        // Will be used on the receiving side
        DataSegment(string dataSegment);

        // returns a string of data segment in the format of the structure above
        string constructDataSegment();
        
        char getStatus() {return header.status;}
        string getPayload() {return payload;}
        int getDestPort() {return header.destPort;}
        int getSourcePort() {return header.sourcePort;}
        
        // This is mostly for testing purposes
        void printDataSegment();
};
