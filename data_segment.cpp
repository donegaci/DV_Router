
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

struct header{
	char sourceport, destport;
	int type, length;
}headerinfo;

//two types of messages, data and control 
enum type { data, control };

void *CreateDataSeg(int type, char sourceport, char destport, int plength, void *payload);
void* getpayload(void *datasegment,int length);
header getheader(void* datasegment);

//creating data segment with header and payload
void *CreateDataSeg(int type, char sourceport, char destport, int plength, void *payload){
	int allocpayload= plength;
	if((type!=data))
	allocpayload=0;

	//getting size of struct header in variable called size
	//headerinfo *size = NULL;
	//size++;
struct header sizeofarray[2];
int size = (char*)&sizeofarray[1]-(char*)&sizeofarray[0];
	void *datasegment = malloc(size + allocpayload);

	//creating header
	header hh;
	hh.type = type;
	hh.sourceport = sourceport;
	hh.destport = destport;
	hh.length = plength;

//filling in the packet with memcpy
std::copy(datasegment,(void*)&hh,size);
std::copy((void*)((char*)datasegment+size),payload, allocpayload);
return datasegment;
}


//get payload from datasegment
void* getpayload(void *datasegment,int length){
	//headerinfo *size = NULL;
	//size++;
struct header sizeofarray[2];
int size = (char*)&sizeofarray[1]-(char*)&sizeofarray[0];
	void *payload= malloc(length);
	std::copy(payload, (void*)((char*)datasegment+size),length);
	return payload;
}

//get header from the data segment
header getheader(void *datasegment){
	header hh;
	//headerinfo *size = NULL;
	//size++;
struct header sizeofarray[2];
int size = (char*)&sizeofarray[1]-(char*)&sizeofarray[0];
	std::copy((void*)(&hh),datasegment,size);
	return hh;
}

int main(){
//	if(dv.getname=='I'){
		char data[100];
		memset(data,0,100);
	// still left to work here
	}