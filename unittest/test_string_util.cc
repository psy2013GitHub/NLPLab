//
// Created by System Administrator on 10/17/15.
//
#include <cstdio>
#include <string>
#include "../config.hh"
#include "../data_type.hh"
#include "../io/read.hh"
#include "../utils/string_util.hh"
#include "../app/jieba/Jieba.hh"

using namespace std;

int main () {
    const string test_file("../test.utf8");
    read::document doc(test_file, "r");
    string line;
    doc.readLine(line);
    printf("sent:\n%s\n", line.c_str());
    string delim;
    doc.readLine(delim);
    /* 测试split */
    vector<string> pieces;
    string_util::split(pieces, line, delim);
    for (size_t i=0; i<pieces.size(); ++i)
        printf("split seg: \ni: %ld, %s\n", i, pieces[i].c_str());
    /* 测试join */
    string_util::join(line, pieces, delim, 0, pieces.size());
    printf("join:\n%s\n", line.c_str());
    /* 测试isIn */
    bool tmp = string_util::isIn(line, delim, 0, line.size());
    printf("isIn:\n%s isIn %s = bool %d\n", delim.c_str(), line.c_str(), tmp);
    /* 测试startswith */
    tmp = string_util::startswith(line, delim);
    printf("startswith:\n%s startswith %s = bool %d\n", line.c_str(), delim.c_str(), tmp);
    /* 测试endswith */
    tmp = string_util::endswith(line, delim);
    printf("endswith:\n%s endswith %s = bool %d\n", line.c_str(), delim.c_str(), tmp);
    /* 测试utf82unicode */
    Unicode res;
    string_util::utf82unicode(res, line, line.size());
    printf("utf82unicode: \n");
    for (size_t i=0; i<res.size(); ++i)
    printf("%.4x ", res[i]);
    printf("\n");
    /* 测试unicode2utf8 */
    string_util::unicode2utf8(line, res);
    printf("unicode2utf8: \n%s\n", line.c_str());
    /* 测试jieba分词 */
    string default_dict_path("../app/jieba/dict/jieba.dict.utf8");
    string usr_dict_path("");
    string hmm_model_path("../app/jieba/dict/hmm_model.utf8");
    jieba::Jieba jieba_inst(default_dict_path, usr_dict_path, hmm_model_path);
    vector<Unicode> cutRes;
    jieba_inst.cut(cutRes, line);
    string tmp_cut;
    string cut_res;
    for (size_t i=0; i<cutRes.size(); ++i) {
        tmp_cut.clear();
        string_util::unicode2utf8(tmp_cut, cutRes[i]);
        cut_res.push_back('/');
        cut_res.append(tmp_cut.begin(), tmp_cut.end());
    }
    printf("jieba.cut: %s\n", cut_res.c_str());

};
