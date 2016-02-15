#include <construction/c_tree.h>
#include <tree/node.h>
#include <tree/tree.h>
#include <bioinf/sequence.h>
#include <bioinf/bioutil.h>
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
const int MAX_LEN = 100000007;

char reference[MAX_LEN];
char read[MAX_LEN];
char shape[SHAPE];

int query();
int indexOnly();

int main(int argc, char **argv) {
    query();
    return 0;
}



int query() {
    scanf ("%s", reference);
    scanf ("%s", shape);
    scanf ("%s", read);

    int n = strlen(reference);
    int m = strlen(read);
    int k = strlen(shape);

    for(int i = 0; i < n; ++i) reference[i] = bioinf::baseToInt(reference[i]);
    for(int i = 0; i < m; ++i) read[i] = bioinf::baseToInt(read[i]);

    CTree* tree = new CTree(reference, n, shape, k);
    Tree* t = new Tree(tree);



//    FILE* tOut = fopen("stablo.bin", "wb");
//    t->save(tOut);
//    fclose(tOut);
//    FILE* tIn = fopen("stablo.bin", "rb");
//    Tree* t2 = new Tree(tIn, t->txt, t->n);
//
//    tree->saveToDotFile("ctree.dot");
//    t->saveToDotFile("tree.dot");
//    t2->saveToDotFile("treeSaved.dot");


    clock_t timeStart = clock();
    t->findMatches(read, m);
    clock_t timeEnd = clock();
    printf ("%.3f\n", ((double)timeEnd - timeStart) / CLOCKS_PER_SEC);

    return 0;
}

int indexOnly() {
    scanf ("%s", reference);
    scanf ("%s", shape);
    scanf ("%s", read);

    int n = strlen(reference);
    int m = strlen(read);
    int k = strlen(shape);

    for(int i = 0; i < n; ++i) reference[i] = bioinf::baseToInt(reference[i]);
    for(int i = 0; i < m; ++i) read[i] = bioinf::baseToInt(read[i]);



    clock_t timeStart = clock();

    CTree* tree = new CTree(reference, n, shape, k);
    Tree* t = new Tree(tree);
    FILE* tOut = fopen("./indices_dir/index", "wb");
    t->save(tOut);
    fclose(tOut);

    clock_t timeEnd = clock();
    printf ("%.3f\n", ((double)timeEnd - timeStart) / CLOCKS_PER_SEC);

    return 0;
}

