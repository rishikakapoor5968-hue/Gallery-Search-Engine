#ifndef PRIORITYRANKING_H
#define PRIORITYRANKING_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include "GalleryManager.h"  // ✅ use your Metadata struct

using namespace std;

struct RankedImage {
    Metadata image;
    int score;
};

class PriorityRanking {
    vector<Metadata> data;
    vector<RankedImage> heap;

    // Convert text to lowercase
    string toLower(string s) {
        for (int i = 0; i < s.length(); i++) {
            s[i] = tolower((unsigned char)s[i]);
        }
        return s;
    }

    // Calculate image relevance score
    int calculateScore(Metadata m, string keyword) {
        int score = 0;

        string title = toLower(m.title);
        string tags = toLower(m.tags);
        string desc = toLower(m.description);
        string file = toLower(m.fileName);

        keyword = toLower(keyword);

        // Higher weight for matching tags
        if (tags.find(keyword) != string::npos)
            score += 10;

        // Title match
        if (title.find(keyword) != string::npos)
            score += 5;

        // Description match
        if (desc.find(keyword) != string::npos)
            score += 3;

        // Filename match
        if (file.find(keyword) != string::npos)
            score += 1;

        return score;
    }

    // Maintain max-heap property
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (heap[parent].score >= heap[index].score)
                break;

            swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int n = heap.size();

        while (true) {
            int largest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < n &&
                heap[left].score > heap[largest].score)
                largest = left;

            if (right < n &&
                heap[right].score > heap[largest].score)
                largest = right;

            if (largest == index)
                break;

            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

    // Insert result into priority queue
    void push(RankedImage item) {
        heap.push_back(item);
        heapifyUp(heap.size() - 1);
    }

    // Remove and return highest-priority result
    RankedImage pop() {
        RankedImage top = heap[0];

        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty())
            heapifyDown(0);

        return top;
    }

public:

    // Receive gallery metadata
    void buildRanking(vector<Metadata> galleryData) {
        data = galleryData;
    }

    // Search and rank images
    void rankImages(string keyword) {
        heap.clear();

        for (int i = 0; i < data.size(); i++) {
            int score = calculateScore(data[i], keyword);

            if (score > 0) {
                RankedImage item;
                item.image = data[i];
                item.score = score;

                push(item);
            }
        }

        if (heap.empty()) {
            cout << "No matching images found." << endl;
            return;
        }

        cout << "\n--- Ranked Image Results ---\n";

        int rank = 1;

        while (!heap.empty()) {
            RankedImage result = pop();

            cout << "\nRank: " << rank++;
            cout << "\nID: " << result.image.id;
            cout << "\nTitle: " << result.image.title;
            cout << "\nTags: " << result.image.tags;
            cout << "\nRelevance Score: "
                 << result.score << endl;
        }
    }
};

#endif