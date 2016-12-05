//
// Created by System Administrator on 10/16/15.
//

#include "MixSeg.hh"

namespace jieba {

    void MixSeg::cut(vector<Unicode>& res, const Unicode& sent, bool hmm) {
        PreFilter prefiler(sent, _symbols);
        PreFilter::Range range;
        /*string tmp; string_util::unicode2utf8(tmp, sent);
        printf("sentence: %s\n", tmp.c_str());
        printf("sentence size: %ld\n", sent.size());*/
        while (prefiler.hasNext()){
            range = prefiler.Next();
            //printf("range.begin: %ld, range.end: %ld\n", range.begin-sent.begin(), range.end-sent.begin());
            /*printf("range.word: ");
            Unicode tmp_u(range.begin, range.end);
                string_util::unicode2utf8(tmp, tmp_u);
                printf(" %s", tmp.c_str());
            printf("\n");*/
            cut(res, range.begin, range.end, hmm);
        }
    }
    void MixSeg::cut(vector<Unicode>& res, \
                 typename Unicode::const_iterator& begin, typename Unicode::const_iterator& end, bool hmm) {
        /*
         * hmm指定是否使用hmm */
        if (!hmm) {
            _mp_seg->cut(res, begin, end);
            return;
        }
        vector<Unicode> words;
        vector<Unicode> hmm_res;
        _mp_seg->cut(words, begin, end);

        /* string tmp1, tmp2;
        printf("words.size(): %ld\n", words.size());
        for (size_t i=0; i<words.size(); ++i) {
            string_util::unicode2utf8(tmp2, words[i]);
            printf("tmp2: %s\n", tmp2.c_str());
            tmp1.append("/");
            tmp1.append(tmp2);
        }
        printf("-----mpseg words: %s\n", tmp1.c_str());
         */

        //printf("3\n");
        Unicode buf;
        Unicode::const_iterator iter1, iter2;
        for (size_t i=0; i<words.size(); ) { // 对于最大概率分出来结果做进一步处理
            if (1 < words[i].size() || _mp_seg->IsUsrDictSingleZh(words[i][0])) { // 1，是字典词，最大概率分出来长度必然大于1
                /*printf("MPSeg direct\n");
                string_util::unicode2utf8(tmp2, words[i]);
                printf("DP Direct %s\n", tmp2.c_str());*/
                res.push_back(words[i++]);
            }
            else { // 2，不是字典词
                size_t j = i;
                while (j<words.size() && words[j].size()==1 && !_mp_seg->IsUsrDictSingleZh(words[j][0])) {
                    buf.push_back(words[j][0]);
                    ++j;
                }
                iter1 = buf.begin(); iter2 = buf.end();
                //printf("now comes the hmm: j=%ld\n", j);
                _hmm_seg->cut(hmm_res, iter1, iter2); // MP分不了的才交给HMM分
                /* 下面做法是错误的，临时变量不能作为引用传递，编译器报错：error: invalid initialization of non-const reference of type
                _hmm_seg.cut(hmm_res, buf.begin(), buf.end()); // MP分不了的才交给HMM分
                 */
                for (size_t k=0; k<hmm_res.size(); ++k)
                    res.push_back(hmm_res[k]);
                buf.clear();
                hmm_res.clear();
                i = j; // 设置i
            }
        }
    }

}