#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;

struct header{
	char sourceport, destport;
	int type, length;
};
enum type { data, control };

void *CreateDataSeg(int type, char sourceport, char destport, int payload, void *payload);
void* getpayload(void *datasegment,int length);
void* getheader(void* datasegment);

//creating data segment with header and payload
void *CreateDataSeg(int type, char sourceport, char destport, int payload, void *payload){
	int allocpayload= payload;
	if((type!=data))
	allocpayload=0;

	header *size = NULL;
	size++;
	void *datasegment = malloc(size + allocpayload);
	//creating header
	header hh;
	hh.type = type;
	hh.sourceport = sourceport;
	hh.destport = destport;
	hh.length = length;

//filling in the packet with memcpy
std::memcpy(datasegment,(void*)&hh,size);
std::memcpy((void*)((char*)datasegment+size),payload, allocpayload);
return datasegment;
}

void* getpayload(void *datasegment,int length){
	header *size = NULL;
	size++;
	void *payload= malloc(length);
	memcpy(payload, void*((char*)datasegment+size),length);
	return payload;
}

void* getheader(void* datasegment){
	header hh;
	header *size = NULL;
	size++;
	memcpy(void*(&hh),datasegment,size);
	return header;
}

int main(){
	if(dv.getname=='I'){
		char data[100];
		memset(data,0,100);

	}
}