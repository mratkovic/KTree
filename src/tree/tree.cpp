/*
 * tree.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#include <tree/tree.h>
#include <construction/c_node.h>
#include <util/utility_functions.h>
#include <bioinf/bioutil.h>

Tree::Tree(int32_t nNodes_, int32_t n_, char* txt_, int8_t kmerLen_)
        : nNodes(nNodes_),
          n(n_),
          txt(txt_),
          maxKmerLen(kmerLen_) {
    nodes = new Node[nNodes];
    suffixArray = new int32_t[n];

    shape = new char[maxKmerLen + 1];
    for (int i = 0; i < maxKmerLen; ++i)
        shape[i] = '0';
    shape[maxKmerLen] = 0;
    wildcardCnt = 0;
}

const int8_t Tree::CHILD_MASK[] = { 0, 0b1, 0b11, 0b111 };

Tree::~Tree() {
    if(nodes) delete nodes;
    if(shape) delete shape;
}
Tree::Tree(CTree* ctree) {
    // copy data
    maxKmerLen = ctree->kmerLen;
    nNodes = ctree->nodeCnt;
    n = ctree->len;
    txt = ctree->txt;

    // init arrays
    nodes = new Node[nNodes];
    suffixArray = new int32_t[n];

    wildcardCnt = 0;
    shape = new char[maxKmerLen + 1];
    for (int i = 0; i < maxKmerLen; ++i){
        shape[i] = ctree->shape[i];
        wildcardCnt += (ctree->shape[i] == '1');
    }

    shape[maxKmerLen] = 0;

    int32_t currentNode = 1, currentIndex = 0;
    dfsTransform(ctree->root(), &nodes[0], currentNode, currentIndex);

}

inline pair<int32_t, int32_t> Tree::diffIdx(char* kmer, const char* txt,
                                            int32_t len, int8_t depth,
                                            Mutation type) {
    switch (type) {
        case Insertion:
            return diffIdxIns(kmer, txt, len, depth);
        case Deletion:
            return diffIdxDel(kmer, txt, len, depth);
        case Missmatch:
            return diffIdxMissmatch(kmer, txt, len, depth);
        default:
            throw "Invalid mutation";
    }
}

// only for Missmatch
inline pair<int32_t, int32_t> Tree::diffIdxMissmatch(char* kmer,
                                                     const char* txt,
                                                     int32_t len,
                                                     int8_t depth) {
    for (int32_t i = 0; i < len; ++i, ++kmer, ++txt) {
        if(i + depth < maxKmerLen && shape[i + depth] == '1') continue;
        if(*kmer != *txt) {
            return {i, i};
        }
    }
    return {len, len};
}

// only for delete
inline pair<int32_t, int32_t> Tree::diffIdxDel(char* kmer, const char* txt,
                                               int32_t len, int8_t depth) {
    int i = 0, j = 0;
    for (; i < len; ++i) {
        if(i + depth < maxKmerLen && shape[i + depth] == '1') continue;
        if(kmer[j] != txt[i]) {
            return {j, i};
        }
        ++j;
    }
    return {j, i};
}

// only for insert
inline pair<int32_t, int32_t> Tree::diffIdxIns(char* kmer, const char* txt,
                                               int32_t len, int8_t depth) {
    int i = 0, j = 0;
    for (; i < len; ++i, ++j) {
        if(i + depth < maxKmerLen && shape[i + depth] == '1') {
            ++j;
            continue;
        }
        if(kmer[j] != txt[i]) {
            return {j, i};
        }
    }
    return {j, i};
}

inline pair<int32_t, int32_t> Tree::search(char* kmer, int8_t kmerLen,
                                           Mutation type) {
    return search(&nodes[0], kmer, kmerLen, 0, type);
}

pair<int32_t, int32_t> Tree::search(Node* n, char* kmer, int8_t kmerLen,
                                    int8_t depth, Mutation type) {
    if(n == NULL) {
        throw "Search ended in null??";
    }

    int32_t mini = min<int32_t>(kmerLen, n->len);
    int32_t kmerIndex, refIndex;
    tie(kmerIndex, refIndex) = diffIdx(kmer, txt + n->id, mini, depth,
                                       type);

    if(refIndex != mini) {  // no match
        return {-1, -1};
    }

    if(kmerIndex == kmerLen) {  // part off inner
        if(depth + mini == maxKmerLen) {  // leaf
            return {n->FIRST, n->LAST};
        } else {
            return {dfsFirstPos(n, depth), dfsLastPos(n, depth)};
        }

    } else if(refIndex == n->len) {  // total
        if(depth + mini == maxKmerLen) {  // leaf
            return {n->FIRST, n->LAST};

        } else {  // find child
            int8_t idx = bioinf::baseToInt(kmer[kmerIndex]);
            if(n->MASK & (1 << idx)) {
                idx = __builtin_popcountll(n->MASK & CHILD_MASK[idx]);
                return search(&nodes[n->CHILD + idx], kmer+kmerIndex, kmerLen - kmerIndex,
                              depth + mini, type);

            } else {  // no child
                return {-1, -1};
            }
        }
    } else {  // debug
        throw "Wrong diff? Should never happen";

    }
}

int32_t Tree::dfsFirstPos(Node* n, int8_t depth) {
    while (n->len + depth < maxKmerLen) {
        depth += n->len;
        n = &nodes[n->CHILD];
    }
    return n->FIRST;
}
int32_t Tree::dfsLastPos(Node* n, int8_t depth) {
    while (n->len + depth < maxKmerLen) {
        depth += n->len;
        int cnt = __builtin_popcountll(n->MASK & 0xFF);
        n = &nodes[n->CHILD + cnt - 1];
    }
    return n->LAST;
}

void Tree::dfsTransform(CNode* cnode, Node* node, int32_t& currentNode,
                        int32_t& currentIndex) {
    if(cnode == NULL) return;

    if(cnode->leaf) {
        CLeaf* leaf = (CLeaf*) cnode;

        node->id = leaf->id;
        node->len = leaf->len;

        node->FIRST = currentIndex;
        for (int32_t pos : leaf->pos) {
            suffixArray[currentIndex++] = pos;
        }
        node->LAST = currentIndex - 1;

    } else {
        CInnerNode* cInner = (CInnerNode*) cnode;

        node->id = cInner->id;
        node->len = cInner->len;

        node->MASK = 0;
        int8_t cCnt = 0;
        for (int i = 0; i < CNode::SIGMA; ++i) {
            if(cInner->children[i]) {
                node->MASK |= 1 << i;
                cCnt++;
            }
        }
        int32_t chNodeId = currentNode;
        currentNode += cCnt;
        node->CHILD = chNodeId;
        for (int i = 0; i < CNode::SIGMA; ++i) {
            if(cInner->children[i]) {
                Node* cn = &nodes[chNodeId++];
                dfsTransform(cInner->children[i], cn, currentNode,
                             currentIndex);
            }
        }

    }
}
void Tree::save(FILE* out) {
    fwrite(&n, sizeof(n), 1, out);
    fwrite(&nNodes, sizeof(nNodes), 1, out);
    fwrite(&maxKmerLen, sizeof(maxKmerLen), 1, out);
    fwrite(shape, sizeof(shape[0]), maxKmerLen, out);

    fwrite(nodes, sizeof(Node), nNodes, out);
    fwrite(suffixArray, sizeof(suffixArray[0]), n, out);

}

Tree::Tree(FILE* in, const char* text, int32_t textLen) {
    fread(&n, sizeof(n), 1, in);
    fread(&nNodes, sizeof(nNodes), 1, in);
    fread(&maxKmerLen, sizeof(maxKmerLen), 1, in);

    assert(n == textLen);

    shape = new char[maxKmerLen + 1];
    shape[maxKmerLen] = 0;
    txt = text;
    nodes = new Node[nNodes];
    suffixArray = new int32_t[n];

    fread(shape, sizeof(shape[0]), maxKmerLen, in);
    fread(nodes, sizeof(Node), nNodes, in);
    fread(suffixArray, sizeof(suffixArray[0]), n, in);

    wildcardCnt = 0;
    for (int i = 0; i < maxKmerLen; ++i){
        wildcardCnt += (shape[i] == '1');
    }
}

////////////////////////////////////////////////////////////////////
// for debug
void Tree::dfsToDot(int nodeId, std::ostream& out, int8_t depth) {
    Node* n = &nodes[nodeId];
    char str[512];
    memset(str, 0, 512);
    string label(txt + n->id, n->len);
    for (int i = 0; i < maxKmerLen; ++i) {
        if(shape[i] == '1') {
            int id = i - depth;
            if(id > 0 && id < n->len) label[id] = '_';
        }
    }

    depth += n->len;
    if(depth == maxKmerLen) {
        vector<int32_t> pos;
        for (int32_t i = n->FIRST; i <= n->LAST; ++i) {
            pos.push_back(suffixArray[i]);
        }
        string poss = vectorToString(pos);
        sprintf(str, "\t%d [label=\"%s (%s)\"];\n", nodeId, label.c_str(),
                poss.c_str());
        out << str;
        return;
    } else {
        sprintf(str, "\t%d [label=\"%s\"];\n", nodeId, label.c_str());
        out << str;
    }

    int cnt = 0;
    for (uint8_t i = 0; i < SIGMA; ++i) {
        if(n->MASK & 1 << i) {
            memset(str, 0, 512);
            // edges
            sprintf(str, "\t%d -- %d;\n", nodeId, n->CHILD + cnt);
            out << str;
            cnt++;
        }
    }

    cnt = 0;
    for (uint8_t i = 0; i < SIGMA; ++i) {
        if(n->MASK & 1 << i) {
            dfsToDot(n->CHILD + cnt, out, depth);
            cnt++;
        }
    }
}

string Tree::toDot() {
    stringstream stream;
    stream << "graph {" << std::endl;
    stream << "graph [ordering=\"out\"];" << std::endl;

    dfsToDot(0, stream, 0);

    stream << "}" << std::endl;
    return stream.str();
}

void Tree::saveToDotFile(string path) {
    ofstream myfile;
    myfile.open(path.c_str());
    myfile << toDot() << endl;
    myfile.close();
}

void Tree::printResults(pair<int32_t, int32_t> result) {
    int32_t first, last, cnt;
    tie(first, last) = result;
    cnt = last - first + 1;

    if(first != -1) {
        printf("%d\n", cnt);
        for (int j = first; j <= last; ++j) {
            printf("%d\n", suffixArray[j]);
        }
    } else {
        printf("0\n");
    }
}

void Tree::findMatches(char* read, int32_t readLen) {
    for (int i = 0; i < readLen - maxKmerLen + 1; ++i) {
        printResults(search(read + i, maxKmerLen, Missmatch));
        printResults(search(read + i, maxKmerLen, Deletion));

        if(i + maxKmerLen - 1 + wildcardCnt < readLen)
            printResults(search(read + i, maxKmerLen + wildcardCnt, Insertion));
        else
            printf("0\n");

    }
}
