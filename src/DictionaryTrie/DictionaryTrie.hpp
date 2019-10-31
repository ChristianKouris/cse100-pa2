/**
 * The purpose of this hpp file is to provide a definition for The 
 * DictionaryTrie class, it's methods, and the MWTNode subclass.
 * This DictionaryTrie class is based on the MultiWay Trie model that is
 * used to store words and the frequency that they appear in. The point of
 * the methods in this class is to build a MWT by inserting words and to 
 * predict completions based on a prefix similar to a search bar.
 *
 * Author: Christian Kouris 
 * Email: ckouris@ucsd.edu
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
     *
     * Parameter: node - a pointer to the current node we are deleting
     */
    void deleteNodes( MWTNode* node );
   
    /* helper method for predictCompletions(), recurses down all of the
     * MWTNodes (after prefix) and if there is a valid word, it added the
     * word along with its frequency to the list passed in.
     *
     * Parameter: wordList - the list of all the word completions we track
     * Parameter: curNode - the current node of the recursion
     * Parameter: curWord - a string of the word built so far
     */
    void listWords( vector<pair<string, unsigned int>*> * wordList,
                    MWTNode* curNode, string curWord );

    /* Helper method for predictUnderscores()
     *
     */
    void getPatterns( vector<pair<string, unsigned int>*> * wordList,
                      MWTNode* curNode, string pattern, unsigned int pos );

    /* Comparator method used to sort the list of words and their frequencies.
     * The rule is: The list is sorted from high frequency to low frequency,
     * if multiple words have the same frequency, then they are sorted
     * lexicographically.
     *
     * Parameter: p1 - the first pair to be compared
     * Parameter: p2 - the second pair to be compared
     */
    static bool compareFreq( const pair<string, unsigned int> * p1, 
                             const pair<string, unsigned int> * p2 );
    
  public:
    /* Default constructor for the DictionaryTrie class which is a MultiWay
     * Trie. The constructor will create a root MWTNode.
     */
    DictionaryTrie();

    /* Insterts a new node into the MWT using the information of "word" and
     * "freq" and returns true if successful. If the string is already in the
     * MWT, the function inserts noting and returns false.
     * 
     * Parameter: word - the word string we are inserting into the MWT
     * Parameter: freq - the frequency of the word
     */
    bool insert(string word, unsigned int freq);

    /* Searches to see if a word is in the MWT. If the word is in the
     * trie, the function will return true. If the word is not in the trie,
     * it will return false.
     *
     * Prameter: word - the string we are looking for in the MWT
     */
    bool find(string word) const;

    /* This function returns a list of predicted completions of the prefix
     * passed into the function up to an amount of numCompletions in order
     * of high to low frequency. If two words have the same frequency, then
     * they are ordered lexicographically.
     *
     * Parameter: prefix - a string that we will return all its completions
     * Parameter: numCompletions - the max length of the list of predictions
     */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* This function takes in a string pattern which most likely contians
     * underscores. These undrscores can be any character in the words that
     * we will return. We will return all predictions for the pattern with 
     * underscore wildcards up to a numCompletions number from high to low
     * frequency.
     *
     * Parameter: pattern - a word that contains underscores as wildcard chars
     * Parameter: numCompletions - the max length of the list of predictions
     */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* Standard destructor for the MWT class. It recurses down to all the 
     * leaves and deletes up to the root
     */
    ~DictionaryTrie();
};

#endif  // DICTIONARY_TRIE_HPP
