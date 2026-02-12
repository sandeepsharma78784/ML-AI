#include <bits/stdc++.h>
using namespace std;

struct Frame { bool hasElement = false; bool expectElement = true; };

bool isNumberToken(const string &tok) {
    if (tok.empty()) return false;
    size_t i = 0;
    if (tok[i] == '+' || tok[i] == '-') ++i;
    bool seenDigit = false, seenDot = false;
    for (; i < tok.size(); ++i) {
        char c = tok[i];
        if (isdigit((unsigned char)c)) seenDigit = true;
        else if (c == '.') {
            if (seenDot) return false;
            seenDot = true;
        } else return false;
    }
    return seenDigit;
}

struct Result { bool valid; int dimensions; int elements; string reason; };

Result analyze(const string &s) {
    vector<Frame> st;
    int maxDepth = 0;
    int totalElements = 0;
    size_t n = s.size();
    size_t i = 0;
    bool sawAny = false;

    while (i < n) {
        char c = s[i];
        if (isspace((unsigned char)c)) { ++i; continue; }
        sawAny = true;

        if (c == '{') {
            // opening
            if (!st.empty() && !st.back().expectElement) {
                return {false, 0, 0, "Unexpected '{' (missing comma before)"};
            }
            st.push_back(Frame());
            maxDepth = max(maxDepth, (int)st.size());
            ++i;
            continue;
        }

        if (c == '}') {
            if (st.empty()) return {false, 0, 0, "Unmatched '}'"};
            Frame f = st.back(); st.pop_back();
            if (f.expectElement || !f.hasElement) return {false, 0, 0, "Empty or incomplete '{}' block"};
            // mark parent that it received an element
            if (!st.empty()) {
                st.back().hasElement = true;
                st.back().expectElement = false;
            }
            ++i;
            continue;
        }

        if (c == ',') {
            if (st.empty()) return {false, 0, 0, "Comma outside braces"};
            if (st.back().expectElement) return {false, 0, 0, "Unexpected comma (missing element) or consecutive commas"};
            st.back().expectElement = true;
            ++i;
            continue;
        }

        // parse a number token: allow optional leading +/-, digits and at most one dot; require at least one digit
        if (c == '+' || c == '-' || c == '.' || isdigit((unsigned char)c)) {
            // collect token
            size_t j = i;
            if (s[j] == '+' || s[j] == '-') ++j;
            while (j < n && (isdigit((unsigned char)s[j]) || s[j] == '.')) ++j;
            string tok = s.substr(i, j - i);
            if (!isNumberToken(tok)) return {false, 0, 0, "Invalid numeric token: '" + tok + "'"};
            if (st.empty()) return {false, 0, 0, "Number outside braces"};
            if (!st.back().expectElement) return {false, 0, 0, "Missing comma before number or unexpected number position"};
            // accept number
            ++totalElements;
            st.back().hasElement = true;
            st.back().expectElement = false;
            i = j;
            continue;
        }

        // any other character invalid
        return {false, 0, 0, string("Invalid character: '") + c + "'"};
    }


    if (!sawAny) return {false, 0, 0, "Empty input"};
    if (!st.empty()) return {false, 0, 0, "Unclosed '{' braces remain"};
    return {true, maxDepth, totalElements, ""};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    if (!getline(cin, line)) return 0;

    Result r = analyze(line);
    if (!r.valid) {
        cout << "Invalid\nReason: " << r.reason << "\n";
    } else {
        cout << "Valid\nDimensions (max nesting of '{'): " << r.dimensions << "\n";
        cout << "Total numeric elements: " << r.elements << "\n";
    }
    return 0;
}