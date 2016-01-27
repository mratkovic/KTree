/*
 * c_node.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#include <construction/c_node.h>

CNode::CNode(int32_t id, int8_t len)
        : CAbstractNode(id, len, false) {
    for (int i = 0; i < SIGMA; ++i) {
        children[i] = NULL;
    }
}

CNode::~CNode() {
    for (int i = 0; i < SIGMA; ++i) {
        if(children[i] != NULL) {
            delete children[i];
        }
    }
}
