//
// Created by System Administrator on 10/13/15.
//

#ifndef NLPCPP_DATA_TYPE_HH
#define NLPCPP_DATA_TYPE_HH

#include <vector>

/* uint8_t/uint16_t 定义*/
#include <cstdint>

//typedef unsigned char uint8_t; // 无符号1字节

//typedef unsigned int uint16_t; // 无符号4字节
// typedef unsigned int size_t; fix me!!!

typedef uint16_t Rune; // included in jieba
typedef std::vector<uint16_t> Unicode; // 就是宽字符向量

/*  unordered_map 定义 */
#ifdef __APPLE__
#  include <unordered_map>
#  include <unordered_set>
#elif(__cplusplus == 201103L)
#  include <unordered_map>
#  include <unordered_set>
#elif defined _MSC_VER
#  include <unordered_map>
#  include <unordered_set>
#else
#  include <tr1/unordered_map>
#  include <tr1/unordered_set>
namespace std {
using std::tr1::unordered_map;
using std::tr1::unordered_set;
}

#endif

#endif //NLPCPP_DATA_TYPE_HH
