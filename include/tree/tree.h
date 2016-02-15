/*
 * tree.h
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#ifndef INCLUDE_TREE_TREE_H_
#define INCLUDE_TREE_TREE_H_

#include <bits/stdc++.h>
#include <tree/node.h>

#include <construction/c_tree.h>
#include <construction/c_node.h>

enum Mutation {Missmatch, Insertion, Deletion};

class Tree {
 public:
    Tree(int32_t nNodes, int32_t n, char* txt, int8_t k);
    Tree(FILE* in, const char* text, int32_t textLen);
    Tree(CTree* ctree);
    ~Tree();
    static const int8_t SIGMA = 4;
    static const int8_t CHILD_MASK[];

    // debug
    void findMatches(char* read, int32_t readLen);
    void printResults(pair<int32_t, int32_t> result);


    pair<int32_t, int32_t> search(char* kmer, int8_t kmerLen, Mutation type);
    pair<int32_t, int32_t> search(Node* n, char* kmer, int8_t kmerLen, int8_t depth, Mutation type);

    string toDot();
    void saveToDotFile(string path);
    void save(FILE* out);
    void dfsToDot(int nodeId, std::ostream& out, int8_t depth);
    void dfsTransform(CNode* cnode, Node* node, int32_t& currentNode,
                      int32_t& currentIndex);

    pair<int32_t, int32_t> diffIdx(char* kmer, const char* txt, int32_t len, int8_t depth, Mutation type);
    pair<int32_t, int32_t> diffIdxMissmatch(char* kmer, const char* txt, int32_t len, int8_t depth);
    pair<int32_t, int32_t> diffIdxDel(char* kmer, const char* txt, int32_t len, int8_t depth);
    pair<int32_t, int32_t> diffIdxIns(char* kmer, const char* txt, int32_t len, int8_t depth);

    int32_t dfsFirstPos(Node* n, int8_t depth);
    int32_t dfsLastPos(Node* n, int8_t depth);

    int32_t nNodes;
    int32_t n;
    const char* txt;
    int8_t maxKmerLen;

    Node* nodes;
    int32_t* suffixArray;
    char* shape;
    int8_t wildcardCnt;

};

#endif /* INCLUDE_TREE_TREE_H_ */
