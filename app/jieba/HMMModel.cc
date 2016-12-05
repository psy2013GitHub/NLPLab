//
// Created by System Administrator on 10/16/15.
//

#include "HMMModel.hh"

namespace jieba {

    bool HMMModel::load_model(const string &model_path) {
        read::document doc(model_path, "r");
        if(!doc.is_Open()) {
            printf("Can't open %s\n", model_path.c_str());
            return false;
        }

        while (!doc.is_EOF()) {
            vector<string> buf;
            string line;
            doc.readLine(line);
            /* 读start_prob */
            if (string_util::startswith(line, string("#prob_start"))) {
                if (!doc.is_EOF())
                    doc.readLine(line);
                else {
                    printf("Invalid model file while reading prob_start\n");
                    return false;
                }
                string_util::split(buf, line, string(" "));
                if (buf.size()!=STATENUM) {
                    printf("Invalid model file while reading prob_start\n");
                    return false;
                }
                for(size_t i=0; i<STATENUM; ++i) {
                    _start_p[i] = atof(buf[i].c_str());
                    //printf("_start_p[i] == %g\n", _start_p[i]);
                }
                buf.clear();
                line.clear();
                continue;
            }
            /* 读trans_prob */
            if (string_util::startswith(line, string("#prob_trans"))) {
                for (size_t i = 0; i < STATENUM; ++i) {
                    if (!doc.is_EOF())
                        doc.readLine(line);
                    else {
                        printf("Invalid model file while reading prob_trans\n");
                        return false;
                    }
                    string_util::split(buf, line, string(" "));
                    if (buf.size() != STATENUM) {
                        //printf("Invalid model file while reading prob_trans\n");
                        return false;
                    }
                    for (size_t j = 0; j < STATENUM; ++j) {
                        _trans_p[i][j] = atof(buf[j].c_str());
                        //printf("_trans_p[%ld][%ld] == %g\n", i, j, _trans_p[i][j]);
                    }
                    buf.clear();
                    line.clear();
                }
                continue;
            }
            /* 读emit_prob */
            Unicode uword;
            vector<string> word_prob_pair;
            if (string_util::startswith(line, string("#prob_emit"))) {
                for (size_t i = 0; i < STATENUM; ) {
                    if (!doc.is_EOF())
                        doc.readLine(line);
                    else {
                        //printf("Invalid model file while reading prob_emit\n");
                        return false;
                    }
                    if (string_util::startswith(line, string("#"))) // 跳过＃行，i不变
                        continue;
                    //printf("line: %s\n", line.c_str());
                    string_util::split(buf, line, string(","));
                    //printf("buf.size() = %ld\n", buf.size());
                    for (size_t j=0; j<buf.size(); ++j) {
                        //printf("buf[%ld]: %s\n", j, buf[j].c_str());
                        string_util::split(word_prob_pair, buf[j], string(":"));
                        if (word_prob_pair.size() != 2) {
                            printf("word_prob_pair.size() = %ld\n", word_prob_pair.size());
                            for (size_t k=0; k<word_prob_pair.size(); ++k)
                                printf("word_prob_pair[%ld] = %s \n", k, word_prob_pair[k].c_str());
                            printf("Invalid model file while reading prob_emit\n");
                            return false;
                        }
                        string_util::utf82unicode(uword, word_prob_pair[0], word_prob_pair[0].size());
                        switch (i) {
                            case B:
                                emitProbB[uword[0]] = atof(word_prob_pair[1].c_str()); // 跳过'：'
                                break;
                            case E:
                                emitProbE[uword[0]] = atof(word_prob_pair[1].c_str()); // 跳过'：'
                                break;
                            case M:
                                emitProbM[uword[0]] = atof(word_prob_pair[1].c_str()); // 跳过'：'
                                break;
                            case S:
                                emitProbS[uword[0]] = atof(word_prob_pair[1].c_str()); // 跳过'：'
                                break;
                        }
                        //if (i==0)
                        //    printf("%ld, word_prob_pair[%d]:%s, value: %s\n", i, 0, word_prob_pair[0].c_str(), word_prob_pair[1].c_str());
                        /* 为什么用 emitProbVec[i]->operator[](uword[0]) = atof(word_prob_pair[1].c_str()); 不行*/
                    }
                    ++i;
                    buf.clear();
                    line.clear();
                }
                continue;
            }
        }
        doc.close();
        return true;
    }

    double HMMModel::getEmitProb(const EmitProbMap* ptMp, uint16_t key,
                       double defVal) const { // 防止const EmitProbMap对象来调用，编译
        EmitProbMap::const_iterator cit = ptMp->find(key);
        if (cit == ptMp->end()) { // 如果emit概率里不存在该词，那么返回defVal
            return defVal;
        }
        return cit->second;
    }

}