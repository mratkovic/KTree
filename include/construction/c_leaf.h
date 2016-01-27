/*
 * c_leaf.h
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#ifndef INCLUDE_C_LEAF_H_
#define INCLUDE_C_LEAF_H_

#include <construction/c_abstract_node.h>

class CLeaf : public CAbstractNode{
 public:
    CLeaf(int32_t id, int8_t len);
    virtual ~CLeaf();

    vector<int32_t> pos;
};

#endif /* INCLUDE_C_LEAF_H_ */
