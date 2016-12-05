//
// Created by System Administrator on 10/16/15.
//

#ifndef NLPCPP_HMMSEG_H
#define NLPCPP_HMMSEG_H

#include <vector>

#include "GeneralConfig.hh"
#include "../../data_type.hh"
#include "PreFilter.hh"
#include "SegBase.hh"
#include "HMMModel.hh"

using namespace std;

namespace jieba {

    class HMMSeg: public SegBase {
    public:
        HMMSeg(const string& model_path): \
           /*基类初始化*/ SegBase(),
           /*派生类初始化*/_hmmmodel(model_path) {}

        void cut(vector<Unicode>& res, const Unicode& sent) const;
        void cut(vector<Unicode>& res, \
                 typename Unicode::const_iterator& begin, typename Unicode::const_iterator& end) const; // 处理所有字符
        void zh_cut(vector<Unicode>& res, \
                 Unicode::const_iterator& begin, Unicode::const_iterator& end) const; // 只处理中文
        void cut_by_state(vector<Unicode>& res, jieba::state* state_vec, \
                    typename Unicode::const_iterator& begin, typename Unicode::const_iterator& end) const;
        double virtabi(jieba::state* state_vec, \
              typename Unicode::const_iterator& left, typename Unicode::const_iterator& right) const;
    private:
        HMMModel _hmmmodel;
        typename Unicode::const_iterator consec_alpha(typename Unicode::const_iterator left, \
                                            typename Unicode::const_iterator limit) const;
        typename Unicode::const_iterator consec_num(typename Unicode::const_iterator left, \
                                            typename Unicode::const_iterator limit) const;
    };

}

#endif //NLPCPP_HMMSEG_H
