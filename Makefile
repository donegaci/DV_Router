CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -std=c++11 -g -Wall -pthread -lboost_system $(CXXOPTIMIZE)
USERID=Group15
CLASSES= DV_router.hpp

all: my-router

my-router: $(CLASSES)
	$(CXX) -o $@ $^ $@.cpp $(CXXFLAGS)

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM *.tar my-router routing-outputA.txt routing-outputB.txt ...\
		routing-outputC.txt routing-outputD.txt routing-outputE.txt routing-outputF.txt 

tarball: clean
	tar -cvf project2_$(USERID).tar *
