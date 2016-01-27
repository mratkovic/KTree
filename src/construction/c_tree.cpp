/*
 * c_tree.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#include <construction/c_tree.h>
#include <construction/c_leaf.h>
#include <construction/c_node.h>

CTree::CTree(char* txt_, int32_t len_)
        : txt(txt_),
          len(len_) {
    root_ = NULL;
    nodeCnt = 0;
}

CTree::~CTree() {
    if(root_) {
        delete root_;
    }
}

void CTree::insert(int32_t start, int32_t len) {
    root_ = insert(root_, start, len, start);
}

int32_t diffId(const char* word1, const char* word2, int32_t len) {
    for (int32_t i = 0; i < len; ++i, ++word1, ++word2) {
        if(*word1 != *word2) {
            return i;
        }
    }
    return len;

}

CAbstractNode* CTree::insert(CAbstractNode* node, int32_t start, int32_t len,
                             int32_t pos) {
    int32_t mini = min<int32_t>(len, node->len);
    int32_t diff = diffId(txt + start, txt + node->id, mini);

    if(diff == mini) {
        // total match
        if(!node->leaf) {
            // inner node

        } else {
            CLeaf* leaf = (CLeaf*) node;
            leaf->pos.push_back(pos);
            return leaf;
        }

    } else {
        // split on inner
        nodeCnt += 2;
        CLeaf* leaf = new CLeaf(start + diff, len - diff);
        CNode* parent = new CNode(node->id, diff);

        node->id += diff;
        node->len -= diff;
        // add child
    }
}

