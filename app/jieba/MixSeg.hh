//
// Created by System Administrator on 10/16/15.
//

#ifndef NLPCPP_MIXSEG_H
#define NLPCPP_MIXSEG_H

#include <vector>

#include "../../utils/string_util.hh"
#include "../../data_type.hh"
#include "SegBase.hh"
#include "PreFilter.hh"
#include "MPSeg.hh"
#include "HMMSeg.hh"

namespace jieba {
    class MixSeg: public SegBase {
    /* MixSeg = MPSeg + HMMSeg */
    public:
        /* 尤其要注意 mp_inst & hmm_inst 生存期必须大于 MixSeg对象生存期 */
        MixSeg(const MPSeg* mp_ptr=NULL, const HMMSeg* hmm_ptr=NULL): \
            /* 基类初始化 */ SegBase(),
            /* 派生初始化 */ _mp_seg(mp_ptr), _hmm_seg(hmm_ptr) {};
        ~MixSeg() {}
        void cut(vector<Unicode>& res, const Unicode& sent, bool hmm);
        void cut(vector<Unicode>& res, \
                 typename Unicode::const_iterator& begin, typename Unicode::const_iterator& end, bool hmm);
    private:
        const MPSeg*  _mp_seg; // 这里到底是用成员还是引用？如果是引用，一旦外面析构了，成员就不合法了
        const HMMSeg* _hmm_seg;
    };
}

#endif //NLPCPP_MIXSEG_H
