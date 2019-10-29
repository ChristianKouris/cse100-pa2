/**
 * TODO: File Header
 *
 * Author: Christian Kouris 
 * Sources: auto (C++) Microsoft Docs, cplusplus reference unordered_map,
 *          std::vector doc, std::pair doc, std::sort doc, string docs
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
   
    /* helper method for the destructor so that we can delete nodes
     * recursively without having to know the height of the trie 
     */
    void deleteNodes( MWTNode* node );
   
    /* helper method for predictCompletions(), recurses down all of the
     * MWTNodes (after prefix) and if there is a valid word, it added the
     * word along with its frequency to the list passed in.
     */
    void listWords( &vector<pair<string, unsigned int>*>, 
                    MWTNode* curNode, string curWord );

    /* Comparator method used to sort the list of words and their frequencies.
     * The rule is: The list is sorted from high frequency to low frequency,
     * if multiple words have the same frequency, then they are sorted
     * lexicographically.
     */
    bool compareFreq( pair<string, unsigned int> p1, 
                      pair<string, unsigned int> p2 );
    
  public:
    /* Default constructor for the DictionaryTrie class which is a MultiWay
     * Trie. The constructor will create a root MWTNode.
     */
    DictionaryTrie();

    /* Insterts a new node into the MWT using the information of "word" and
     * "freq" and returns true if successful. If the string is already in the
     * MWT, the function inserts noting and returns false.
     */
    bool insert(string word, unsigned int freq);

    /* Searches to see if a word is in the MWT. If the word is in the
     * trie, the function will return true. If the word is not in the trie,
     * it will return false.
     */
    bool find(string word) const;

    /* This function returns a list of predicted completions of the prefix
     * passed into the function up to an amount of numCompletions in order
     * of high to low frequency. If two words have the same frequency, then
     * they are ordered lexicographically.
     */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* Standard destructor for the MWT class. It recurses down to all the 
     * leaves and deletes up to the root
     */
    ~DictionaryTrie();
};

#endif  // DICTIONARY_TRIE_HPP
