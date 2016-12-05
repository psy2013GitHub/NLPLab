//
// Created by System Administrator on 10/15/15.
//

#ifndef NLPCPP_TRIE_H
#define NLPCPP_TRIE_H


// 本质上，Trie是unordered map
#include <cassert>
#include <vector>
#include <string>
#include <deque>
#include "../../data_type.hh"
#include "../../utils/string_util.hh"

using namespace std;

namespace graph {


    template <class KeyType, class NodeType> /*
 *  KeyType，节点键类型，必须是容器，且有size操作
 *  NodeType，节点值类型，在Trie内以指针存储
 * */
    class Trie { // 实际是用unordered_map实现
    public:
    static const size_t BASE_NUM = (size_t(1) << (sizeof(KeyType)<<3));
    public:
        class TrieNode { // Trie节点实际是对unordered_map的封装
        public:
            TrieNode(): value_ptr(NULL), nextMap(NULL) {}

        public: /* 每个节点包含 value_ptr(节点值指针) + unordered_map */
            typedef unordered_map<KeyType, TrieNode*> TrieMap; // 与它链接的子Trie
            const NodeType* value_ptr;
            TrieMap*  nextMap;
        };
    public:
        typedef const TrieNode* const_TrieNode_ptr;

    public:
        Trie() {}; // 默认初始化，便于继承类对它初始化；
        Trie(const vector<KeyType>& keys, const vector<TrieNode*>& nodes) {CreateTrie(keys, nodes);};
        ~Trie() {
            for (size_t i=0; i<BASE_NUM; ++i) {
                if (!_base[i].nextMap) // 若未曾分配TrieMap，i.e. 该节点内存均在栈上，不用管。
                    continue;
                //  否则，说明存在堆上内存
                for (typename TrieNode::TrieMap::iterator iter=_base[i].nextMap->begin(); \
                       iter!=_base[i].nextMap->end(); ++iter) { // 1，删除nextMap上所有节点；
                    deleteNode(iter->second); // DeleteNode 递归删除内部node
                    iter->second = NULL; // 重置NULL
                }
                delete _base[i].nextMap; // 2，删除nextMap
                _base[i].nextMap = NULL; // 重置NULL
            }
        };

        TrieNode* find(size_t& idx, const vector<KeyType>& keys);
        void insert(const vector<KeyType>& , const NodeType* );

    protected: // 定义为protected，便于继承
       TrieNode _base[BASE_NUM];
       void CreateTrie(const vector<vector<KeyType> >& , const vector<NodeType*>& nodes);
       void deleteNode(TrieNode* node);
    };
    /* static 成员定义 */
    /* template <class KeyType, class NodeType>
    const size_t Trie::BASE_NUM = (1 << (sizeof(KeyType)<<3)); // 2^(8 * sizeof(KeyType))，采自cppjieba */


    template <class KeyType, class NodeType>
    typename Trie<KeyType, NodeType>::TrieNode* \
           Trie<KeyType, NodeType>::find(size_t& idx, const vector<KeyType>& keys) {
        /*
         * 返回key不匹配后开始位置
         * res返回不匹配节点指针（NULL或者叶子节点）
         * idx返回不匹配key位置
         * */
        assert(keys.size());
        idx = 0;
        TrieNode* node_ptr;
        node_ptr = _base + keys[0];
        if (node_ptr->nextMap==NULL) { // base node
            //printf("就在基上\n");
            return node_ptr;
        } // 到达叶子节点
        typename TrieNode::TrieMap::const_iterator iter;
        for ( ++idx; idx < keys.size(); ++idx) {
            if( node_ptr->nextMap==NULL || \
                   (iter = node_ptr->nextMap->find(keys[idx]))==node_ptr->nextMap->end())
            break;
            node_ptr = iter->second;
        }
        return node_ptr;
    }

    template <class KeyType, class NodeType>
    void Trie<KeyType, NodeType>::insert(const vector<KeyType>& keys, const NodeType* value_ptr) {
        /* 只有insert有内存分配，分别'new TrieMap'，'new TrieNode' */
        assert(keys.size());
        size_t end_idx(0);
        TrieNode* end_node = find(end_idx, keys); // 加上namespace防止与stl find混淆

        //printf("end_idx: %ld\n", end_idx);
        typename TrieNode::TrieMap::const_iterator iter;
        //printf("end_idx: %ld\n", end_idx);
        string tmp;
        string_util::unicode2utf8(tmp, keys); //

        if (end_idx==0) { // end_idx=0包含两种情况：1，基节点，第一个就不匹配；2，基节点匹配但是基节点没有nextMap
            if (end_node->value_ptr==NULL) {
                if (keys.size()==1) { // 如果keys长度为1，那么直接返回
                    end_node->value_ptr = value_ptr;
                    return;
                }
            }
            ++end_idx;
        }
        //printf("插入词: %s\n", tmp.c_str());
        for (size_t i=end_idx; i<keys.size(); ++i) { // 情况1：叶子节点；情况2：中间节点，但是找不到该词
            //printf("  轮到字: i=%ld\n", i);
            // printf("  end_node->nextMap: %d\n", (end_node->nextMap)==NULL);
            if ((end_node->nextMap)==NULL) { /* 新建nextMap，初始化NodeType */
                //printf("new TrieMap;\n");
                end_node->nextMap = new typename TrieNode::TrieMap;
            }
            if((iter = end_node->nextMap->find(keys[i]))==end_node->nextMap->end()) { // 没找到
                //printf("new TrieNode;\n");
                end_node->nextMap->operator[](keys[i]) = new TrieNode; /* 新建TrieNode */
            }
            //printf("继续插入下一个字\n");
            end_node = end_node->nextMap->operator[](keys[i]); /* 中间不插入数字，直到最后节点*/
        }
        end_node->value_ptr = value_ptr;
        return;
    };


    template <class KeyType, class NodeType>
    void Trie<KeyType, NodeType>::CreateTrie(const vector<vector<KeyType> >& keys_vec,\
                   const vector<NodeType*> &nodes) {
        assert(keys_vec.size() == nodes.size());
        assert(keys_vec.size());
        for (size_t i=0; i<keys_vec.size(); ++i) {
            //printf("word %ld\n", i);
             insert(keys_vec[i], nodes[i]);
             string tmp;
             string_util::unicode2utf8(tmp, keys_vec[i]);
             //printf("i:%ld | keys: %s\n",i, tmp.c_str());
        }
    };

    template <class KeyType, class NodeType>
    void Trie<KeyType, NodeType>::deleteNode(TrieNode* node_ptr) { // 析构方法，参照insert里'new'来'delete'
        if (!node_ptr) return;
        if (node_ptr->nextMap) {
            for (typename TrieNode::TrieMap::iterator iter = node_ptr->nextMap->begin();\
                      iter != node_ptr->nextMap->end(); ++iter) {
                deleteNode(iter->second);
                iter->second = NULL;
            }
            delete node_ptr->nextMap;
            node_ptr->nextMap = NULL;
        }
        delete node_ptr;
    };
}



#endif //NLPCPP_TRIE_H
