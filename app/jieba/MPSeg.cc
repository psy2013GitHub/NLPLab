//
// Created by System Administrator on 10/15/15.
//

#include "MPSeg.hh"
#include "../../utils/string_util.hh"

namespace jieba {

    bool MPSeg::IsUsrDictSingleZh(const uint16_t& input) const {
        // 该函数返回是不是用户字典单一字，在MixSeg里与“无字典字”想区分，因为目前尚没有考虑添加用户字典，所以暂不实现
        return false;
    }

    void MPSeg::cut(vector<Unicode>& res, const Unicode &sent) const {
        PreFilter prefilter(sent, _symbols);
        PreFilter::Range range;
        while (prefilter.hasNext()) { // 按照symbols对句子进行分割
            range = prefilter.Next();
            cut(res, range.begin, range.end); // 对每一段进行处理
        }
        return;
    }

    void MPSeg::cut(vector<Unicode>& res, \
                Unicode::const_iterator& begin, Unicode::const_iterator& end, \
                size_t max_word_len) const {
        /*Unicode utmp(begin, end);
        string tmp;
        string_util::unicode2utf8(tmp, utmp);
        printf("MPSeg cut word: %s\n", tmp.c_str());*/
        vector<DAG> dags;
        //printf("MPSeg::iterator cut\n");
        _dict_trie->genDAG(dags, begin, end); // 生成有向无环图
        /*for (size_t i=0; i<dags.size(); ++i) {
            printf("i: %ld\n", i);
            for (size_t j=0; j<dags[i].nexts.size(); ++j)
                printf(" j: %ld", dags[i].nexts[j].first);
            printf("\n");
        }*/
        CalcDP(dags);
        CutByDag(res, dags);
    }

    void MPSeg::CalcDP(vector<DAG> &dags) const {
        for (typename vector<DAG>::reverse_iterator riter=dags.rbegin(); riter!=dags.rend(); ++riter) {
            // 动态规划求解最大可能路径
            string tmp;
            riter->pinfo = NULL; riter->logprob = jieba::MINDOUBLE; //riter->word.clear();
            size_t j_pos = 0; // iter_j在句子中的位置
            double logprob = 0.0;
            if (riter->nexts.size()) { // 可以多字成词
                const DictUnit* unit_ptr = NULL;
                for (typename vector<pair<size_t, const DictUnit*> >::iterator iter_j= riter->nexts.begin(); \
                         iter_j!=riter->nexts.end(); ++iter_j) { // 遍历所有能与riter成词的字，unordered_map在不同机器上iter顺序不固定, undefined
                    logprob = 0.0;
                    j_pos = iter_j -> first;
                    unit_ptr = iter_j -> second;

                   /* printf("MPSeg: iter_j: %ld\n", j_pos);
                    string tmp;
                    printf("dag unit_ptr == NULL: %d\n", unit_ptr==NULL);
                    if (unit_ptr) {
                        string_util::unicode2utf8(tmp, unit_ptr->word);
                        printf("dag word: %s\n", tmp.c_str());
                    }*/
                    /* 计算route[j_pos+1]的概率，注意是j_pos+1 */
                    if (j_pos + 1 < dags.size()) // 越界，那么默认为0
                        logprob += dags[j_pos+1].logprob;  //
                    /* 计算riter与iter_j成词的概率 */
                    logprob += (unit_ptr!=NULL)?unit_ptr->weight:_dict_trie->GetMinWeight(); //
                    //printf("j_pos = %ld, ", j_pos);
                    /*if (unit_ptr!=NULL) {
                        string_util::unicode2utf8(tmp, unit_ptr->word);
                        printf("word: %s ", tmp.c_str());
                    }

                    else
                         printf("word:  ");

                         printf("iter_j-riter->nexts.begin() = %ld, logprob: %g, ritter.logprob:% g\n", \
                         iter_j-riter->nexts.begin(), logprob, riter->logprob);
                         */
                    /* 是不是比已有值更大 */
                    if (riter->logprob < logprob) { // 大于已有概率
                        riter -> logprob = logprob;
                        riter -> pinfo   = unit_ptr;
                        riter -> nextPos = j_pos; // 下一个位置是j_pos
                    }
                }
            } else { // 单字成词
                printf("Error: wrong format dag, check DictTrie::genDAG() code\n");
                return;
            }
            if (riter->pinfo) {
                string_util::unicode2utf8(tmp, riter->pinfo->word);
                //printf("CalcDP word: %s\n", tmp.c_str());
            } else {
                //printf("CalcDP Special word: %.4x\n", riter->word);
                string_util::unicode2utf8(tmp, Unicode(1, riter->word));
            }
           // printf("riter_final word: %ld\n", riter->nextPos);
        }
    }

    void MPSeg::CutByDag(vector<Unicode>& res, const vector<DAG>& dags) const {
        string tmp; Unicode u_tmp;
        for (typename vector<DAG>::const_iterator iter = dags.begin(); iter!=dags.end(); ) {
            if (iter->pinfo) {
                res.push_back(iter->pinfo->word);
                string_util::unicode2utf8(tmp, iter->pinfo->word);
                //printf("wordlen > 1: %s\n", tmp.c_str());
                iter += iter->pinfo->word.size();
            } else { // “特殊字符” 或 “字不在DictTrie里”
                //printf("Special symbols %.4x\n", iter->word);
               res.push_back(Unicode(1,iter->word)); // Unicode是vector可以这样初始化；
                ++iter;
            }
        }
    }

}