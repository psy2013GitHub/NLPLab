//
// Created by System Administrator on 10/15/15.
//

#include "PreFilter.hh"

namespace jieba {

    PreFilter::Range PreFilter::Next() {
        Range range;
        range.begin = _cursor;
        while (_cursor < _sent.end()) {
            if (_symbols.find(*_cursor)!=_symbols.end()) { // 如果iter是分割字符
                if (range.begin==_cursor) {// 如果当前字符就是分割字符，将range范围设置为1，弹出分割字符
                    // printf("Prefilter single range: *_cursor = %.4x", *_cursor);
                    ++_cursor;
                }
                range.end = _cursor;
                return range;
            }
            ++_cursor;
        }
        range.end = _sent.end(); // 最后一段，因为使用时一般调用hasNext()，所以最后一段不可能为空；
        return range;
    }

    bool PreFilter::hasNext() {
        return _cursor != _sent.end();
    }
}