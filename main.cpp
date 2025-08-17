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

/**
 * MiniSearchEngine: Full-featured search engine implementation
 * 
 * Key Features:
 * - Inverted index for O(1) term lookups
 * - TF-IDF scoring algorithm for relevance ranking
 * - Smart text preprocessing and tokenization
 * - Context-aware snippet generation
 * - Batch document loading from files
 */
class MiniSearchEngine {
private:
    vector<Document> documents;
    
    // Inverted index: term -> set of document IDs containing the term
    unordered_map<string, unordered_set<int>> invertedIndex;
    
    // Term frequency: document_id -> (term -> frequency count)
    unordered_map<int, unordered_map<string, int>> termFrequency;
    
    // Document frequency: term -> number of documents containing it
    unordered_map<string, int> documentFrequency;
    
    /**
     * Text preprocessing: normalizes text for consistent indexing
     * Converts to lowercase and handles punctuation
     */
    string preprocessText(const string& text) {
        string result;
        for (char c : text) {
            if (isalnum(c) || isspace(c)) {
                result += tolower(c);
            } else {
                result += ' ';
            }
        }
        return result;
    }
    
    /**
     * Tokenization: extracts meaningful terms from text
     * Filters out short words that typically don't add search value
     */
    vector<string> tokenize(const string& text) {
        vector<string> tokens;
        istringstream iss(preprocessText(text));
        string word;
        
        while (iss >> word) {
            if (word.length() > 2) {
                tokens.push_back(word);
            }
        }
        return tokens;
    }
    
    /**
     * TF-IDF Calculation: industry-standard relevance scoring
     * Formula: TF(term, doc) * IDF(term) = frequency * log(total_docs / docs_with_term)
     * Higher scores indicate more relevant documents
     */
    double calculateTFIDF(const string& term, int docId) {
        unordered_map<string, int>::iterator termIter = termFrequency[docId].find(term);
        if (termIter == termFrequency[docId].end()) {
            return 0.0;
        }
        
        double tf = static_cast<double>(termFrequency[docId][term]);
        double idf = log(static_cast<double>(documents.size()) / 
                        static_cast<double>(documentFrequency[term]));
        
        return tf * idf;
    }
    
    /**
     * Smart snippet generation with query context
     * Finds relevant content sections and creates preview text
     */
    string generateSnippet(const Document& doc, const vector<string>& queryTerms) {
        string text = doc.content;
        string lowerText = preprocessText(text);
        
        // Find first occurrence of any query term for context
        size_t bestPos = 0;
        for (const string& term : queryTerms) {
            size_t pos = lowerText.find(term);
            if (pos != string::npos) {
                bestPos = pos;
                break;
            }
        }
        
        // Extract 150-character window around the found term
        size_t start = (bestPos > 75) ? bestPos - 75 : 0;
        size_t length = min(static_cast<size_t>(150), text.length() - start);
        
        string snippet = text.substr(start, length);
        if (start > 0) snippet = "..." + snippet;
        if (start + length < text.length()) snippet += "...";
        
        return snippet;
    }
    
public:
    /**
     * Index a new document for searching
     * Builds inverted index and calculates term frequencies
     * Titles receive double weight for improved relevance
     */
    void addDocument(const string& title, const string& content, const string& url = "") {
        int docId = static_cast<int>(documents.size());
        documents.emplace_back(docId, title, content, url);
        
        vector<string> titleTokens = tokenize(title);
        vector<string> contentTokens = tokenize(content);
        
        // Combine tokens with title weighting (titles are more important)
        vector<string> allTokens = titleTokens;
        allTokens.insert(allTokens.end(), titleTokens.begin(), titleTokens.end()); // Double weight
        allTokens.insert(allTokens.end(), contentTokens.begin(), contentTokens.end());
        
        // Build search index structures
        unordered_set<string> uniqueTerms;
        for (const string& token : allTokens) {
            invertedIndex[token].insert(docId);
            termFrequency[docId][token]++;
            uniqueTerms.insert(token);
        }
        
        // Update document frequency for IDF calculation
        for (const string& term : uniqueTerms) {
            documentFrequency[term]++;
        }
    }
    
