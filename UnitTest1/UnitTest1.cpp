#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab12.11/lab12.11.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
        TEST_METHOD(TestAddOrUpdate_NewEntry)
        {
            DictionaryEntry* head = nullptr;
            addOrUpdateEntry(head, "apple", "яблуко", 1);
            Assert::IsNotNull(head);
            Assert::AreEqual(string("apple"), head->englishWord);
            Assert::AreEqual(string("яблуко"), head->ukrainianWord);
            Assert::AreEqual(1, head->accessCount);
            deleteDictionary(head);
        }
	};
}
