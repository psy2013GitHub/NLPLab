//
// Created by System Administrator on 10/15/15.
//

#include "SegBase.hh"

namespace jieba {
    void SegBase::load_special_symbols() {
        size_t size = sizeof(SPECIAL_SYMBOL)/sizeof(*SPECIAL_SYMBOL);
        for (size_t i = 0; i < size; i ++) {
            _symbols.insert(SPECIAL_SYMBOL[i]);
        }
        assert(_symbols.size());
    }
}