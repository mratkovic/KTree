/*
 * c_node.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#include <construction/c_node.h>

CNode::CNode(int32_t id_, int8_t len_, bool leaf_)
        : id(id_),
          len(len_),
          leaf(leaf_) {

}

CNode::~CNode() {
    if(!leaf) {
        CInnerNode* node = (CInnerNode*) this;
        delete node;
    }
}

CInnerNode::CInnerNode(int32_t id, int8_t len)
        : CNode(id, len, false) {
    for (int i = 0; i < SIGMA; ++i) {
        children[i] = NULL;
    }
}

CInnerNode::~CInnerNode() {
    for (int i = 0; i < SIGMA; ++i) {
        if(children[i] != NULL) {
            delete children[i];
        }
    }
}

CLeaf::CLeaf(int32_t id, int8_t len)
        : CNode(id, len, true) {
}

CLeaf::CLeaf(int32_t id, int8_t len, int32_t pos_)
        : CLeaf(id, len) {
    pos.push_back(pos_);
}

CLeaf::~CLeaf() {
}

