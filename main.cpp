#include <construction/c_tree.h>
#include <tree/node.h>
#include <tree/tree.h>
#include <bioinf/sequence.h>
using namespace std;

void readFasta() {
    const char* fastaInPath = "bla";
    FILE* fastaIn = fopen(fastaInPath, "r");
    bioinf::Sequence* seq = new bioinf::Sequence;
    fprintf(stderr, "Reading sequence file %s\n", fastaInPath);
    seq->readSequencesFromFASTA(fastaIn);
    fprintf(stderr, "Number of sequences: %lu\n",
            (unsigned long) seq->numOfSequences());
    for (uint32_t i = 0; i < seq->numOfSequences(); ++i) {
        fprintf(stderr, "\t%d: %s: [%d]\n", i, seq->info(i), seq->seqLen(i));
    }
    fclose(fastaIn);
    cout << "END" << endl;
}

const int SHAPE = 32;
const int MAX_LEN = 1000007;

char reference[MAX_LEN];
char read[MAX_LEN];
char shape[SHAPE];

int main(int argc, char **argv) {
    scanf ("%s", reference);
    scanf ("%s", shape);
    scanf ("%s", read);

    int n = strlen(reference);
    int m = strlen(read);
    int k = strlen(shape);

    CTree* tree = new CTree(reference, n, shape, k);
    cout << "Tree constructed" << endl;
    cout << "Node cnt:" << tree->nodeCnt << endl;

    Tree* t = new Tree(tree);



    FILE* tOut = fopen("stablo.bin", "wb");
    t->save(tOut);
    fclose(tOut);

    FILE* tIn = fopen("stablo.bin", "rb");
    Tree* t2 = new Tree(tIn, t->txt, t->n);

    tree->saveToDotFile("ctree.dot");
    t->saveToDotFile("tree.dot");
    t2->saveToDotFile("treeSaved.dot");


    t->findMatches(read, m);

    return 0;
}

