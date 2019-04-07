
# Distance Vector Routers
The purpose of this project is to implement, validate and demonstrate a simple Distance Vector protocol of the type that might be used by Peer to Peer, Adhoc, Wireless Sensor or Internet of Things communication devices. 

The project implements a router that communicates with other routers by exchanging UDP datagrams and uses a distance vector routing protocol that implements the distributed Bellman-Ford algorithm.

# Instructions

This project uses the **Boost Library**. See [Getting Started Guide](https://www.boost.org/doc/libs/1_66_0/more/getting_started/unix-variants.html) for install instructions.

To run the startup script, `startup.sh`, on Mac, you will need to install **Xquartz** available [here](https://www.xquartz.org/).


To run the code and start up all routers:
```
make
./startup
```
