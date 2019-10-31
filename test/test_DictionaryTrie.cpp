/**
 * This function is a tester function for the DictionaryTrie Class. The methods
 * tested here are insert, find, predictCompletions and predictUnderscores
 *
 * Author: Christian Kouris
 * Email: ckouris@ucsd.edu
 * Sources:
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty test */
TEST(DictTrieTests, EMPTY_TEST) {
    DictionaryTrie dict;
    ASSERT_EQ(dict.find("abrakadabra"), false);
}

TEST(DictTrieTests, INSERT_TEST) {
    DictionaryTrie dict;
    dict.insert("help", 100);
    ASSERT_EQ(dict.find("help"), true);
}

TEST(DictTrieTests, INCORRECT_FIND_TEST) {
    DictionaryTrie dict;
    dict.insert("animal", 100);
    ASSERT_EQ(dict.find("an"), false);
}

TEST(DictTrieTests, REINSERT_TEST) {
    DictionaryTrie dict;
    dict.insert("animal", 100);
    ASSERT_EQ(dict.insert("animal", 15), false);
}

TEST(DictTrieTests, PREDICT_COMPLETIONS_SIZE_TEST) {
    DictionaryTrie dict;
    dict.insert("animal", 100);
    dict.insert("acme", 90);
    dict.insert("coffee", 10);
    dict.insert("animation", 100);
    dict.insert("anarchy", 5);
    dict.insert("beauty", 20);
    dict.insert("an", 1000);
    dict.insert("annihilate", 50);
    dict.insert("anagram", 10);
    vector<string> list = dict.predictCompletions("an", 4);
    ASSERT_EQ( list.size(), 4 );
}

TEST(DictTrieTests, PREDICT_COMPLETIONS_OVER_SIZE_TEST) {
    DictionaryTrie dict;
    dict.insert("animal", 100);
    dict.insert("acme", 90);
    dict.insert("coffee", 10);
    dict.insert("animation", 100);
    dict.insert("anarchy", 5);
    dict.insert("beauty", 20);
    dict.insert("an", 1000);
    dict.insert("annihilate", 50);
    dict.insert("anagram", 10);
    vector<string> list = dict.predictCompletions("an", 100);
    ASSERT_EQ( list.size(), 6 );
}

TEST(DictTrieTests, PREDICT_COMPLETIONS_BAD_PREFIX_TEST) {
    DictionaryTrie dict;
    dict.insert("animal", 100);
    dict.insert("acme", 90);
    dict.insert("coffee", 10);
    dict.insert("animation", 100);
    dict.insert("anarchy", 5);
    dict.insert("beauty", 20);
    dict.insert("an", 1000);
    dict.insert("annihilate", 50);
    dict.insert("anagram", 10);
    vector<string> list = dict.predictCompletions("hel", 10);
    ASSERT_EQ( list.size(), 0 );
}

TEST(DictTrieTests, PREDICT_COMPLETIONS_FREQ_ORDER_TEST) {
    DictionaryTrie dict;
    dict.insert("animal", 100);
    dict.insert("acme", 90);
    dict.insert("coffee", 10);
    dict.insert("animation", 100);
    dict.insert("anarchy", 5);
    dict.insert("beauty", 20);
    dict.insert("an", 1000);
    dict.insert("annihilate", 50);
    dict.insert("anagram", 10);
    vector<string> list = dict.predictCompletions("an", 4);
    ASSERT_EQ( list[0], "an" );
    ASSERT_EQ( list[3], "annihilate" );
}

TEST(DictTrieTests, PREDICT_COMPLETIONS_ALPHABET_TEST) {
    DictionaryTrie dict;
    dict.insert("animal", 100);
    dict.insert("acme", 90);
    dict.insert("coffee", 10);
    dict.insert("animation", 100);
    dict.insert("anarchy", 5);
    dict.insert("beauty", 20);
    dict.insert("an", 1000);
    dict.insert("annihilate", 50);
    dict.insert("anagram", 10);
    vector<string> list = dict.predictCompletions("an", 4);
    ASSERT_EQ( list[1], "animal" );
}

TEST(DictTrieTests, PREDICT_UNDERSCORES_SIZE_TEST) {
    DictionaryTrie dict;
    dict.insert("band", 100);
    dict.insert("hand", 90);
    dict.insert("handle", 10);
    dict.insert("tang", 100);
    dict.insert("fort", 5);
    dict.insert("land", 20);
    dict.insert("stand", 1000);
    dict.insert("rand", 50);
    dict.insert("bond", 10);
    vector<string> list = dict.predictUnderscores("_an_", 3);
    ASSERT_EQ( list.size(), 3 );
}

TEST(DictTrieTests, PREDICT_UNDERSCORES_OVER_SIZE_TEST) {
    DictionaryTrie dict;
    dict.insert("band", 100);
    dict.insert("hand", 90);
    dict.insert("handle", 10);
    dict.insert("tang", 100);
    dict.insert("fort", 5);
    dict.insert("land", 20);
    dict.insert("stand", 1000);
    dict.insert("rand", 50);
    dict.insert("bond", 10);
    vector<string> list = dict.predictUnderscores("_an_", 20);
    ASSERT_EQ( list.size(), 5 );
}

TEST(DictTrieTests, PREDICT_UNDERSCORES_FREQ_ORDER_TEST) {
    DictionaryTrie dict;
    dict.insert("band", 100);
    dict.insert("hand", 90);
    dict.insert("handle", 10);
    dict.insert("tang", 5);
    dict.insert("fort", 5);
    dict.insert("land", 90);
    dict.insert("stand", 1000);
    dict.insert("rand", 50);
    dict.insert("bond", 10);
    vector<string> list = dict.predictUnderscores("_an_", 3);
    ASSERT_EQ( list[0],  "band");
}

TEST(DictTrieTests, PREDICT_UNDERSCORES_ALPH_ORDER_TEST) {
    DictionaryTrie dict;
    dict.insert("band", 100);
    dict.insert("land", 90);
    dict.insert("handle", 10);
    dict.insert("tang", 5);
    dict.insert("fort", 5);
    dict.insert("hand", 90);
    dict.insert("stand", 1000);
    dict.insert("rand", 50);
    dict.insert("bond", 10);
    vector<string> list = dict.predictUnderscores("_an_", 3);
    ASSERT_EQ( list[1], "hand" );
}

