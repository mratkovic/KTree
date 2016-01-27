/*
 * c_abstract_node.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#include <construction/c_abstract_node.h>

CAbstractNode::CAbstractNode(int32_t id_, int8_t len_, bool leaf_)
        : id(id_),
          len(len_),
          leaf(leaf_) {

}

CAbstractNode::CAbstractNode() {

}

CAbstractNode::~CAbstractNode() {
}
