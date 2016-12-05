//
// Created by System Administrator on 10/15/15.
//

#ifndef NLPCPP_PREFILTER_H
#define NLPCPP_PREFILTER_H

#include <cstdio>
#include <vector>
#include "../../data_type.hh"
#include "../../utils/string_util.hh"

using namespace std;

namespace jieba {

    class PreFilter {
    /* 作用是提前按照_symbols将句子分割*/
    public:
        struct Range {
            Range() {};
            Range(typename Unicode::const_iterator iter1, typename Unicode::const_iterator iter2):\
               begin(iter1), end(iter2) {};
            typename Unicode::const_iterator begin;
            typename Unicode::const_iterator end;
        };
    public:
        PreFilter(const Unicode& sentence, const unordered_set<uint16_t>& sep_symbols):\
          _sent(sentence), _symbols(sep_symbols), _cursor(sentence.begin()) {}
        ~PreFilter() {}

        Range Next();
        bool hasNext();

    private:
        const Unicode& _sent;
        const unordered_set<uint16_t>& _symbols; // 用于分割字符
        typename Unicode::const_iterator _cursor; // 当前在句子中位置
    };
}



#endif //NLPCPP_PREFILTER_H
