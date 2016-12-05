
BUILD_TARGETS=main
CC=/usr/local/bin/g++
CXX=/usr/local/bin/g++

CXXFLAGS=-g -O3 -Wall -Wextra -Wno-sign-compare  -I/opt/local/include/ -I/usr/local/include/ 

all: $(BUILD_TARGETS)

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

main: io/read.o utils/emo_util.o utils/string_util.o main.o


.depend: depend

depend: main.cc io/read.cc utils/emo_util.cc utils/string_util.cc
	-rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

include .depend

.PHONY : clean
clean:
	-rm -rf $(BUILD_TARGETS) 
	-rm -rf io/read.o io/read utils/emo_util.o utils/emo_util
