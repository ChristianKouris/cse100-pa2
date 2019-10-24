/**
 * TODO: File Header
 *
 * Author:
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

/* TODO */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    return {};
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/* TODO */
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
