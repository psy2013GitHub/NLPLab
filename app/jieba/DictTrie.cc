//
// Created by System Administrator on 10/15/15.
//

#include "DictTrie.hh"


namespace jieba {

    double DictTrie::GetMinWeight() const {
        return _min_weight;
    }

    void DictTrie::initDictTree(const UsrWordWeightOption option) {
        // 加载字典
        loadDict(_default_dict_path);
        normWeight();
        setUserDictDefaultWeight(option);
        loadUserDict(_usr_dict_path);
        // 建Trie
        vector<Unicode> keys; vector<DictUnit*> vals;
        for (size_t i=0; i<_static_node_infos.size(); ++i) {
            keys.push_back(_static_node_infos[i].word);
            vals.push_back(&_static_node_infos[i]);
        }
        CreateTrie(keys, vals); // CreateTrie()继承自基类
    }

    void DictTrie::loadDict(const string& dict_path, \
                double default_weight, const string& default_tag) {
        /* 加载默认词典 */
        printf("loadDict path: %s\n", dict_path.c_str());
        read::document doc(dict_path, "r");
        if (!doc.is_Open()) {
            printf("can't open %s\n", _default_dict_path.c_str());
            return;
        }
        string line;
        vector<string> buf;
        size_t lineno = 0;
        DictUnit unit;
        while (!doc.is_EOF()) {
             ++lineno;
             doc.readLine(line);
             //printf("line: %s\n", line.c_str());
             if (line.size()>0) {
                 string_util::split(buf, line, string(" "));
                 if (buf.size() < DICT_COLUMN_NUM) {
                     printf("split result illegal, line: %ld, line: %s, split size: %ld\n", lineno, line.c_str(), buf.size());
                     return;
                 }
                 //printf("buf[0]: %s\n", buf[0].c_str());
                 string_util::utf82unicode(unit.word, buf[0], buf[0].size());
                 string tmp;
                 string_util::unicode2utf8(tmp, unit.word);
                 //printf("unit.word: %s\n", tmp.c_str());
                 unit.weight = (1<buf.size()||!buf[1].size()) ? atof(buf[1].c_str()) : default_weight;
                 unit.tag    = (2<buf.size()||!buf[2].size()) ? buf[2] : default_tag;
                 _static_node_infos.push_back(unit); // push_back是深复制，所以不用担心unit在出栈时消失

                 buf.clear();
             }
        }
        doc.close();
    }

    void DictTrie::normWeight() { /* 归一化 & log */
        if (!_static_node_infos.size())
            printf("empty nodes\n");
        double sum = 0.0;
        for (size_t i=0; i<_static_node_infos.size(); ++i) {
            sum += _static_node_infos[i].weight;
        }
        for (size_t i=0; i<_static_node_infos.size(); ++i) {
            _static_node_infos[i].weight = log(double(_static_node_infos[i].weight)/sum);
        }
    }

    bool DictTrie::unit_cmp (const DictUnit& lhs, const DictUnit& rhs) {
        return lhs.weight < rhs.weight;
    }
    void DictTrie::setUserDictDefaultWeight(const UsrWordWeightOption option) {
        if (!_static_node_infos.size())
            printf("empty nodes\n");
        vector<DictUnit> x = _static_node_infos;
        std::sort(x.begin(), x.end(), unit_cmp);
        _min_weight = x[0].weight;
        switch (option) {
            case WordWeightMin:
                _usr_word_default_weight = x[0].weight;
                return;
            case WordWeightMedian:
                _usr_word_default_weight = x[x.size()/2].weight;
                return;
            case WordWeightMax:
                _usr_word_default_weight = x[x.size()-1].weight;
                return;
            default:
                printf("Unknown method %d\n", option);
                return;
        }
    }

    void DictTrie::loadUserDict(const string& usr_dict_path) {
        /* 记载用户定义字典 */
        if (!usr_dict_path.size())
            return;
        vector<string> files;
        string delim(":"); string_util::split(files, usr_dict_path, delim);
        for (size_t i = 0; i < files.size(); ++i) {
            loadDict(files[i], _usr_word_default_weight, UNKNOWN_TAG);
        }
    }

    void DictTrie::genDAG(vector<DAG>& res, \
           typename Unicode::const_iterator& begin, typename Unicode::const_iterator& end) const {
        /* 获得句子的有向无环图 */
        //printf("Trie::genDAG\n");
        assert(begin <= end);
        res.clear();
        res.resize(end-begin); // resize调整大小
        const TrieNode* node_ptr;
        string tmp;
        for (typename Unicode::const_iterator  iter_i = begin; iter_i!=end; ++iter_i) { // 循环每个字，
            string_util::unicode2utf8(tmp, Unicode(1,*iter_i));
            //printf("DictTrie word: %s ", tmp.c_str());
            node_ptr = _base + *iter_i;
            /* 首先，与自己本身肯定能组词 */
            res[iter_i-begin].nexts.clear();
            //printf("初始大小: %ld ",  res[iter_i-begin].nexts.size());
            res[iter_i-begin].nexts.push_back(pair<size_t, const DictUnit*>(size_t(iter_i - begin), node_ptr->value_ptr)); // 值得注意的是，sent[i]可能为新字 <=> 这里node_ptr可能为NULL !
            //printf("新大小: %ld ",  res[iter_i-begin].nexts.size());
            if (node_ptr->value_ptr==NULL) {
                // 如果字在DictTrie中不存在
                res[iter_i-begin].word = *iter_i;
                //printf("字典中找不到, ");
                continue;
            }
            if (node_ptr->nextMap==NULL) { // 它与其他任何字都不能成词
                //printf("空nextMap ");
                continue;
            }
            /* 其次，寻找句子中可能与其成词的字 */
            for (typename Unicode::const_iterator iter_j = iter_i+1; \
                             node_ptr!=NULL && node_ptr->nextMap!=NULL && iter_j!=end; \
                           ++iter_j) {
                //printf("genDAG node_ptr=NULL: %d\n", node_ptr==NULL);
                TrieNode::TrieMap::const_iterator map_iter;
                map_iter = node_ptr->nextMap->find(*iter_j);
                string_util::unicode2utf8(tmp, Unicode(1,*iter_j));
                if (map_iter == node_ptr->nextMap->end()) {
                    //printf("nextMap中没找到 %s ", tmp.c_str());
                    break;
                }
                //printf("nextMap中找到 %s ", tmp.c_str());
                //printf("---------\n");
                res[iter_i-begin].nexts.push_back(pair<size_t, const DictUnit*>(iter_j - begin, map_iter->second->value_ptr));
                //printf("新大小: %ld ",  res[iter_i-begin].nexts.size());
                node_ptr = map_iter->second;
            }
            //printf("\n");
        }
        /*printf("dags 大小 ");
        for (size_t i=0; i<res.size(); ++i)
            printf("%ld: %ld \n", i, res[i].nexts.size());
            */
    }
}
