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
};
