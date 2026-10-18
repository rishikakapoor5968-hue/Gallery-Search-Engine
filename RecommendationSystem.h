#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include "GalleryManager.h"  // ✅ use your Metadata struct

using namespace std;

class RecommendationSystem {
    unordered_map<string, vector<int>> tagToImages; // tag -> image IDs
    vector<Metadata> data;

public:
    void buildGraph(vector<Metadata> galleryData) {
        data = galleryData;
        tagToImages.clear();

        for (auto &m : data) {
            string tags = m.tags;
            string word = "";
            for (int i = 0; i <= tags.length(); i++) {
                if (i == tags.length() || tags[i] == ',' || tags[i] == ' ') {
                    if (!word.empty()) {
                        tagToImages[word].push_back(m.id);
                        word = "";
                    }
                } else {
                    word += tags[i];
                }
            }
        }
        cout << "Recommendation graph built successfully!\n";
    }

    void recommendByTag(string tag) {
        if (tagToImages.find(tag) == tagToImages.end()) {
            cout << "No recommendations for tag: " << tag << endl;
            return;
        }

        cout << "\n--- Recommendations for tag: " << tag << " ---\n";
        for (int id : tagToImages[tag]) {
            for (auto &m : data) {
                if (m.id == id) {
                    cout << "ID: " << m.id << " | Title: " << m.title
                         << " | Tags: " << m.tags << endl;
                }
            }
        }
    }
};

#endif
