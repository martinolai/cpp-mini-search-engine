# Mini Search Engine C++

A lightweight, educational search engine implementation in C++ that demonstrates core information retrieval concepts including indexing, TF-IDF scoring, and result ranking.

## 🚀 Features

- **Inverted Index**: Fast document retrieval using hash-based inverted indexing
- **TF-IDF Scoring**: Industry-standard Term Frequency - Inverse Document Frequency algorithm
- **Smart Preprocessing**: Text normalization, tokenization, and stop-word filtering
- **Snippet Generation**: Automatic excerpt creation with query term context
- **Interactive Search**: Real-time query processing with ranked results
- **File Support**: Load documents from external text files
- **Performance Statistics**: Index size and document count reporting

## 🏗️ Architecture

### Core Components

#### 1. Document Class
```cpp
class Document {
    int id;           // Unique document identifier
    string title;     // Document title (weighted higher in scoring)
    string content;   // Full document content
    string url;       // Optional URL reference
};
```

#### 2. SearchResult Class
```cpp
class SearchResult {
    int documentId;   // Reference to source document
    double score;     // TF-IDF relevance score
    string title;     // Document title
    string snippet;   // Generated excerpt with context
    string url;       // Document URL
};
```

#### 3. MiniSearchEngine Class
The main engine containing:
- **Inverted Index**: `unordered_map<string, unordered_set<int>>`
- **Term Frequencies**: `unordered_map<int, unordered_map<string, int>>`
- **Document Frequencies**: `unordered_map<string, int>`

## 🧮 Algorithm Implementation

### TF-IDF Calculation

**Term Frequency (TF)**:
```
TF(term, doc) = count(term in doc)
```

**Inverse Document Frequency (IDF)**:
```
IDF(term) = log(total_documents / documents_containing_term)
```

**Final Score**:
```
Score(term, doc) = TF(term, doc) × IDF(term)
```

### Text Processing Pipeline

1. **Preprocessing**: Convert to lowercase, remove punctuation
2. **Tokenization**: Split into individual words
3. **Filtering**: Remove words shorter than 3 characters
4. **Indexing**: Build inverted index and calculate frequencies

### Search Process

1. **Query Processing**: Tokenize and preprocess search query
2. **Document Matching**: Find documents containing query terms using inverted index
3. **Score Calculation**: Compute TF-IDF scores for each matching document
4. **Ranking**: Sort results by descending relevance score
5. **Snippet Generation**: Create contextual excerpts around query terms

## 🔧 Technical Implementation

### Data Structures

- **Inverted Index**: `O(1)` term lookup using hash tables
- **Term Frequency Maps**: Nested hash maps for efficient frequency access
- **Result Vectors**: Dynamic arrays for flexible result handling

### Performance Characteristics

- **Indexing**: `O(n × m)` where n = documents, m = average document length
- **Search**: `O(k × log(r))` where k = query terms, r = matching documents
- **Memory**: `O(v × d)` where v = vocabulary size, d = document count

### Title Weighting Strategy

Titles receive **double weight** in the scoring algorithm to reflect their higher semantic importance:

```cpp
// Combine tokens (title gets double weight)
vector<string> allTokens = titleTokens;
allTokens.insert(allTokens.end(), titleTokens.begin(), titleTokens.end());
allTokens.insert(allTokens.end(), contentTokens.begin(), contentTokens.end());
```

## 🚀 Getting Started

### Prerequisites

- C++11 or later compiler (g++, clang++, MSVC)
- Standard C++ libraries

### Compilation

```bash
g++ -o search_engine main.cpp -std=c++11
```

For optimized builds:
```bash
g++ -O3 -o search_engine main.cpp -std=c++11
```

### Running

```bash
./search_engine
```

### Sample Usage

```
=== Search Engine Statistics ===
Indexed documents: 5
Unique terms: 89
=================================

Enter your search query (or 'quit' to exit): programming algorithms

=== Results for: "programming algorithms" ===
Found 3 results

[1] Search Algorithms
    URL: https://example.com/search-algorithms
    ...algorithms are fundamental in computer science. They include linear search, binary search, and more complex algorithms...
    Score: 2.847

[2] Introduction to C++ Programming  
    URL: https://example.com/cpp-intro
    ...C++ is a powerful and versatile programming language. It is used to develop operating systems, games...
    Score: 1.923

[3] Data Structures in C++
    URL: https://example.com/data-structures
    ...Data structures are essential for organizing and managing data efficiently. In C++ we have arrays...
    Score: 0.693
```

## 📁 File Format Support

Load documents from text files using pipe-separated format:

```
Title 1|Content of document 1|https://example.com/doc1
Title 2|Content of document 2|https://example.com/doc2
Title 3|Content of document 3
```

```cpp
searchEngine.loadFromFile("documents.txt");
```

## 🎯 Use Cases

### Educational
- Learn information retrieval concepts
- Understand search engine fundamentals
- Study TF-IDF implementation
- Explore text processing techniques

### Development
- Prototype search functionality
- Build document management systems
- Create content recommendation engines
- Implement local search features

### Research
- Experiment with ranking algorithms
- Test preprocessing strategies
- Analyze search performance
- Compare scoring methods

## 🔬 Advanced Features

### Customizable Scoring
Modify the TF-IDF calculation by adjusting the weight factors:

```cpp
// Example: Add title boost
if (isInTitle) {
    tf *= 2.0;  // Double weight for title matches
}
```

### Extensible Architecture
Easy to add new features:
- Phrase searching
- Boolean operators
- Fuzzy matching
- Synonym expansion
- Custom filters

## 📊 Performance Considerations

### Scalability Limits
- **Documents**: Efficiently handles thousands of documents
- **Memory Usage**: Approximately 1-2MB per 1000 documents
- **Search Speed**: Sub-millisecond response for typical queries

### Optimization Opportunities
- Implement tf-idf normalization
- Add result caching
- Use memory-mapped files for large datasets
- Implement parallel indexing

## 🤝 Contributing

This is an educational project designed to demonstrate search engine concepts. Potential improvements:

- [ ] Add boolean query operators (AND, OR, NOT)
- [ ] Implement phrase searching with quotes
- [ ] Add fuzzy string matching
- [ ] Create web interface
- [ ] Implement result caching
- [ ] Add multi-language support
- [ ] Optimize memory usage
- [ ] Add unit tests

## 📚 Learning Resources

### Information Retrieval
- "Introduction to Information Retrieval" by Manning, Raghavan, and Schütze
- "Modern Information Retrieval" by Baeza-Yates and Ribeiro-Neto

### Algorithm Background
- TF-IDF: Term Frequency-Inverse Document Frequency
- Inverted Index: Core data structure for search engines
- Text Processing: Tokenization and normalization techniques

## 📄 License

This project is open source and available under the MIT License.

## 🙋‍♂️ Support

For questions about the implementation or to report issues, please create an issue in the repository.
