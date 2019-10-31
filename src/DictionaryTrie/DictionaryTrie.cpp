 /**
 * The purpose of this cpp file is to provide implementation for the
 * methods whose signatures were defined in the hpp file. This trie
 * is built in the style of a MultiWay Trie that stores words and their
 * frequencies. The class' primary purpose is to build a trie and then
 * predict words from a given prefix.
 *
 * Author: Christian Kouris 
 * Email: ckouris@ucsd.edu
 * Sources: auto (C++) Microsoft Docs, cplusplus reference unordered_map,
 *          std::vector doc, std::pair doc, std::sort doc, string docs
 */
#include "DictionaryTrie.hpp"
#include <iostream>
#include <algorithm>

/* Default constructor for the DictionaryTrie class which is a MultiWay
 * Trie. The constructor will create a root MWTNode.
 */
DictionaryTrie::DictionaryTrie() {

    root = new MWTNode();

}

/* Insterts a new node into the MWT using the information of "word" and
 * "freq" and returns true if successful. If the string is already in the
 * MWT, the function inserts noting and returns false.
 *
 * Parameter: word - the word string we are inserting into the MWT
 * Parameter: freq - the frequency of the word
 */
bool DictionaryTrie::insert(string word, unsigned int freq) { 

    MWTNode* currNode = root;
   
    //loop through all of the letters in word sans the final one
    for( unsigned int i = 0; i < word.size()-1; i++ ) {

        //search for the letter in the current node's hash map
        //if the letter doesn't exist add it to the hash map
        if( currNode->hashMap.find(word[i]) == currNode->hashMap.end() ) {
            
            MWTNode* newNode = new MWTNode();
            currNode->hashMap.emplace(word[i], newNode);
            currNode = newNode;

        } else {
    
        //if it exists, go to it
        currNode = currNode->hashMap.find(word[i])->second;

        }

    }

    //now do this for the last letter, but if it's already in, return false
    //case 1: final letter node does not exist
    if( currNode->hashMap.find(word[word.size()-1]) == 
        currNode->hashMap.end() ) {
        
        MWTNode* newNode = new MWTNode();
        newNode->isEnd = true;
        newNode->freq = freq;
        currNode->hashMap.emplace(word[word.size()-1], newNode);
        return true;

    } 

    MWTNode* finalLetter = currNode->hashMap.find(word[word.size()-1])->second;
    //case 2: final letter node exists but is not an end node
    if (finalLetter->isEnd == false ) {
        
        //case 2: final letter node exists but is not an end node
        //an example is inserting word an after word animal
        finalLetter->isEnd = true;
        finalLetter->freq = freq;
        return true;

    }
    
    //case 3: final letter node exists and is already marked as an end node
    return false; 

}

/* Searches to see if a word is in the MWT. If the word is in the
 * trie, the function will return true. If the word is not in the trie,
 * it will return false.
 *
 * Prameter: word - the string we are looking for in the MWT
 */
bool DictionaryTrie::find(string word) const { 

    if( word.size() < 1 ) {
        return false;
    }
    //pretty much do what the insert function does
    MWTNode* currNode = root;
   
    //recurse down the MWT to see if each letter exists in the MWT
    for( unsigned int i = 0; i < word.size()-1; i++ ) {

        //see if the word is in the hash map, if not return false
        if( currNode->hashMap.find(word[i]) == currNode->hashMap.end() ) {
            
            return false;

        } else {
    
            currNode = currNode->hashMap.find(word[i])->second;

        }

    }
    
    //case 1: final letter node does not exist
    if( currNode->hashMap.find(word[word.size()-1]) == 
        currNode->hashMap.end() ) {
        
        return false;

    } 

    //case 2: final letter node exists but is not an end node
    MWTNode* finalLetter = currNode->hashMap.find(word[word.size()-1])->second;
    if (finalLetter->isEnd == false ) {
        
        return false;

    }
    
    //case 3: final letter node exists and is already marked as an end node
    return true; 

}

