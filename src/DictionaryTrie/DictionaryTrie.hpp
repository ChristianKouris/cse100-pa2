/**
 * TODO: File Header
 *
 * Author:
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

using namespace std;

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
  private:
    /** Inner class which defines a Multiway tree node */
    class MWTNode {
      public:
        //is it the last letter of a word
        bool isEnd;
        //frequency
        unsigned int freq;
        //a map that pairs a char to another MWTNode
        unordered_map<char, MWTNode*> hashMap; 
        
        MWTNode() {
            isEnd = false;
            freq = 0;
            hashMap = std::unordered_map<char, MWTNode*>();
        }
    };

    MWTNode* root;    
    
  public:
    /* TODO: add function header */
    DictionaryTrie();

    /* TODO: add function header */
    bool insert(string word, unsigned int freq);

    /* TODO: add function header */
    bool find(string word) const;

    /* TODO: add function header */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    ~DictionaryTrie();
};

#endif  // DICTIONARY_TRIE_HPP
