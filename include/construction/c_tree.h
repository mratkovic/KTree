/*
 * c_tee.h
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#ifndef INCLUDE_C_TREE_H_
#define INCLUDE_C_TREE_H_

#include <construction/c_abstract_node.h>

class CTree {
 public:
    CTree(char* txt, int32_t len);
    virtual ~CTree();
    void insert(int32_t start, int32_t len);

    int32_t nodeCnt;
 private:
    CAbstractNode* insert(CAbstractNode* node, int32_t start, int32_t len, int32_t pos);

    CAbstractNode* root_;
    char* txt;
    int32_t len;

};

#endif /* INCLUDE_C_TREE_H_ */
