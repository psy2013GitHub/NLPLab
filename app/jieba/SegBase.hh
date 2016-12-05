//
// Created by System Administrator on 10/15/15.
//

#ifndef NLPCPP_SEGBASE_H
#define NLPCPP_SEGBASE_H

#include <cassert>
#include "../../data_type.hh"

using namespace std;

namespace jieba {

    //const char* const SPECIAL_CHARS = " \t\n，。";
    const uint16_t SPECIAL_SYMBOL[] = {32u, 9u, 10u, 65292u, 12290u};

    class SegBase {
    public:
        SegBase() {load_special_symbols();}
        ~SegBase() {}

    protected: // 定义为protected，便于继承
        unordered_set<uint16_t> _symbols;
        void load_special_symbols();
    };

}

#endif //NLPCPP_SEGBASE_H
