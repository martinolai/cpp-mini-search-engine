#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <string>
using namespace std;

/**
 * SearchResult: Encapsulates search results with relevance scoring
 */
class SearchResult {
public:
    int documentId;
    double score;      // TF-IDF relevance score
    string title;
    string snippet;    // Auto-generated content preview
    string url;

    SearchResult();
    SearchResult(int id, double s, const string& t, const string& snip, const string& u = "");
};

#endif
