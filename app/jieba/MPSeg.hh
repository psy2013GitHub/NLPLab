//
// Created by System Administrator on 10/15/15.
//

#ifndef NLPCPP_MPSEG_H
#define NLPCPP_MPSEG_H

#include "PreFilter.hh"
#include "../../data_type.hh"
#include "../../dsa/graph/Trie.hh"
#include "DictTrie.hh"
#include "SegBase.hh"

namespace jieba {

    const size_t MAXWORDLEN(10000);

    class MPSeg: public SegBase { /* Maxinum Probility 最大概率分词法 */
    public:
        MPSeg() {}; // 默认初始化
        MPSeg(const DictTrie* dict_trie_ptr=NULL): \
              SegBase(),
              _dict_trie(dict_trie_ptr) {};
        ~MPSeg() {};
        void cut(vector<Unicode>& res, const Unicode& sent) const; // cut 整个句子
        void cut(vector<Unicode>& res, \
                Unicode::const_iterator& begin, Unicode::const_iterator& end, \
                size_t max_word_len = MAXWORDLEN) const;
        bool IsUsrDictSingleZh(const uint16_t& input) const;
    private:
        const DictTrie* _dict_trie;

        /* 动态规划法计算最大可能cut */
        void CalcDP(vector<DAG>& dags) const; // 最大可能路径
        void CutByDag(vector<Unicode>& res, const vector<DAG>& dags) const; // 按照最大可能路径cut
    };
}


#endif //NLPCPP_MPSEG_H
