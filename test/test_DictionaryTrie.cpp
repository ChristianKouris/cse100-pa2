/**
 * TODO: File HEADER
 *
 * Author:
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
