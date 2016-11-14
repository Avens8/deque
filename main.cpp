#include <iostream>
#include <deque>
#include "gtest/gtest.h"
#include "deque.h"

TEST(DEQUE, construcorTest)
{
    Deque<int> firstD;
    Deque<int> secondD;
    for (int i = 1; i <= 1000; ++i)
        firstD.push_back(i);
    for (int i = 1001; i <= 2000; ++i)
        firstD.push_front(i);
    secondD = firstD;
    ASSERT_EQ(firstD.size(), secondD.size());
    for (int i = 0; i < 2000; ++i)
        ASSERT_EQ(firstD[i], secondD[i]);
    Deque<int> thirdD;
    thirdD = secondD = firstD = firstD;
    for (int i = 0; i < 2000; ++i)
        ASSERT_EQ(firstD[i], thirdD[i]);
}

TEST(DEQUE, pushAndPopTest)
{
    
    Deque<int> myDeck;
    std::deque<int> stdDeck;
    for (int i = 1; i <= 1000; ++i)
    {
        if (rand() % 2 == 0)
        {
            myDeck.push_front(i);
            stdDeck.push_front(i);
        }
        else {
            myDeck.push_back(i);
            stdDeck.push_back(i);
        }
    }
    for (int i = 0; i < 1000; ++i)
    {
        ASSERT_EQ(myDeck[i], stdDeck[i]);
    }
    for (int i = 1; i <= 1000; ++i)
    {
        if (rand() % 2 == 0)
        {
            myDeck.pop_back();
            stdDeck.pop_back();
        }
        else {
            myDeck.pop_front();
            stdDeck.pop_front();
        }
        for (int j = 0; j < myDeck.size(); ++j) {
            ASSERT_EQ(myDeck[j], stdDeck[j]);
        }
    }
    ASSERT_TRUE(myDeck.empty());
    
}

TEST(DEQUE, frontBackTest)
{
    Deque<int> myDeck;
    std::deque<int> stdDeck;
    for (int i = 1; i <= 50; ++i)
    {
        myDeck.push_front(i);
        stdDeck.push_front(i);
        ASSERT_EQ(myDeck.back(), stdDeck.back());
        ASSERT_EQ(myDeck.front(), stdDeck.front());
    }
    
    for (int i = 1; i <= 50; ++i) {
        myDeck.push_back(i);
        stdDeck.push_back(i);
        ASSERT_EQ(myDeck.back(), stdDeck.back());
        ASSERT_EQ(myDeck.front(), stdDeck.front());
    }
    Deque<int> myDeckSec;
    std::deque<int> stdDeckSec;
    for (int i = 1; i <= 50; ++i) {
        myDeck.push_back(i);
        stdDeck.push_back(i);
        ASSERT_EQ(myDeck.back(), stdDeck.back());
        ASSERT_EQ(myDeck.front(), stdDeck.front());
    }
}



TEST(DEQUE, iteratorTest)
{
    Deque<int> myDeck;
    std::deque<int> stdDeck;
    for (int i = 1; i <= 1000; ++i)
    {
        if (rand() % 2 == 0)
        {
            myDeck.push_front(i);
            stdDeck.push_front(i);
        }
        else {
            myDeck.push_back(i);
            stdDeck.push_back(i);
        }
    }
    int cnt = 0;
    for (auto it = myDeck.begin(); it != myDeck.end(); ++it) {
        cnt++;
        ASSERT_EQ(*it, *(stdDeck.begin() + (it - myDeck.begin())));
    }
    ASSERT_EQ(cnt, myDeck.size());
    cnt = 0;
    for (auto it = myDeck.end() - 1; it > myDeck.begin(); --it) {
        cnt++;
        ASSERT_EQ(*it, *(stdDeck.begin() + (it - myDeck.begin())));
    }
    ASSERT_EQ(cnt, myDeck.size() - 1);
    cnt = 0;
    for (auto it = myDeck.rbegin(); it != myDeck.rend(); ++it) {
        cnt++;
        ASSERT_EQ(*it, *(stdDeck.rbegin() + (it - myDeck.rbegin())));
    }
    ASSERT_EQ(cnt, myDeck.size());
    cnt = 0;
    ASSERT_EQ(*myDeck.cbegin(), *stdDeck.cbegin());
    ASSERT_EQ(*myDeck.crbegin(), *stdDeck.crbegin());
    ASSERT_EQ(*(myDeck.cend() - 1), *(stdDeck.cend()- 1));
    ASSERT_EQ(*(myDeck.crend() - 1), *(stdDeck.crend() - 1));
    for (int i = 1; i < 1000; ++i)
    {
        ASSERT_EQ((myDeck.begin()[i]), (stdDeck.begin()[i]));
        ASSERT_EQ(*(myDeck.end() - i), *(stdDeck.end() - i));
        ASSERT_EQ(*(myDeck.cbegin() + i), *(stdDeck.cbegin() + i));
        ASSERT_EQ(*(myDeck.cend() - i), *(stdDeck.cend() - i));
        ASSERT_EQ((myDeck.rbegin()[i]), (stdDeck.rbegin()[i]));
        ASSERT_EQ(*(myDeck.rend() - i), *(stdDeck.rend() - i));
        ASSERT_EQ(*(myDeck.crbegin() + i), *(stdDeck.crbegin() + i));
        ASSERT_EQ(*(myDeck.crend() - i), *(stdDeck.crend() - i));
        
    }
}




TEST(DEQUE, timeTest)
{
    Deque<int> myDeck;
    for (int i = 0; i < 1e7; ++i)
    {
        if (rand() % 2 == 0)
            myDeck.push_back(rand());
        else
            myDeck.push_front(rand());
    }
    for (int i = 0; i < 1e7; ++i)
    {
        if (rand() % 2 == 0)
            myDeck.pop_back();
        else
            myDeck.pop_front();
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    Deque<int>::iterator a;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
