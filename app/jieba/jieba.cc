//
// Created by System Administrator on 10/15/15.
//

#include "jieba.hh"

namespace jieba {

    void Jieba::cut(vector<Unicode>& res, const string &sent, const cut_option option) {

        Unicode usent;
        string_util::utf82unicode(usent, sent, sent.size());
        //printf("usent.size(): %ld\n", usent.size());

        switch (option) {
            case HMM: // _mix_seg
                _mix_seg->cut(res, usent, true);
                break;
            case NO_HMM: // mp_seg
                _mp_seg->cut(res, usent);
                break;
            default:
                printf("Error: no cut method named %d\n", option);
        }
    }

}