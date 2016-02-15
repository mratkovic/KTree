/*
 * c_tree.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: marko
 */

#include <construction/c_tree.h>

#include <bioinf/bioutil.h>
#include <util/utility_functions.h>

CTree::CTree(const char* txt_, int32_t len_, const char* shape_,
             int8_t kmerLen_)
        : txt(txt_),
          len(len_),
          shape(shape_),
          kmerLen(kmerLen_) {
    root_ = NULL;
    nodeCnt = 0;


    // Construct trie
    for (int i = 0; i < len - kmerLen + 1; i++) {
        insert(i, kmerLen);
    }
}

CTree::~CTree() {
    if(root_) {
        delete root_;
    }
}

void CTree::insert(int32_t start, int32_t len) {
    root_ = insert(root_, start, len, start, 0);
}

inline int32_t CTree::diffIdx(const char* word1, const char* word2, int32_t len,
                              int8_t depth) {
    for (int32_t i = 0; i < len; ++i, ++word1, ++word2) {
        if(i+depth < kmerLen && shape[i + depth] == '1') continue;
        if(*word1 != *word2) {
            return i;
        }
    }
    return len;
}

CInnerNode* CTree::splitNode(CNode* node, int32_t i) {
    // new parent
    CInnerNode* parent = new CInnerNode(node->id, i);

    // old node update
    node->id += i;
    node->len -= i;

    //int8_t idx = bioinf::baseToInt(txt[node->id]);
    int8_t idx = txt[node->id];
    parent->children[idx] = node;

    return parent;
}

CNode* CTree::insert(CNode* node, int32_t start, int32_t len, int32_t pos,
                     int8_t depth) {
    if(node == NULL) {
        nodeCnt++;
        return new CLeaf(start, len, pos);
    }

    int32_t mini = min<int32_t>(len, node->len);
    int32_t diff = diffIdx(txt + start, txt + node->id, mini, depth);

    if(diff == mini) {  // total match

        if(!node->leaf) {  // inner node
            CInnerNode* inner = (CInnerNode *) node;
            //int8_t idx = bioinf::baseToInt(txt[start + diff]);
            int8_t idx = txt[start + diff];
            inner->children[idx] = insert(inner->children[idx], start + mini,
                                          len - mini, pos, depth + mini);
            return inner;

        } else {  // kmer complete match - add pos
            CLeaf* leaf = (CLeaf*) node;
            leaf->pos.push_back(pos);
            return leaf;
        }

    } else {  // split on inner
        nodeCnt += 2;

        // update old node
        CInnerNode* parent = splitNode(node, diff);

        // new leaf
        CLeaf* leaf = new CLeaf(start + diff, len - diff, pos);
        //int8_t idx = bioinf::baseToInt(txt[start + diff]);
        int8_t idx = txt[start + diff];
        parent->children[idx] = leaf;
        return parent;
    }

}

CNode* CTree::root() {
    return root_;
}

////////////////////////////////////////////////////////////////////
// for debug
void CTree::dfs(CNode *x, std::ostream& out, int8_t depth) {
    if(x == NULL) {
        return;
    }

    char str[512];
    memset(str, 0, 512);
    string label(txt + x->id, x->len);
    for (int i = 0; i < kmerLen; ++i) {
        if(shape[i] == '1') {
            int id = i - depth;
            if(id > 0 && id < x->len) label[id] = '_';
        }
    }

    if(x->leaf) {
        string poss = vectorToString(((CLeaf *) x)->pos);
        sprintf(str, "\t%d [label=\"%s (%s)\"];\n", x, label.c_str(),
                poss.c_str());
        out << str;
        return;
    } else {
        sprintf(str, "\t%d [label=\"%s\"];\n", x, label.c_str());
        out << str;
    }

    CInnerNode* inner = (CInnerNode*) x;
    for (uint8_t i = 0; i < CNode::SIGMA; ++i) {
        if(inner->children[i]) {
            memset(str, 0, 512);
            // edges
            sprintf(str, "\t%d -- %d;\n", inner, inner->children[i]);
            out << str;
        }
    }

    for (uint8_t i = 0; i < 4; ++i) {
        if(inner->children[i]) {
            dfs(inner->children[i], out, depth + inner->len);
        }
    }
}

string CTree::toDot() {
    stringstream stream;
    stream << "graph {" << std::endl;
    stream << "graph [ordering=\"out\"];" << std::endl;

    dfs(root_, stream, 0);

    stream << "}" << std::endl;
    return stream.str();
}

void CTree::saveToDotFile(string path) {
    ofstream myfile;
    myfile.open(path.c_str());
    myfile << toDot() << endl;
    myfile.close();
}

