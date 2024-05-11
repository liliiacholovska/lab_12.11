#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct DictionaryEntry {
    string englishWord;
    string ukrainianWord;
    int accessCount;
    DictionaryEntry* next;

    DictionaryEntry(const string& eng, const string& ukr, int count)
        : englishWord(eng), ukrainianWord(ukr), accessCount(count), next(nullptr) {}
};

void deleteDictionary(DictionaryEntry*& head) {
    while (head) {
        DictionaryEntry* temp = head;
        head = head->next;
        delete temp;
    }
}

void addOrUpdateEntry(DictionaryEntry*& head, const string& eng, const string& ukr, int count = 0) {
    DictionaryEntry* current = head;
    DictionaryEntry* prev = nullptr;
    while (current) {
        if (current->englishWord == eng) {
            current->ukrainianWord = ukr;
            current->accessCount += count;
            return;
        }
        prev = current;
        current = current->next;
    }
    DictionaryEntry* newEntry = new DictionaryEntry(eng, ukr, count);
    if (!prev || prev->englishWord > eng) {
        newEntry->next = head;
        head = newEntry;
    }
    else {
        newEntry->next = prev->next;
        prev->next = newEntry;
    }
}

void removeEntry(DictionaryEntry*& head, const string& eng) {
    DictionaryEntry* current = head;
    DictionaryEntry* prev = nullptr;
    while (current) {
        if (current->englishWord == eng) {
            if (prev) prev->next = current->next;
            else head = current->next;
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void printDictionary(const DictionaryEntry* head) {
    if (!head) {
        cout << "Dictionary is empty.\n";
        return;
    }
    while (head) {
        cout << "English: " << head->englishWord << " - Ukrainian: " << head->ukrainianWord << " (Accesses: " << head->accessCount << ")\n";
        head = head->next;
    }
}

void saveDictionaryToFile(const DictionaryEntry* head, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        while (head) {
            outFile << head->englishWord << ":" << head->ukrainianWord << ":" << head->accessCount << "\n";
            head = head->next;
        }
        outFile.close();
        cout << "Dictionary saved to file successfully." << endl;
    }
    else {
        cout << "Unable to open file for writing." << endl;
    }
}

void loadDictionaryFromFile(DictionaryEntry*& head, const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        deleteDictionary(head); // Clear current dictionary before loading new one
        string line;
        while (getline(inFile, line)) {
            size_t first_colon = line.find(':');
            size_t second_colon = line.find(':', first_colon + 1);
            string eng = line.substr(0, first_colon);
            string ukr = line.substr(first_colon + 1, second_colon - first_colon - 1);
            int count = stoi(line.substr(second_colon + 1));
            addOrUpdateEntry(head, eng, ukr, count);
        }
        inFile.close();
        cout << "Dictionary loaded from file successfully." << endl;
    }
    else {
        cout << "Unable to open file for reading." << endl;
    }
}

bool readLine(string& line) {
    getline(cin, line);
    return !line.empty();
}

DictionaryEntry* findAndRemoveMaxAccess(DictionaryEntry*& head) {
    if (!head) return nullptr;

    DictionaryEntry* current = head;
    DictionaryEntry* maxNode = head;
    DictionaryEntry* maxPrev = nullptr;
    DictionaryEntry* prev = nullptr;

    while (current) {
        if (current->accessCount > maxNode->accessCount) {
            maxNode = current;
            maxPrev = prev;
        }
        prev = current;
        current = current->next;
    }

    if (maxNode == head) head = head->next;
    else if (maxPrev) maxPrev->next = maxNode->next;
    maxNode->next = nullptr;
    return maxNode;
}

void printAndTransfer(DictionaryEntry*& oldHead, DictionaryEntry*& newHead, const string& filename) {
    if (oldHead) {
        DictionaryEntry* maxNode = findAndRemoveMaxAccess(oldHead);
        if (maxNode) {
            cout << "Transferring: " << maxNode->englishWord << " - " << maxNode->ukrainianWord << " (Accesses: " << maxNode->accessCount << ")" << endl;
            maxNode->next = newHead;
            newHead = maxNode;

            // Save the transferred node to the specified file
            ofstream outFile(filename, ios::app); // Open file in append mode
            if (outFile.is_open()) {
                outFile << maxNode->englishWord << ":" << maxNode->ukrainianWord << ":" << maxNode->accessCount << "\n";
                outFile.close();
                cout << "Entry saved to file: " << filename << endl;
            }
            else {
                cout << "Unable to open file for writing." << endl;
            }
        }
        cout << "New Dictionary After Transfer:\n";
        printDictionary(newHead);
    }
    else {
        cout << "No entries left to transfer.\n";
    }
}

int main() {
    DictionaryEntry* listHead = nullptr;
    DictionaryEntry* newListHead = nullptr;
    string filename, eng, ukr;
    int choice;

    while (true) {
        cout << "1. Add or Update Entry\n";
        cout << "2. Remove Entry\n";
        cout << "3. Print Initial Dictionary\n";
        cout << "4. Load from file\n";
        cout << "5. Save to file\n";
        cout << "6. Transfer entries based on max access to new list and print\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer

        switch (choice) {
        case 1:
            cout << "Enter English word: ";
            if (!readLine(eng)) continue;
            cout << "Enter Ukrainian word: ";
            if (!readLine(ukr)) continue;
            addOrUpdateEntry(listHead, eng, ukr, 1);  // Assuming an access counts as an addition
            break;
        case 2:
            cout << "Enter English word to remove: ";
            if (!readLine(eng)) continue;
            removeEntry(listHead, eng);
            break;
        case 3:
            cout << "Initial Dictionary (List):\n";
            printDictionary(listHead);
            break;
        case 4:
            cout << "Enter filename to load from: ";
            if (!readLine(filename)) continue;
            loadDictionaryFromFile(listHead, filename);
            break;
        case 5:
            cout << "Enter filename to save to: ";
            if (!readLine(filename)) continue;
            saveDictionaryToFile(listHead, filename);
            break;
        case 6:
            cout << "Enter filename to save transferred entry: ";
            if (!readLine(filename)) continue;
            printAndTransfer(listHead, newListHead, filename);
            break;
        case 8:
            cout << "Exiting program.\n";
            deleteDictionary(listHead);
            deleteDictionary(newListHead);
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    return 0;
}
