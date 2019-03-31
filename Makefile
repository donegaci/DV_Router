CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread  $(CXXOPTIMIZE)
USERID=Group15
CLASSES=

all: my-router

udp-server: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp
	
udp-client: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM *.tar my-router routing-outputA.txt routing-outputB.txt ...\
		routing-outputC.txt routing-outputD.txt routing-outputE.txt routing-outputF.txt 

tarball: clean
	tar -cvf project2_$(USERID).tar *