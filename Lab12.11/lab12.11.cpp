#include <iostream>
#include <fstream>
#include <string>
#include <limits>

#ifndef DICTIONARY_ENTRY_H
#define DICTIONARY_ENTRY_H

struct DictionaryEntry {
    std::string englishWord;
    std::string ukrainianWord;
    int accessCount;
    int height;

    DictionaryEntry* next;  // For linked list
    DictionaryEntry* left;  // For AVL tree
    DictionaryEntry* right; // For AVL tree

    DictionaryEntry(std::string eng, std::string ukr, int count)
        : englishWord(eng), ukrainianWord(ukr), accessCount(count), next(nullptr), left(nullptr), right(nullptr), height(1) {}
};

void deleteList(DictionaryEntry* head);
DictionaryEntry* findAndRemoveMax(DictionaryEntry*& head);
void deleteTree(DictionaryEntry* root);
void rebuildDictionary(DictionaryEntry*& oldHead, DictionaryEntry*& newHead);
void printBinaryTree(const DictionaryEntry* root);
void printList(const DictionaryEntry* head);
void saveDictionaryToFile(DictionaryEntry* head, const std::string& filename);
void loadDictionaryFromFile(DictionaryEntry*& head, const std::string& filename);
DictionaryEntry* insertInAVLTree(DictionaryEntry* node, DictionaryEntry* newEntry);
void insertInOrder(DictionaryEntry*& head, DictionaryEntry* newEntry);
void addOrUpdateEntry(DictionaryEntry*& head, const std::string& eng, const std::string& ukr, int count = 0);
void removeEntry(DictionaryEntry*& head, const std::string& eng);
bool readInt(int& value);
bool readLine(std::string& line);
void clearInputBuffer();

int height(DictionaryEntry* N) {
    if (N == nullptr)
        return 0;
    return N->height;
}

int getBalance(DictionaryEntry* N) {
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}

DictionaryEntry* rightRotate(DictionaryEntry* y) {
    DictionaryEntry* x = y->left;
    DictionaryEntry* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

DictionaryEntry* leftRotate(DictionaryEntry* x) {
    DictionaryEntry* y = x->right;
    DictionaryEntry* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

DictionaryEntry* insertInAVLTree(DictionaryEntry* node, DictionaryEntry* newEntry) {
    if (node == nullptr)
        return newEntry;

    if (newEntry->englishWord < node->englishWord)
        node->left = insertInAVLTree(node->left, newEntry);
    else if (newEntry->englishWord > node->englishWord)
        node->right = insertInAVLTree(node->right, newEntry);
    else
        return node;

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && newEntry->englishWord < node->left->englishWord)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && newEntry->englishWord > node->right->englishWord)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && newEntry->englishWord > node->left->englishWord) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && newEntry->englishWord < node->right->englishWord) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void printBinaryTree(const DictionaryEntry* root) {
    if (!root) return;
    printBinaryTree(root->left);
    std::cout << root->englishWord << " - " << root->ukrainianWord << " (" << root->accessCount << ")\n";
    printBinaryTree(root->right);
}

