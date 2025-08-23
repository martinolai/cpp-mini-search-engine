#include "../include/Document.h"

Document::Document(int id, const string& title, const string& content, const string& url)
    : id(id), title(title), content(content), url(url) {}