/* This function returns a list of predicted completions of the prefix
 * passed into the function up to an amount of numCompletions in order
 * of high to low frequency. If two words have the same frequency, then
 * they are ordered lexicographically.
 *
 * Parameter: prefix - a string that we will return all its completions
 * Parameter: numCompletions - the max length of the list of predictions
 */
vector<string> DictionaryTrie::predictCompletions(
    string prefix, unsigned int numCompletions) {
    
    //first traverse down the Trie so we get to the node for prefix
    MWTNode* currNode = root;
    for( unsigned int i = 0; i < prefix.size(); i++ ) { 

        //check to see if we ended up in a node that doesn't exist
        if( currNode->hashMap.find(prefix[i]) == currNode->hashMap.end() ) {
            return std::vector<string>();
        }
        
        //go to the node
        currNode = currNode->hashMap.find(prefix[i])->second;

    }

    //now that we're at the prefix, we want to get everything under it.
    vector<pair<string,unsigned int>*> * stringAndFreq = 
        new std::vector<std::pair<string, unsigned int>*>();

    
    //call the helper function
    listWords( stringAndFreq, currNode, prefix );

    //we now have an array of every word past prefix. We need to sort it
    std::sort( stringAndFreq->begin(), stringAndFreq->end(), compareFreq );

    //create a list to hold the predicted completions
    vector<string> completionList = std::vector<string>();
    unsigned int i = 0; 
    //loop through numCompletion times or the entire vector of pairs
    while (  i < numCompletions && i < stringAndFreq->size() ) {

        completionList.push_back( stringAndFreq->at(i)->first );
        i++;

    }

    //free memory of allocated pairs
    for( unsigned int i = 0; i < stringAndFreq->size(); i++ ) {
        delete stringAndFreq->at(i);
    }
    delete stringAndFreq;

    //return the list of predicted compltions
    return completionList;
}

/* This function takes in a string pattern which most likely contians
 * underscores. These undrscores can be any character in the words that
 * we will return. We will return all predictions for the pattern with 
 * underscore wildcards up to a numCompletions number from high to low
 * frequency.
 *
 * Parameter: pattern - a word that contains underscores as wildcard chars
 * Parameter: numCompletions - the max length of the list of predictions
 */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    
    //create a vector to hold the words and their frequencies
    vector<pair<string,unsigned int>*> * stringAndFreq = 
        new std::vector<std::pair<string, unsigned int>*>();

    //call the recursion helper function to populate vector list
    getPatterns( stringAndFreq, root, pattern, 0 );

    //use the same sorter as in predictCompletions
    std::sort( stringAndFreq->begin(), stringAndFreq->end(), compareFreq );

    //Do the same thing as predictUnderscores to get a single string vector
    vector<string> wildCardList = std::vector<string>();
    unsigned int i = 0; 
    //loop through numCompletion times or the entire vector of pairs
    while (  i < numCompletions && i < stringAndFreq->size() ) {

        wildCardList.push_back( stringAndFreq->at(i)->first );
        i++;

    }

    //free memory of allocated pairs
    for( unsigned int i = 0; i < stringAndFreq->size(); i++ ) {
        delete stringAndFreq->at(i);
    }
    delete stringAndFreq;

    //return the list of predicted underscores
    return wildCardList;
}

/* Standard destructor for the MWT class. It recurses down to all the 
 * leaves and deletes up to the root
 */
DictionaryTrie::~DictionaryTrie() {

    deleteNodes( root );

}

/* helper method for the destructor so that we can delete nodes
 * recursively without having to know the height of the trie 
 *
 * Parameter: node - a pointer to the current node we are deleting
 */
