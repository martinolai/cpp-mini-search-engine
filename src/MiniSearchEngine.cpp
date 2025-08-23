#include "../include/MiniSearchEngine.h"

string MiniSearchEngine::preprocessText(const string& text) {
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

vector<string> MiniSearchEngine::tokenize(const string& text) {
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

double MiniSearchEngine::calculateTFIDF(const string& term, int docId) {
    auto termIter = termFrequency[docId].find(term);
    if (termIter == termFrequency[docId].end()) {
        return 0.0;
    }
    double tf = static_cast<double>(termFrequency[docId][term]);
    double idf = log(static_cast<double>(documents.size()) /
                    static_cast<double>(documentFrequency[term]));
    return tf * idf;
}

string MiniSearchEngine::generateSnippet(const Document& doc, const vector<string>& queryTerms) {
    string text = doc.content;
    string lowerText = preprocessText(text);

    size_t bestPos = 0;
    for (const string& term : queryTerms) {
        size_t pos = lowerText.find(term);
        if (pos != string::npos) {
            bestPos = pos;
            break;
        }
    }

    size_t start = (bestPos > 75) ? bestPos - 75 : 0;
    size_t length = min(static_cast<size_t>(150), text.length() - start);

    string snippet = text.substr(start, length);
    if (start > 0) snippet = "..." + snippet;
    if (start + length < text.length()) snippet += "...";

    return snippet;
}

void MiniSearchEngine::addDocument(const string& title, const string& content, const string& url) {
    int docId = static_cast<int>(documents.size());
    documents.emplace_back(docId, title, content, url);

    vector<string> titleTokens = tokenize(title);
    vector<string> contentTokens = tokenize(content);

    vector<string> allTokens = titleTokens;
    allTokens.insert(allTokens.end(), titleTokens.begin(), titleTokens.end());
    allTokens.insert(allTokens.end(), contentTokens.begin(), contentTokens.end());

    unordered_set<string> uniqueTerms;
    for (const string& token : allTokens) {
        invertedIndex[token].insert(docId);
        termFrequency[docId][token]++;
        uniqueTerms.insert(token);
    }

    for (const string& term : uniqueTerms) {
        documentFrequency[term]++;
    }
}

vector<SearchResult> MiniSearchEngine::search(const string& query, int maxResults) {
    vector<string> queryTerms = tokenize(query);
    unordered_map<int, double> scores;

    for (const string& term : queryTerms) {
        auto indexIter = invertedIndex.find(term);
        if (indexIter != invertedIndex.end()) {
            for (int docId : indexIter->second) {
                scores[docId] += calculateTFIDF(term, docId);
            }
        }
    }

    vector<SearchResult> results;
    for (const auto& scorePair : scores) {
        int docId = scorePair.first;
        double score = scorePair.second;
        const Document& doc = documents[docId];

        string snippet = generateSnippet(doc, queryTerms);
        results.emplace_back(docId, score, doc.title, snippet, doc.url);
    }

    sort(results.begin(), results.end(),
        [](const SearchResult& a, const SearchResult& b) {
            return a.score > b.score;
        });

    if (results.size() > static_cast<size_t>(maxResults)) {
        results.resize(maxResults);
    }

    return results;
}

void MiniSearchEngine::printResults(const vector<SearchResult>& results, const string& query) {
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

void MiniSearchEngine::loadFromFile(const string& filename) {
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

void MiniSearchEngine::printStats() {
    cout << "\n=== Search Engine Statistics ===" << endl;
    cout << "Indexed documents: " << documents.size() << endl;
    cout << "Unique terms: " << invertedIndex.size() << endl;
    cout << "================================" << endl;
}
