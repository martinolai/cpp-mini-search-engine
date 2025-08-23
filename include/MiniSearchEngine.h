#ifndef MINISEARCHENGINE_H
#define MINISEARCHENGINE_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "Document.h"
#include "SearchResult.h"

using namespace std;

/**
 * MiniSearchEngine: Full-featured search engine implementation
 */
class MiniSearchEngine {
private:
    vector<Document> documents;
    unordered_map<string, unordered_set<int>> invertedIndex;
    unordered_map<int, unordered_map<string, int>> termFrequency;
    unordered_map<string, int> documentFrequency;

    string preprocessText(const string& text);
    vector<string> tokenize(const string& text);
    double calculateTFIDF(const string& term, int docId);
    string generateSnippet(const Document& doc, const vector<string>& queryTerms);

public:
    void addDocument(const string& title, const string& content, const string& url = "");
    vector<SearchResult> search(const string& query, int maxResults = 10);
    void printResults(const vector<SearchResult>& results, const string& query);
    void loadFromFile(const string& filename);
    void printStats();
};

#endif
