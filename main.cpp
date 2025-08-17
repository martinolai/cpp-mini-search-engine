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

using namespace std;

/**
 * Document: Represents a searchable document with metadata
 * Core concept: Data encapsulation for document storage systems
 */
class Document {
public:
    int id;
    string title;
    string content;
    string url;
    
    Document(int id, const string& title, const string& content, const string& url = "")
        : id(id), title(title), content(content), url(url) {}
};

/**
 * SearchResult: Encapsulates search results with relevance scoring
 * Used for ranking and displaying search outcomes to users
 */
class SearchResult {
public:
    int documentId;
    double score;      // TF-IDF relevance score
    string title;
    string snippet;    // Auto-generated content preview
    string url;
    
    SearchResult() : documentId(0), score(0.0), title(""), snippet(""), url("") {}
    
    SearchResult(int id, double s, const string& t, const string& snip, const string& u = "")
        : documentId(id), score(s), title(t), snippet(snip), url(u) {}
};
