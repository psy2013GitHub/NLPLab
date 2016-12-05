
BUILD_TARGETS=test_string_util
CC=/usr/local/bin/g++
CXX=/usr/local/bin/g++

CXXFLAGS=-g -O3 -Wall -Wextra -Wno-sign-compare  -I/opt/local/include/ -I/usr/local/include/ -std=c++0x#-lstdc++ 

all: $(BUILD_TARGETS)

%.o:%.cc 
	$(CXX) $(CXXFLAGS) -c $< -o $@


test_string_util: ../io/read.o ../utils/string_util.o test_string_util.o ../app/jieba/SegBase.o ../dsa/graph/Trie.o ../app/jieba/DictTrie.o ../app/jieba/Prefilter.o ../app/jieba/MPSeg.o ../app/jieba/HMMModel.o ../app/jieba/HMMSeg.o ../app/jieba/MixSeg.o ../app/jieba/Jieba.o

.depend: depend

depend: test_string_util.cc ../io/read.cc ../utils/string_util.cc ../app/jieba/SegBase.cc ../dsa/graph/Trie.cc ../app/jieba/DictTrie.cc ../app/jieba/Prefilter.cc ../app/jieba/MPSeg.cc ../app/jieba/HMMModel.cc ../app/jieba/HMMSeg.cc ../app/jieba/MixSeg.cc ../app/jieba/Jieba.cc
	-rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

include .depend

.PHONY : clean
clean:
	-rm -rf $(BUILD_TARGETS) 
	-rm -rf main ../io/read.o ../utils/string_util.o test_string_util.o ../app/jieba/SegBase.o ../dsa/graph/Trie.o ../app/jieba/DictTrie.o ../app/jieba/Prefilter.o ../app/jieba/MPSeg.o ../app/jieba/HMMModel.o ../app/jieba/HMMSeg.o ../app/MixSeg.o ../app/jieba/Jieba.o
