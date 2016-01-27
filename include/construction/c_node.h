/*
 * c_node.h
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#ifndef INCLUDE_C_NODE_H_
#define INCLUDE_C_NODE_H_

#include <construction/c_abstract_node.h>
#define SIGMA 4

class CNode : public CAbstractNode {
 public:
    CNode(int32_t id, int8_t len);
    virtual ~CNode();

    CAbstractNode* children[SIGMA];
};

#endif /* INCLUDE_C_NODE_H_ */
