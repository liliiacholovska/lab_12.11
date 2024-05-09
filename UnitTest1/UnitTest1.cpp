#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab12.11/lab12.11.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
        DictionaryEntry* listHead = nullptr;

        TEST_METHOD_INITIALIZE(SetUp)
        {
            listHead = nullptr;
        }

        TEST_METHOD_CLEANUP(TearDown)
        {
            deleteList(listHead);
            listHead = nullptr;
        }

        TEST_METHOD(RebuildDictionaryAVLProperties)
        {
            addOrUpdateEntry(listHead, "apple", "яблуко", 1);
            addOrUpdateEntry(listHead, "banana", "банан", 2);
            addOrUpdateEntry(listHead, "cherry", "вишня", 3);
            DictionaryEntry* treeHead = nullptr;
            rebuildDictionary(listHead, treeHead);

            Assert::IsNotNull(treeHead);

            if (treeHead != nullptr)
            {
                Assert::IsNotNull(treeHead->left);
                Assert::IsNotNull(treeHead->right);
                Assert::IsTrue(abs(getBalance(treeHead)) <= 1);
                Assert::IsTrue(abs(getBalance(treeHead->left)) <= 1);
                Assert::IsTrue(abs(getBalance(treeHead->right)) <= 1);
            }

            deleteTree(treeHead);
        }
	};
}