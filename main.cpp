#include <node.h>
#include <trie.h>
using namespace std;

int main(int argc, char **argv) {

    const char keys[][9] = { "the", "a", "there", "an", "an", "by", "bye", "an",
            "their" };


    Trie* trie = new Trie();

    // Construct trie
    for (int i = 0; i < 8; i++) {
        trie->insert(keys[i], strlen(keys[i]), i);
    }
    vector<int> rv;
    trie->search("an", 2, rv);
    for(auto i : rv) {
        cout << i << endl;
    }
    cout << "END" << endl;
    return 0;
}

