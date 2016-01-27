/*
 * c_abstract_node.h
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#ifndef INCLUDE_CONSTRUCTION_C_ABSTRACT_NODE_H_
#define INCLUDE_CONSTRUCTION_C_ABSTRACT_NODE_H_

#include <bits/stdc++.h>
using namespace std;

class CAbstractNode {
 public:
    CAbstractNode();
    CAbstractNode(int32_t id, int8_t len, bool leaf=false);
    virtual ~CAbstractNode();

    int32_t id;
    int8_t len; // no need for more - kmer
    bool leaf;


};


#endif /* INCLUDE_CONSTRUCTION_C_ABSTRACT_NODE_H_ */
