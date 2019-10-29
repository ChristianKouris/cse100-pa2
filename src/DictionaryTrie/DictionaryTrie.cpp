/**
 * TODO: File Header
 *
 * Author: Christian Kouris
 */
#include "DictionaryTrie.hpp"
#include <iostream>

DictionaryTrie::DictionaryTrie() {

    root = new MWTNode();

}

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

vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    
    //first traverse down the Trie so we get to the node for prefix
    MWTNode* currNode = root;
    for( int i = 0; i < prefix.size(); i++ ) { 

        //go to the node
        currNode = currNode->hashMap.find(prefix[i])->second;

        //check to see if we ended up in a node that doesn't exist
        if( currNode == nullptr ) {
            return std::vector<string>();
        }

    }

    //now that we're at the prefix, we want to get everything under it.
    vector<pair<string,unsigned int>*> stringAndFreq = 
        std::vector<std::pair<string, unsigned int>*>();

    
    //call the helper function
    listWords( stringAndFreq, currNode, prefix );

    //we now have an array of every word past prefix. We need to sort it
    std::sort( stringAndFreq.begin(), stringAndFreq.end(), compareFreq);

    //create a list to hold the predicted completions, create iterators
    //that will  end on either numCompletion loops or the end of the vector
    vector<string> completionList = std::vector<string>();
    auto wordIter = stringAndFreq.begin(); int i = 0;
    //loop through numCompletion times or the entire vector of pairs
    while ( i < numCompletions && wordIter != stringAndFreq.end() ) {

        completionList.push_back( (*wordIter)->first );

    }

    //free memory of allocated pairs
    delete stringAndFreq;

    //return the list of predicted compltions
    return completionList;
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

DictionaryTrie::~DictionaryTrie() {

    deleteNodes( root );

}

void DictionaryTrie::deleteNodes( MWTNode* node ) {

    if( node->hashMap.empty() == true ) {
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
    
void listWords( &vector<pair<string, unsigned int>*> wordList, 
                MWTNode* curNode, string curWord ) {

    //base case of current node being null
    if( curNode == nullptr ) {
        return;
    }
    
    //check to see if the current node is the final letter for a word
    if( curNode->isEnd == true ) {

        //put the word and its frequency into the vector
        wordList.push_back( 
            new std::pair<string, unsigned int>( curWord, curNode->freq ) );

    }
    
    //create an iterator at the beginning
    auto iterator = curNode->hashMap.begin();
    //loop through each key
    while( iterator != node->hashMap.end() ) {

        //append the key character to the current word
        newWord = curWord + iterator->first;
        
        //recurse down to listwords
        listWords( wordList, iterator->second, newWord ); 

    }

}

bool compareFreq( pair<string, unsigned int> p1, 
                  pair<string, unsigned int> p2 ) {

    //if they have same frequency, they will be in alphabetical order
    if( p1.second == p2.second ) {

        return p1.compare(p2) < 0;

    }

    return p2.second < p1.second;


}
