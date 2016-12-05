//
// Created by System Administrator on 10/16/15.
//

#include "HMMSeg.hh"

namespace jieba {

    void HMMSeg::cut(vector<Unicode>& res, const Unicode& sent) const {
        PreFilter prefiler(sent, _symbols);
        PreFilter::Range range;
        while (prefiler.hasNext()) {
            range = prefiler.Next();
            cut(res, range.begin, range.end);
        }
    }

    void HMMSeg::cut(vector<Unicode>& res, \
          typename Unicode::const_iterator& begin, typename Unicode::const_iterator& end) const {
        /* 处理被_symbol分割后的bulk片段 */
        assert(begin < end);
        typename Unicode::const_iterator left  = begin;
        typename Unicode::const_iterator right = begin;
        while (right < end) {
            if (*right < 0x80) { // ascini编码
                if (left < right) { // [begin, end) 是中文
                    zh_cut(res, left, right);
                    left = right;
                } else { // 从begin开始找连续非中文，直接push_back到res
                    left = right;
                    do {
                        right = consec_alpha(left, end); // right对于left后第一个中文
                        if (left!=right) {
                            //printf("consec alpha found\n");
                            break;
                        }
                        right = consec_num(left, end);
                        if (left!=right) {
                            //printf("consec num found\n");
                            break;
                        }
                        ++right; // 既非中文，亦非字母或数字
                    } while (false);
                    res.push_back(Unicode(left, right<end?right:end));
                    left = right;
                }
            } else ++right;
        }
        if (left<right)
            zh_cut(res, left, right);
    }

    void HMMSeg::zh_cut(vector<Unicode> &res, Unicode::const_iterator &begin,
                        Unicode::const_iterator &end) const {
        jieba::state state_vec[end-begin];
        double prob = virtabi(state_vec, begin, end);
        string tmp;
        Unicode utmp(begin, end);
        string_util::unicode2utf8(tmp, utmp);
        //printf("zh_cut word: %s\n", tmp.c_str());
        //printf("virtabi probability %g\n", prob);
        //printf("state vec: ");
        /*for (size_t i=0; i<end-begin; ++i)
            printf(" %d ", state_vec[i]);
        printf("\n");
         */
        cut_by_state(res, state_vec, begin, end);
    }

    typename Unicode::const_iterator HMMSeg::consec_alpha(typename Unicode::const_iterator left, \
                                                typename Unicode::const_iterator limit) const {
        /* 注意，此处是传值调用 */
        if (left==limit) return limit;
        assert(left<limit);
        if (string_util::isalpha(*left)) {
            ++left;
        } else return left;
        while (left<limit) {
            if (string_util::isalpha(*left) || string_util::isnum(*left)) { // QQ3123603158将被分割为一个？？？
                ++left;
            }
        }
        return left;
    }
    typename Unicode::const_iterator HMMSeg::consec_num(typename Unicode::const_iterator left, \
                                                typename Unicode::const_iterator limit) const {
        if (left==limit) return limit;
        assert(left<limit);
        if (string_util::isnum(*left)) {
            ++left;
        } else return left;
        while (left<limit) {
            if (*left=='.' || string_util::isnum(*left)) { // 包括小数
                ++left;
            }
        }
        return left;
    }

    void fill_pre(double* prev_array, double* array, size_t size) {
        double tmp;
        for (size_t i=0; i<size; ++i) {
            prev_array[i] = array[i];
        }
    }

    double HMMSeg::virtabi(jieba::state* state_vec, \
              typename Unicode::const_iterator& left, typename Unicode::const_iterator& right) const {
        /*
         * stata_vec是最终最优路径
         *  */
        assert(left < right);
        double prob = 0.0;
        double emitProb = jieba::MINDOUBLE;
        double tmp = 0.0;
        jieba::state best_prev_state=jieba::state::B;
        jieba::state state_mat[right-left+1][STATENUM];
        double prev_prob_vec[STATENUM];
        double prob_vec[STATENUM];

        // 确定开始字状态
        for (size_t i = 0; i < STATENUM; ++i) { // 取最大值作为开始状态
            prob_vec[i] = _hmmmodel._start_p[i] + _hmmmodel.getEmitProb(_hmmmodel.emitProbVec[i], *left, jieba::MINDOUBLE);
            //printf("%ld: after prob_vec[%ld]=%g \n", 0, i, prob_vec[i]);
            state_mat[0][i] = jieba::state::B; // 任意设置
        }
        fill_pre(prev_prob_vec, prob_vec, STATENUM);

        // 确定中间自状态
        for (typename Unicode::const_iterator iter = left+1; iter != right; ++iter) {
            /* 注意，由于分词的特点，只能是 B->M/E E->B/S M->M/E S->B/S，这里不用管，因为hmm_model.utf8 将不能转换值设成了MINDOUBLE */
            for (size_t i = 0; i < STATENUM; ++i) {
                prob = jieba::MINDOUBLE;
                tmp  = jieba::MINDOUBLE;
                emitProb = _hmmmodel.getEmitProb(_hmmmodel.emitProbVec[i], *iter, jieba::MINDOUBLE);
                //printf("%ld: emitProb: %g \n", iter-left, emitProb);
                for (size_t j = 0; j < STATENUM; ++j) {
                    tmp = _hmmmodel._trans_p[j][i] + prev_prob_vec[j] + emitProb;
                    //printf("transProb[%ld][%ld]: %g, tmp[%ld]: %g", j, i, _hmmmodel._trans_p[j][i], j, tmp);
                    if (prob < tmp) {
                        prob = tmp;
                        best_prev_state = jieba::state(j);
                    }
                }
                //printf("\n prob_vec[%ld]: %g\n",i, prob);
                prob_vec[i] = prob;
                state_mat[iter-left][i] = best_prev_state; // i 时间点下 最优 i-1 状态
            }
            fill_pre(prev_prob_vec, prob_vec, STATENUM);
        }
        /* 确定自后向前状态序列 */
        //  首先，算最后个时间点的状态，跟python jieba一样，最后点只考虑E和S
        double max_prob = 0.0;
        jieba::state max_last_idx=jieba::state::E; // 默认是E
        //printf("E-S=%g\n", prob_vec[size_t(jieba::state::E)] - prob_vec[size_t(jieba::state::S)]);
        if (prob_vec[size_t(jieba::state::E)] < prob_vec[size_t(jieba::state::S)]) {
            //printf("S won\n");
            max_last_idx = jieba::state::S;
        }
        max_prob = prob_vec[max_last_idx];
        // 其次，自后向前计算每个中间时间点的状态
        for (size_t i=1; left+i<right+1; ++i) {
            state_vec[right-left-i]=max_last_idx;
            max_last_idx = state_mat[right-left-i][max_last_idx];
        }
        return max_prob;
    }

    void HMMSeg::cut_by_state(vector<Unicode>& res, jieba::state* state_vec, \
                    typename Unicode::const_iterator& begin, typename Unicode::const_iterator& end) const {
                    // 按照隐状态对句子进行切分;
        Unicode::const_iterator left = begin;
        Unicode::const_iterator right = begin;
        for (size_t i=0; left < end && i<end-begin; i++) {
            if (state_vec[i]==jieba::E || state_vec[i]==jieba::S) { // 枚举类型
                right = begin+i+1;
                res.push_back(Unicode(left, right)); // 有没有处理 BMMS 这种情况？
                left = right;
            }
        }
    }

}