    /**
     * Execute search query and return ranked results
     * Uses TF-IDF scoring with result sorting by relevance
     */
    vector<SearchResult> search(const string& query, int maxResults = 10) {
        vector<string> queryTerms = tokenize(query);
        unordered_map<int, double> scores;
        
        // Calculate relevance scores for matching documents
        for (const string& term : queryTerms) {
            unordered_map<string, unordered_set<int>>::iterator indexIter = invertedIndex.find(term);
            if (indexIter != invertedIndex.end()) {
                for (int docId : indexIter->second) {
                    scores[docId] += calculateTFIDF(term, docId);
                }
            }
        }
        
        // Convert to result objects with snippets
        vector<SearchResult> results;
        for (const pair<const int, double>& scorePair : scores) {
            int docId = scorePair.first;
            double score = scorePair.second;
            const Document& doc = documents[docId];
            
            string snippet = generateSnippet(doc, queryTerms);
            results.emplace_back(docId, score, doc.title, snippet, doc.url);
        }
        
        // Sort by relevance score (highest first)
        sort(results.begin(), results.end(), 
             [](const SearchResult& a, const SearchResult& b) {
                 return a.score > b.score;
             });
        
        // Limit number of results
        if (results.size() > static_cast<size_t>(maxResults)) {
            results.resize(maxResults);
        }
        
        return results;
    }
    
    /**
     * Display formatted search results to console
     */
    void printResults(const vector<SearchResult>& results, const string& query) {
        cout << "\n=== Results for: \"" << query << "\" ===" << endl;
        cout << "Found " << results.size() << " results\n" << endl;
        
        for (size_t i = 0; i < results.size(); ++i) {
            const SearchResult& result = results[i];
            cout << "[" << (i + 1) << "] " << result.title << endl;
            if (!result.url.empty()) {
                cout << "    URL: " << result.url << endl;
            }
            cout << "    " << result.snippet << endl;
            cout << "    Score: " << fixed << setprecision(3) 
                 << result.score << endl << endl;
        }
    }
    
    /**
     * Batch load documents from text file
     * Format: title|content|url (pipe-separated values)
     */
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        
        while (getline(file, line)) {
            size_t pos1 = line.find('|');
            if (pos1 == string::npos) continue;
            
            size_t pos2 = line.find('|', pos1 + 1);
            string title = line.substr(0, pos1);
            string content = (pos2 != string::npos) ? 
                            line.substr(pos1 + 1, pos2 - pos1 - 1) : 
                            line.substr(pos1 + 1);
            string url = (pos2 != string::npos) ? line.substr(pos2 + 1) : "";
            
            addDocument(title, content, url);
        }
        file.close();
    }
    
    /**
     * Display search engine statistics
     */
    void printStats() {
        cout << "\n=== Search Engine Statistics ===" << endl;
        cout << "Indexed documents: " << documents.size() << endl;
        cout << "Unique terms: " << invertedIndex.size() << endl;
        cout << "================================" << endl;
    }
};

/**
 * Interactive search engine demo
 * Demonstrates full system functionality with sample data
 */
int main() {
    MiniSearchEngine searchEngine;
    
    // Load sample documents
    searchEngine.addDocument(
        "Introduction to C++ Programming",
        "C++ is a powerful and versatile programming language. It is used to develop operating systems, games, desktop applications and much more. C++ supports object-oriented programming.",
        "https://example.com/cpp-intro"
    );
    
    searchEngine.addDocument(
        "Search Algorithms",
        "Search algorithms are fundamental in computer science. They include linear search, binary search, and more complex algorithms like those used in web search engines.",
        "https://example.com/search-algorithms"
    );
    
    searchEngine.addDocument(
        "Data Structures in C++",
        "Data structures are essential for organizing and managing data efficiently. In C++ we have arrays, vectors, maps, sets and many other useful data structures.",
        "https://example.com/data-structures"
    );
    
    searchEngine.addDocument(
        "Machine Learning with Python",
        "Python is the most popular language for machine learning. Libraries like TensorFlow, PyTorch and scikit-learn make it easy to implement machine learning algorithms.",
        "https://example.com/ml-python"
    );
    
    searchEngine.addDocument(
        "Web Development with JavaScript",
        "JavaScript is essential for modern web development. It allows you to create interactive user interfaces and dynamic web applications. It is used in both frontend and backend.",
        "https://example.com/js-web"
    );
    
    searchEngine.printStats();
    
    // Interactive search loop
    string query;
    while (true) {
        cout << "\nEnter your search query (or 'quit' to exit): ";
        getline(cin, query);
        
        if (query == "quit" || query == "exit") {
            break;
        }
        
        if (query.empty()) {
            continue;
        }
        
        vector<SearchResult> results = searchEngine.search(query);
        searchEngine.printResults(results, query);
    }
    
    cout << "Thank you for using the Mini Search Engine!" << endl;
    return 0;
}