void DictionaryTrie::deleteNodes( MWTNode* node ) {

    if( node->hashMap.empty() == true ) {
        delete node;
        return;
    }
    //create an iterator at the beginning
    auto iterator = node->hashMap.begin();
    //loop from beginning ot end
    while( iterator != node->hashMap.end() ) {

        // recurse to each Node stored as values
        deleteNodes( iterator->second );
        iterator++;

    }

    //finally delete the current node and return
    delete node;

}
    
/* helper method for predictCompletions(), recurses down all of the
 * MWTNodes (after prefix) and if there is a valid word, it added the
 * word along with its frequency to the list passed in.
 *
 * Parameter: wordList - the list of all the word completions we track
 * Parameter: curNode - the current node of the recursion
 * Parameter: curWord - a string of the word built so far
 */
void DictionaryTrie::listWords( vector<pair<string, unsigned int>*> * wordList,
                                MWTNode* curNode, string curWord ) {

    //base case of current node being null
    if( curNode == nullptr ) {
        return;
    }
    
    //check to see if the current node is the final letter for a word
    if( curNode->isEnd == true ) {

        //put the word and its frequency into the vector
        wordList->push_back( 
            new std::pair<string, unsigned int>( curWord, curNode->freq ) );

    }
    
    //create an iterator at the beginning
    auto iterator = curNode->hashMap.begin();
    //loop through each key
    while( iterator != curNode->hashMap.end() ) {

        //append the key character to the current word
        string newWord = curWord + iterator->first;
        
        //recurse down to listwords
        listWords( wordList, iterator->second, newWord ); 
        
        iterator++;

    }

}

/* Helper method for predictUnderscores() which recurses down 
 * the MWT. For each recursion we either recurse down the chracter at
 * pos or if its an underscore recurse down all characters at pos.
 * when pos reaches the end of pattern's length, it adds the word
 * if valid.
 *
 * Parameter: wordList - a list holding words and their frequencies
 * Parameter: curNode - the current node in the recursion
 * Parameter: pattern - the current string pattern
 * Parameter: pos - the position in the pattern the recursion is at
 */
void DictionaryTrie::getPatterns( 
    vector<pair<string, unsigned int>*> * wordList, MWTNode* curNode, 
    string pattern, unsigned int pos ) {

    //if we pass in a null Node
    if( curNode == nullptr ) {
        return;
    }

    //base case if we are after the last character
    if( pos == pattern.size() ) {

        //if it is an end of a word, add it to the list
        if( curNode->isEnd ) {

            wordList->push_back( 
                new std::pair<string, unsigned int>(pattern, curNode->freq) );

        }

        //return since we have filled in the underscores
        return;

    }

    //check to see if the current letter is an underscore or not
    if( pattern[pos] == '_' ) {
        
        //if it is an underscore recurse down every character in curNode
        auto iterator = curNode->hashMap.begin();
        while( iterator != curNode->hashMap.end() ) {
            string newStr = pattern;
            *(newStr.begin() + pos) = iterator->first;
            getPatterns( wordList, iterator->second, newStr, pos+1 );
            iterator++;
        }

    } else {

        //check to see if the char is in the tree
        auto charIter = curNode->hashMap.find(pattern[pos]);
        if( charIter == curNode->hashMap.end() ) {
            return;
        }

        getPatterns( wordList, charIter->second, pattern, pos+1 );

    }

}

/* Comparator method used to sort the list of words and their frequencies.
 * The rule is: The list is sorted from high frequency to low frequency,
 * if multiple words have the same frequency, then they are sorted
 * lexicographically.
 *
 * Parameter: p1 - the first pair to be compared
 * Parameter: p2 - the second pair to be compared
 */
bool DictionaryTrie::compareFreq( const pair<string, unsigned int> * p1, 
                                  const pair<string, unsigned int> * p2 ) {

    //if they have same frequency, they will be in alphabetical order
    if( p1->second == p2->second ) {

        return p1->first.compare(p2->first) < 0;

    }

    return p2->second < p1->second;

}
