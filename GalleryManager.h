// PVL PROJECT - Module 1 : Gallery Manager + Metadata Storage (Final Fixed)

#ifndef GALLERYMANAGER_H
#define GALLERYMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>

using namespace std;

struct Metadata {
    int id;
    char fileName[100];
    char fileType[10]; // image or video
    char title[50];
    char tags[100];
    char date[20];
    char description[100];
};

class GalleryManager {
    vector<Metadata> data;
    string file = "metadata.dat";

    // check file is video or image by extension
    string getMediaType(string fileName) {
        transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
        if (fileName.find(".mp4")!= string::npos || fileName.find(".avi")!= string::npos ||
            fileName.find(".mov")!= string::npos || fileName.find(".mkv")!= string::npos) {
            return "video";
        }
        return "image";
    }

    void loadFile() {
        data.clear();
        ifstream fin(file, ios::binary);
        if(!fin) return;
        Metadata m;
        while(fin.read((char*)&m, sizeof(m))) {
            data.push_back(m);
        }
        fin.close();
    }

    void saveFile() {
        ofstream fout(file, ios::binary | ios::trunc);
        for(auto &item : data) {
            fout.write((char*)&item, sizeof(Metadata));
        }
        fout.close();
    }

public:
    GalleryManager() {
        loadFile();
    }

    // 1. Add Single Image/Video
    bool addImage(string fileName, string title, string tags, string date, string description) {
        if(fileName.empty()) {
            cout << "File name empty hai" << endl;
            return false;
        }
        Metadata m;
        m.id = data.empty()? 1 : data.back().id + 1; // fixed id logic

        strncpy(m.fileName, fileName.c_str(), 99);
        m.fileName[99] = '\0';

        string type = getMediaType(fileName);
        strncpy(m.fileType, type.c_str(), 9);
        m.fileType[9] = '\0';

        strncpy(m.title, title.c_str(), 49);
        m.title[49] = '\0';
        strncpy(m.tags, tags.c_str(), 99);
        m.tags[99] = '\0';
        strncpy(m.date, date.c_str(), 19);
        m.date[19] = '\0';
        strncpy(m.description, description.c_str(), 99);
        m.description[99] = '\0';

        data.push_back(m);
        saveFile();
        cout << type << " added successfully! ID = " << m.id << endl;
        return true;
    }

    // 2. NEW FEATURE - Add Many Files at Once
    void addMultipleImages(vector<string> fileNames, string titlePrefix, string tags, string date, string description) {
        cout << "\nAdding " << fileNames.size() << " files..." << endl;
        for(int i=0; i < fileNames.size(); i++) {
            string title = titlePrefix + " " + to_string(i+1);
            addImage(fileNames[i], title, tags, date, description);
        }
        cout << "All files added!" << endl;
    }

    // 3. View Gallery
    void viewGallery() {
        if(data.empty()) {
            cout << "Gallery empty hai" << endl;
            return;
        }
        cout << "\n--- Gallery - Saari Photos & Videos ---" << endl;
        for(auto &item : data) {
            cout << "ID: " << item.id
                 << " | File: " << item.fileName << " [" << item.fileType << "]"
                 << " | Title: " << item.title
                 << " | Tags: " << item.tags
                 << " | Date: " << item.date << endl;
        }
    }

    // 4. Search by Tag
    vector<Metadata> searchByTag(string tag) {
        vector<Metadata> res;
        for(auto &item : data) {
            string t = item.tags;
            if(t.find(tag)!= string::npos) res.push_back(item);
        }
        return res;
    }

    // 5. Search by Date
    vector<Metadata> searchByDate(string date) {
        vector<Metadata> res;
        for(auto &item : data) {
            string d = item.date;
            if(d.find(date)!= string::npos) res.push_back(item);
        }
        return res;
    }

    // 6. Search by Title
    vector<Metadata> searchByTitle(string title) {
        vector<Metadata> res;
        for(auto &item : data) {
            string t = item.title;
            if(t.find(title)!= string::npos) res.push_back(item);
        }
        return res;
    }

    // 7. Delete
    bool deleteImage(int id) {
        for(int i=0; i<data.size(); i++) {
            if(data[i].id == id) {
                data.erase(data.begin()+i);
                saveFile();
                cout << "ID " << id << " delete ho gaya" << endl;
                return true;
            }
        }
        cout << "ID nahi mila" << endl;
        return false;
    }

    vector<Metadata> getMetadataForOtherModules() {
        return data;
    }
};

#endif