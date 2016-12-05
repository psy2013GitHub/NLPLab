//
// Created by System Administrator on 10/12/15.
//

#include <vector>
#include <string>

using namespace std;

template <class Iter, class ValContainer>
void kmp(Iter& begin, Iter& end, const ValContainer& pat, vector<Iter>& res) {
    vector<Iter> next_table = gen_next(pat);
    int j=0;
    for (int i=0; begin+i<end; ) {
        if (j==-1 || *(begin+i)==*(pat.begin()+j)) {
            ++i;
            ++j;
        }
        else
            j = next[j];
        if (j==pat.size()-1) {
            i+=j;
            j=0;
            res.push_back(begin+i);
            break;
        }
    }
}

template <class ValContainer>
vector<int>& gen_next(ValContainer& pattern) {
    // next[j]表示第j个元素前面（不包括j，如果包括j，那么移动后仍然不匹配）对称串的长度
    vector<int> next;
    /* next[0]=-1 */int len_j = -1; next.push_back(len_j);
    for (int j=0; j<pattern.size()-1; ) {
        /* 对于第j+1个位置 next[j+1] */
        if (len_j == -1 || pattern[len_j] == pattern[j]) { // make sure []，operator== is supported
            next[++j] = ++len_j;
        }
        else {
            len_j = next[len_j];
        }
    }
    return next;
}