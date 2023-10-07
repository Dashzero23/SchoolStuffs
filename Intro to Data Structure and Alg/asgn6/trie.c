#include "trie.h"

#include "code.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

TrieNode *trie_node_create(uint16_t code) {
    TrieNode *node = (TrieNode *) malloc(sizeof(TrieNode));
    if (node) {
        node->code = code;
        for (int i = 0; i < ALPHABET; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

void trie_node_delete(TrieNode *n) {
    if (n != NULL) {
        free(n);
    }
}

TrieNode *trie_create() {
    TrieNode *root = trie_node_create(EMPTY_CODE);
    if (root != NULL) {
        return root;
    } else {
        return false;
    }
}

void trie_reset(TrieNode *root) {
    for (int i = 0; i < ALPHABET; i++) {
        if (root->children[i]) {
            trie_delete(root->children[i]);
            root->children[i] = NULL;
        }
    }
}

void trie_delete(TrieNode *n) {
    if (!n) {
        return;
    }
    for (int i = 0; i < ALPHABET; i++) {
        if (n->children[i]) {
            trie_delete(n->children[i]);
            n->children[i] = NULL;
        }
    }
    trie_node_delete(n);
}

TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    if (!sym) {
        return NULL;
    }
    return n->children[sym];
}
