//
// Created by System Administrator on 10/15/15.
//

#ifndef NLPCPP_DICTTREE_H
#define NLPCPP_DICTTREE_H

#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

#include "GeneralConfig.hh"
#include "../../data_type.hh"
#include "../../io/read.hh"
#include "../../utils/string_util.hh"
#include "../../dsa/graph/Trie.hh"

using namespace std;

namespace jieba {

        const size_t DICT_COLUMN_NUM =3;      // 字典文件默认行数
        const string UNKNOWN_TAG ="";         // 字典词默认tag


        struct DictUnit { // 定义Trie节点值类型
            DictUnit(): word(0,0), weight(MINDOUBLE), tag("") {};
            Unicode word;  //单个unicode
            float  weight;
            string tag;
        };

        struct DAG { // 有向无环图，用于最大概率成词
            DAG(): pinfo(NULL), nextPos(0), logprob(jieba::MINDOUBLE), word(0) {}; // word, nexts默认构造
            const DictUnit* pinfo; // 最大概率成词点所在位置
            size_t nextPos;      // 下一个dag在句子中的起始位置
            double logprob;      // 最大log概率
            uint16_t word;       // 不在字典中的字
            vector<pair<size_t, const DictUnit*> > nexts; // pair(位置，字典单元)
        };

        // Unicode树，基类，用于继承，便于后面拓展类成员
        typedef graph::Trie<uint16_t, DictUnit> DictTrieBase; // 注意：key类型必须是uint16_t，不能是Unicode这样的vector
        class DictTrie: public DictTrieBase {
        public:
            enum UsrWordWeightOption { // 用户词典freq默认设置选项
                WordWeightMin,
                WordWeightMedian,
                WordWeightMax,
            };
        public:
            DictTrie(const string& default_dict_path, const string& usr_dict_path, \
                     const UsrWordWeightOption option=WordWeightMin /* 默认是最小权重 */): \
             /* 基类初始化 */   DictTrieBase(),
             /* 派生类初始化 */ _default_dict_path(default_dict_path), _usr_dict_path(usr_dict_path)
             /* 建树 */       {initDictTree(option);}
            ~DictTrie() {
              //  在DictTree中无new操作，所以不用写析构
            } //  每个派生类只负责自己的析构，由编译器自动调用基类析构，顺序：派生类 -> 基类

            double GetMinWeight() const; // 节点不存在时，最小权重；

            /* 句子里[begin, end)内词可以在dicttrie里面找到的dag */
            void genDAG(vector<DAG>& res, Unicode::const_iterator& begin, Unicode::const_iterator& end) const;


        private:
            string _default_dict_path; // 默认字典路径
            string _usr_dict_path;     // 用户字典路径

            vector<DictUnit> _static_node_infos;
            double _usr_word_default_weight;
            double _min_weight;

            void loadDict(const string& dict_path, double default_weight=0.0, const string& default_tag=jieba::UNKNOWN_TAG);
            void initDictTree(const UsrWordWeightOption option); // 初始化Trie
            void normWeight(); // 对所有节点weight标准化
            static bool unit_cmp(const DictUnit& lhs, const DictUnit& rhs); // DictUnit间cmp方法
            void setUserDictDefaultWeight(const UsrWordWeightOption option); // 设置user字典默认权重
            void loadUserDict(const string& usr_dict_path); // 加载user字典

        };
};

#endif //NLPCPP_DICTTREE_H
