#include "include/MiniSearchEngine.h"

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
