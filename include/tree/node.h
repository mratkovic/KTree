/*
 * node.h
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#ifndef INCLUDE_TREE_NODE_H_
#define INCLUDE_TREE_NODE_H_

#include <bits/stdc++.h>

#define ID id
#define LEN len

#define FIRST x
#define LAST y

#define CHILD x
#define MASK y


class Node {
 public:
    Node(int32_t id, int8_t len);
    Node();
    ~Node();

    int32_t id;
    int8_t len;

    int32_t x; // first position or child
    int32_t y; // last position or mask;

};

#endif /* INCLUDE_TREE_NODE_H_ */
