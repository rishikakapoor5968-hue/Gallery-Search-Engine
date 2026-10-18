// PBL PROJECT - Module 2 : Search Engine
// Hash Table for Fast Search + Trie for Keyword/Prefix Search

#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

// ====================================================
//                  HASH TABLE
// ====================================================

class HashTable {

    struct HashNode {
        int id;
        Metadata data;
        HashNode* next;

        HashNode(int i, Metadata m) {
            id = i;
            data = m;
            next = NULL;
        }
    };

    static const int SIZE = 10;

    HashNode* table[SIZE];

    // Hash function
    int hashFunction(int id) {
        return id % SIZE;
    }

public:

    // Constructor
    HashTable() {
        for(int i = 0; i < SIZE; i++) {
            table[i] = NULL;
        }
    }


    // Insert image into Hash Table
    void insert(Metadata m) {

        int index = hashFunction(m.id);

        HashNode* newNode = new HashNode(m.id, m);

        // Collision handling using chaining
        if(table[index] == NULL) {
            table[index] = newNode;
        }
        else {

            HashNode* temp = table[index];

            while(temp->next != NULL) {
                temp = temp->next;
            }

            temp->next = newNode;
        }
    }


    // Search image by ID
    Metadata* search(int id) {

        int index = hashFunction(id);

        HashNode* temp = table[index];

        while(temp != NULL) {

            if(temp->id == id) {
                return &(temp->data);
            }

            temp = temp->next;
        }

        return NULL;
    }


    // Clear Hash Table
    void clear() {

        for(int i = 0; i < SIZE; i++) {

            HashNode* temp = table[i];

            while(temp != NULL) {

                HashNode* temp2 = temp;
                temp = temp->next;

                delete temp2;
            }

            table[i] = NULL;
        }
    }


    // Destructor
    ~HashTable() {
        clear();
    }
};


// ====================================================
//                       TRIE
// ====================================================

class Trie {

    struct TrieNode {

        TrieNode* child[26];

        vector<int> imageIDs;

        bool isEnd;

        TrieNode() {

            isEnd = false;

            for(int i = 0; i < 26; i++) {
                child[i] = NULL;
            }
        }
    };

    TrieNode* root;


    // Convert uppercase character to lowercase
    char toLower(char ch) {

        if(ch >= 'A' && ch <= 'Z') {
            return ch + 32;
        }

        return ch;
    }


    // Insert one keyword
    void insertWord(string word, int imageID) {

        TrieNode* temp = root;

        for(int i = 0; i < word.length(); i++) {

            char ch = toLower(word[i]);

            // Ignore characters other than a-z
            if(ch < 'a' || ch > 'z') {
                continue;
            }

            int index = ch - 'a';

            if(temp->child[index] == NULL) {
                temp->child[index] = new TrieNode();
            }

            temp = temp->child[index];
        }

        temp->isEnd = true;

        temp->imageIDs.push_back(imageID);
    }


    // Delete all nodes of Trie
    void deleteTrie(TrieNode* node) {

        if(node == NULL)
            return;

        for(int i = 0; i < 26; i++) {
            deleteTrie(node->child[i]);
        }

        delete node;
    }


    // Collect image IDs from a prefix
    void collectIDs(TrieNode* node, vector<int>& result) {

        if(node == NULL)
            return;

        // If this is the end of a word
        if(node->isEnd) {

            for(int i = 0; i < node->imageIDs.size(); i++) {

                result.push_back(node->imageIDs[i]);
            }
        }


        // Visit all children
        for(int i = 0; i < 26; i++) {

            if(node->child[i] != NULL) {

                collectIDs(node->child[i], result);
            }
        }
    }


public:

    Trie() {

        root = new TrieNode();
    }


    // Insert keywords from one image
    void insertTags(string tags, int imageID) {

        string word = "";

        for(int i = 0; i <= tags.length(); i++) {

            if(i == tags.length() || tags[i] == ',' || tags[i] == ' ') {

                if(word.length() > 0) {

                    insertWord(word, imageID);

                    word = "";
                }
            }

            else {

                word += tags[i];
            }
        }
    }


