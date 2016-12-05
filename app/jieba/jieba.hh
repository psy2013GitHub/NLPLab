//
// Created by System Administrator on 10/15/15.
//

#ifndef NLPCPP_JIEBA_H
#define NLPCPP_JIEBA_H


#include <string>
#include <vector>
#include "../../data_type.hh"
#include "../../utils/string_util.hh"
#include "../../dsa/graph/Trie.hh"
#include "DictTrie.hh"
#include "MixSeg.hh"

using namespace std;

namespace jieba {


    class Jieba {
    public:
        enum cut_option {NO_HMM, HMM};

        Jieba (const string& default_dict_path, const string& usr_dict_path, \
               const string& hmm_model_path): \
        _dict_trie(default_dict_path, usr_dict_path), _hmm_model_path(hmm_model_path), \
        _mp_seg(NULL), _hmm_seg(NULL), _mix_seg(NULL) {
            /* 重新初始化
             * */
            _mp_seg  = new MPSeg(&_dict_trie);
            _hmm_seg = new HMMSeg(hmm_model_path);
            _mix_seg = new MixSeg(_mp_seg, _hmm_seg);
        };

        ~Jieba () {
            delete _mp_seg;
            delete _hmm_seg;
            delete _mix_seg;
        }

        void cut (vector<Unicode>& res, const string& sent, const cut_option option=HMM); // 默认配合使用HMM分词


    private:
        DictTrie _dict_trie;
        string _hmm_model_path;
        MPSeg*  _mp_seg;
        HMMSeg* _hmm_seg;
        MixSeg* _mix_seg;
    };


}


#endif //NLPCPP_JIEBA_H

/*
 * 1，加载字典前缀树，建立词图；
 * 2，分词；
 * */