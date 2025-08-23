#include "../include/SearchResult.h"

SearchResult::SearchResult() 
    : documentId(0), score(0.0), title(""), snippet(""), url("") {}

SearchResult::SearchResult(int id, double s, const string& t, const string& snip, const string& u)
    : documentId(id), score(s), title(t), snippet(snip), url(u) {}
