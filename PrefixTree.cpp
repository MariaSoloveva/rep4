#include "PrefixTree.hpp"

std::pair<Node*, bool> PrefixTree::find(const char character, Node* n) {
    bool changed = false;
    if (!n) { n = root; }

    if (n->list_of_vertexes.find(character) != n->list_of_vertexes.end()) {
        n = n->list_of_vertexes[character];
        changed = true;
    }
    return std::make_pair(n, changed);
};

void PrefixTree::insert(const char character, unsigned int code, Node* n) {
    Node* new_node = new Node;
    new_node->parent = n;
    new_node->code_value = code;
    n->list_of_vertexes.insert(std::make_pair(character, new_node));
};