    // Search using keyword or prefix
    vector<int> searchPrefix(string prefix) {

        vector<int> result;

        TrieNode* temp = root;


        for(int i = 0; i < prefix.length(); i++) {

            char ch = toLower(prefix[i]);

            if(ch < 'a' || ch > 'z') {
                continue;
            }

            int index = ch - 'a';

            if(temp->child[index] == NULL) {

                return result;
            }

            temp = temp->child[index];
        }


        // Collect all images having this prefix
        collectIDs(temp, result);

        return result;
    }


    // Clear Trie and create a new empty Trie
    void clear() {

        deleteTrie(root);

        root = new TrieNode();
    }


    ~Trie() {

        deleteTrie(root);
    }
};


// ====================================================
//                  SEARCH ENGINE
// ====================================================

class SearchEngine {

    HashTable hashTable;

    Trie trie;

    vector<Metadata> data;


public:


    // -----------------------------------------------
    // Build Search Engine
    // -----------------------------------------------

    void buildIndex(vector<Metadata> galleryData) {

        data = galleryData;

        // Clear old indexes
        hashTable.clear();
        trie.clear();


        // Insert every image
        for(int i = 0; i < data.size(); i++) {

            // Hash Table
            hashTable.insert(data[i]);


            // Trie
            trie.insertTags(data[i].tags, data[i].id);
        }

        cout << "Search index created successfully!" << endl;
    }


    // -----------------------------------------------
    // Search image by ID using Hash Table
    // -----------------------------------------------

    void searchByID(int id) {

        Metadata* result = hashTable.search(id);

        if(result == NULL) {

            cout << "Image ID not found." << endl;
            return;
        }


        cout << "\n--- Image Found ---" << endl;

        cout << "ID: " << result->id << endl;
        cout << "File Name: " << result->fileName << endl;
        cout << "Title: " << result->title << endl;
        cout << "Tags: " << result->tags << endl;
        cout << "Date: " << result->date << endl;
        cout << "Description: " << result->description << endl;
    }


    // -----------------------------------------------
    // Search using keyword/prefix using Trie
    // -----------------------------------------------

    void searchByKeyword(string keyword) {

        vector<int> result = trie.searchPrefix(keyword);


        if(result.empty()) {

            cout << "No images found." << endl;
            return;
        }


        cout << "\n--- Search Results ---" << endl;


        for(int i = 0; i < result.size(); i++) {

            int id = result[i];


            // Find metadata using Hash Table
            Metadata* image = hashTable.search(id);


            if(image != NULL) {

                cout << "\nID: " << image->id << endl;
                cout << "File Name: " << image->fileName << endl;
                cout << "Title: " << image->title << endl;
                cout << "Tags: " << image->tags << endl;
                cout << "Date: " << image->date << endl;
                cout << "Description: " << image->description << endl;
            }
        }
    }


    // -----------------------------------------------
    // Display Hash Table Search
    // -----------------------------------------------

    void searchMenu() {

        int choice;

        do {

            cout << "\n========== SEARCH ENGINE ==========" << endl;

            cout << "1. Search by Image ID" << endl;
            cout << "2. Search by Keyword / Prefix" << endl;
            cout << "3. Exit" << endl;

            cout << "Enter choice: ";
            cin >> choice;


            if(choice == 1) {

                int id;

                cout << "Enter Image ID: ";
                cin >> id;

                searchByID(id);
            }


            else if(choice == 2) {

                string keyword;

                cout << "Enter keyword/prefix: ";
                cin >> keyword;

                searchByKeyword(keyword);
            }


            else if(choice == 3) {

                cout << "Exiting search..." << endl;
            }


            else {

                cout << "Invalid choice." << endl;
            }

        } while(choice != 3);
    }
};



#endif