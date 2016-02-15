/*
 * c_node.h
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#ifndef INCLUDE_C_NODE_H_
#define INCLUDE_C_NODE_H_

#include <bits/stdc++.h>
using namespace std;

class CNode {
 public:
    CNode();
    CNode(int32_t id, int8_t len, bool leaf = false);
    ~CNode();
    static const int8_t SIGMA = 4;

    int32_t id;
    int8_t len;  // no need for more - kmer
    bool leaf;

};

class CInnerNode : public CNode {
 public:
    CInnerNode(int32_t id, int8_t len);
    ~CInnerNode();

    CNode* children[SIGMA];
};

class CLeaf : public CNode {
 public:
    CLeaf(int32_t id, int8_t len);
    CLeaf(int32_t id, int8_t len, int32_t pos);
    ~CLeaf();
    vector<int32_t> pos;
};

#endif /* INCLUDE_C_NODE_H_ */
