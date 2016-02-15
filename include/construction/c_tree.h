/*
 * c_tee.h
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#ifndef INCLUDE_C_TREE_H_
#define INCLUDE_C_TREE_H_

#include <construction/c_node.h>

class CTree {
 public:
    CTree(const char* txt, int32_t len, const char* shape, int8_t kmerLen);
    ~CTree();
    void insert(int32_t start, int32_t len);

    CNode* root();

    string toDot();
    void saveToDotFile(string path);

    int32_t nodeCnt;
    const char* txt;
    int32_t len;
    int8_t kmerLen;
    const char* shape;

 private:
    CNode* insert(CNode* node, int32_t start, int32_t len, int32_t pos, int8_t depth);
    CInnerNode* splitNode(CNode* node, int32_t i);
    int32_t diffIdx(const char* word1, const char* word2, int32_t len, int8_t depth);
    void dfs(CNode *x, std::ostream& out, int8_t depth);
    CNode* root_;
};

#endif /* INCLUDE_C_TREE_H_ */
