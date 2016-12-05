//
// Created by System Administrator on 10/16/15.
//

#ifndef NLPCPP_HMMMODEL_H
#define NLPCPP_HMMMODEL_H

#include <string>

#include "../../io/read.hh"
#include "../../data_type.hh"
#include "../../utils/string_util.hh"

using namespace std;

namespace jieba {

    /* 用于从文件中load模型 */


    enum state {B=0, E=1, M=2, S=3};
    const size_t STATENUM=4;
    class HMMModel {
    public:
        typedef unordered_map<uint16_t, double> EmitProbMap;

    public:
        HMMModel(const string& model_path): _model_path(model_path) {
            emitProbVec.push_back(&emitProbB);
            emitProbVec.push_back(&emitProbE);
            emitProbVec.push_back(&emitProbM);
            emitProbVec.push_back(&emitProbS);
            load_model(model_path);
        };
        ~HMMModel() {
            emitProbVec[0] = NULL;
            emitProbVec[1] = NULL;
            emitProbVec[2] = NULL;
            emitProbVec[3] = NULL;
        }

        double getEmitProb(const EmitProbMap* ptMp, uint16_t key,
                           double defVal) const;
    public:
        const string _model_path;
        double _start_p[STATENUM];
        double _trans_p[STATENUM][STATENUM];
        /* EmitProbMap _emit_p[STATENUM]; 这样表述不行，因为C的方式已经固定了大小了，所以会Segment Fault*/
        EmitProbMap emitProbB;
        EmitProbMap emitProbE;
        EmitProbMap emitProbM;
        EmitProbMap emitProbS;
        vector<EmitProbMap* > emitProbVec; // 不用自己释放内存
        bool load_model(const string& model_path);
    };

}


#endif //NLPCPP_HMMMODEL_H
