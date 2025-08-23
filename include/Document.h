#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
using namespace std;

/**
 * Document: Represents a searchable document with metadata
 */
class Document {
public:
    int id;
    string title;
    string content;
    string url;

    Document(int id, const string& title, const string& content, const string& url = "");
};

#endif