void deleteTree(DictionaryEntry* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void rebuildDictionary(DictionaryEntry*& oldHead, DictionaryEntry*& newHead) {
    DictionaryEntry* maxNode;
    while ((maxNode = findAndRemoveMax(oldHead)) != nullptr) {
        newHead = insertInAVLTree(newHead, maxNode);
    }
}

void insertInOrder(DictionaryEntry*& head, DictionaryEntry* newEntry) {
    if (!head || newEntry->englishWord < head->englishWord) {
        newEntry->next = head;
        head = newEntry;
    }
    else {
        DictionaryEntry* current = head;
        while (current->next && current->next->englishWord < newEntry->englishWord) {
            current = current->next;
        }
        newEntry->next = current->next;
        current->next = newEntry;
    }
}

void printList(const DictionaryEntry* head) {
    while (head) {
        std::cout << head->englishWord << " - " << head->ukrainianWord << " (" << head->accessCount << ")\n";
        head = head->next;
    }
}

DictionaryEntry* findAndRemoveMax(DictionaryEntry*& head) {
    if (!head) return nullptr;

    DictionaryEntry* current = head, * maxNode = head, * prev = nullptr, * maxPrev = nullptr;
    while (current) {
        if (!maxNode || current->accessCount > maxNode->accessCount) {
            maxNode = current;
            maxPrev = prev;
        }
        prev = current;
        current = current->next;
    }
    if (maxNode == head) {
        head = head->next;
    }
    else if (maxPrev) {
        maxPrev->next = maxNode->next;
    }
    maxNode->next = nullptr;
    return maxNode;
}

void addOrUpdateEntry(DictionaryEntry*& head, const std::string& eng, const std::string& ukr, int count) {
    DictionaryEntry* current = head;
    while (current) {
        if (current->englishWord == eng) {
            current->ukrainianWord = ukr;
            current->accessCount += count;  // зб≥льшуЇмо л≥чильник на передане значенн€
            return;
        }
        current = current->next;
    }
    // якщо слово не було знайдено, створюЇмо новий елемент з переданою к≥льк≥стю звернень
    DictionaryEntry* newEntry = new DictionaryEntry(eng, ukr, count);
    insertInOrder(head, newEntry);
}

#endif // DICTIONARY_ENTRY_H

void removeEntry(DictionaryEntry*& head, const std::string& eng) {
    DictionaryEntry* current = head, * prev = nullptr;
    while (current) {
        if (current->englishWord == eng) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                head = head->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void saveDictionaryToFile(DictionaryEntry* head, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file for writing.\n";
        return;
    }
    DictionaryEntry* current = head;
    while (current) {
        file << current->englishWord << " " << current->ukrainianWord << " " << current->accessCount << "\n";
        current = current->next;
    }
    file.close();
}

void loadDictionaryFromFile(DictionaryEntry*& head, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file for reading.\n";
        return;
    }

    std::cout << "Loading from file will overwrite the current dictionary. Continue? (y/n): ";
    char response;
    std::cin >> response;
    clearInputBuffer(); // Clear the input buffer to handle next inputs properly
    if (response == 'y' || response == 'Y') {
        deleteList(head); // Clears the existing list
        head = nullptr;
    }
    else {
        return; // Exit the function if the user chooses not to overwrite
    }

    std::string eng, ukr;
    int count;
    while (file >> eng >> ukr >> count) { // Correctly formatted input assumed
        addOrUpdateEntry(head, eng, ukr, count);
    }
    file.close();
    std::cout << "Dictionary loaded successfully.\n";
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool readInt(int& value) {
    std::cin >> value;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

bool readLine(std::string& line) {
    std::getline(std::cin, line);
    return !line.empty();
}

void deleteList(DictionaryEntry* head) {
    while (head) {
        DictionaryEntry* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    DictionaryEntry* listHead = nullptr;
    DictionaryEntry* treeHead = nullptr;
    std::string filename, eng, ukr;
    int choice;

    while (true) {
        std::cout << "1. Add or Update Entry\n";
        std::cout << "2. Remove Entry\n";
        std::cout << "3. Print Dictionary (List)\n";
        std::cout << "4. Load from file\n";
        std::cout << "5. Save to file\n";
        std::cout << "6. Rebuild Dictionary (to Tree)\n";
        std::cout << "7. Print Dictionary (Tree)\n";
        std::cout << "8. Exit\n";
        if (!readInt(choice)) {
            std::cout << "Invalid input. Please try again.\n";
            continue;
        }

        switch (choice) {
        case 1:
            std::cout << "Enter English word: ";
            if (!readLine(eng)) continue;
            std::cout << "Enter Ukrainian word: ";
            if (!readLine(ukr)) continue;
            addOrUpdateEntry(listHead, eng, ukr);
            break;
        case 2:
            std::cout << "Enter English word to remove: ";
            if (!readLine(eng)) continue;
            removeEntry(listHead, eng);
            break;
        case 3:
            std::cout << "Initial Dictionary (List):\n";
            printList(listHead);
            break;
        case 4:
            std::cout << "Enter filename to load from: ";
            if (!readLine(filename)) continue;
            loadDictionaryFromFile(listHead, filename);
            break;
        case 5:
            std::cout << "Enter filename to save to: ";
            if (!readLine(filename)) continue;
            saveDictionaryToFile(listHead, filename);
            break;
        case 6:
            rebuildDictionary(listHead, treeHead);
            deleteList(listHead);
            listHead = nullptr;
            break;
        case 7:
            printBinaryTree(treeHead);
            break;
        case 8:
            deleteTree(treeHead);
            std::cout << "Exiting program.\n";
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    return 0;
}