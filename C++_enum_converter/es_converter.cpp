#include <iostream>
#include <iomanip>
#include <climits>
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <stack>
#include <string>
#include <functional>
#include <numeric>
#include <map>
#include <set>
#include <cstdlib>
#include <bitset>
#include <unordered_map>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

#define Loop(i, n) for(int i = 0; i < (int)n; i++)
#define Loop1(i, n) for(int i = 1; i <= (int)n; i++)
#define Loopr(i, n) for(int i = (int)n - 1; i >= 0; i--)
#define Loopr1(i, n) for(int i = (int)n; i >= 1; i--)


#define DISPLAY_WIDTH 60


struct info_t {
  bool valid;
  string type;
  string name;
  vector<string> factors;
};

inline bool is_whitespace(char);
inline bool is_out_of_range(string*, int);
inline bool is_string_t(string);
int first_non_whitespace_index(string*, int);
string trim(string);
info_t parse(string);
string convert(info_t);
void display(string);

int main() {
  while (true) {
    cout << "Put any definition of enum type or vector<string> type:" << endl;
    string s;
    while (true) {
      string t; cin >> t;
      string::size_type pos = t.find(";");
      if (pos != string::npos) {
        s += t.substr(0, pos + 1);
        break;
      }
      else {
        s += t + " ";
      }
    }
    info_t info = parse(s);
    if (info.valid) {
      cout << endl;
      display(convert(info));
      cout << endl;
    }
  }
}

info_t parse(string s) {
  info_t ret;
  ret.valid = false;
  pair<int, int> p = { 0, 0 };
  try {
    s = trim(s);
    // check s is end with ';'
    if (s.length() == 0 || s.back() != ';') throw "invalid use of function parse(string)";
    if (s.substr(p.first, 4) == "enum") {
      // parse enum
      p.first += 4;
      if (!is_whitespace(s[p.first])) throw "parse error";
      ret.type = "enum";
      p.first = first_non_whitespace_index(&s, p.first);
      p.second = s.find('{', p.first);
      if (p.second < 0) throw "parse error";
      ret.name = trim(s.substr(p.first, p.second - p.first));
      if (ret.name.empty()) throw "parse error";
      p.first = p.second + 1;
      while (true) {
        p.second = s.find(',', p.first);
        if (p.second < 0) {
          p.second = s.find('}', p.first);
          if (p.second < 0) throw "parse error";
          else {
            ret.factors.push_back(trim(s.substr(p.first, p.second - p.first)));
            ret.valid = true;
            return ret;
          }
        }
        else {
          ret.factors.push_back(trim(s.substr(p.first, p.second - p.first)));
          p.first = p.second + 1;
        }
      }
    }
    else if (s.substr(p.first, 6) == "vector") {
      // parse vector
      p.first = first_non_whitespace_index(&s, p.first + 6);
      if (s[p.first] != '<') throw "parse error";
      p.first = first_non_whitespace_index(&s, p.first + 1);
      if (s.substr(p.first, 6) != "string") throw "parse error";
      p.first = first_non_whitespace_index(&s, p.first + 6);
      if (s[p.first] != '>') throw "parse error";
      p.first++;
      if (!is_whitespace(s[p.first])) throw "parse error";
      ret.type = "vector<string>";
      p.second = s.find('=', p.first);
      if (p.second < 0) p.second = s.find('(', p.first);
      if (p.second < 0) throw "parse error";
      ret.name = trim(s.substr(p.first, p.second - p.first));
      if (ret.name.empty()) throw "parse error";
      p.first = p.second + 1;
      p.first = s.find('{', p.first);
      p.first++;
      while (true) {
        p.second = s.find(',', p.first);
        if (p.second < 0) {
          p.second = s.find('}', p.first);
          if (p.second < 0) throw "parse error";
          else {
            ret.factors.push_back(trim(s.substr(p.first, p.second - p.first)));
            if (!is_string_t(ret.factors.back())) throw "parse error";
            ret.valid = true;
            return ret;
          }
        }
        else {
          ret.factors.push_back(trim(s.substr(p.first, p.second - p.first)));
          if (!is_string_t(ret.factors.back())) throw "parse error";
          p.first = p.second + 1;
        }
      }
    }
    else throw "unknown type";
  }
  catch (const char* err_message) {
    cout << "Error: " << err_message << "." << endl;
    return ret;
  }
  return ret;
}

string convert(info_t info) {
  string ret;
  if (info.type == "enum") {
    ret += "vector<string> ";
    ret += "str_of_" + info.name + " = { ";
    if (info.factors.size() > 0) {
      ret += "\"" + info.factors[0] + "\"";
      Loop1(i, info.factors.size() - 1) {
        ret += ", \"" + info.factors[i] + "\"";
      }
    }
    ret += " };";
  }
  else if (info.type == "vector<string>") {
    ret += "enum ";
    ret += info.name + "_t { ";
    if (info.factors.size() > 0) {
      ret += info.factors[0].substr(1, info.factors[0].length() - 2);
      Loop1(i, info.factors.size() - 1) {
        ret += ", " + info.factors[i].substr(1, info.factors[i].length() - 2);
      }
    }
    ret += " };";
  }
  else {
    cout << "Error: unknown type of \"" << info.type << "\", please retry." << endl;
  }
  return ret;
}


inline bool is_whitespace(char c) {
  vector<char> whitespace = { ' ', '\t', '\n' };
  Loop(i, whitespace.size()) {
    if (c == whitespace[i]) return true;
  }
  return false;
}

inline bool is_out_of_range(string *sp, int k) {
  if (k < 0) return true;
  else if (k >= (int)sp->length()) return true;
  else return false;
}

inline bool is_string_t(string s) {
  if (s.length() < 2) return false;
  else if (s[0] != '\"' || s.back() != '\"') return false;
  else return true;
}

int first_non_whitespace_index(string *sp, int k) {
  while (true) {
    if (is_out_of_range(sp, k)) return -1;
    else {
      if (is_whitespace((*sp)[k])) k++;
      else return k;
    }
  }
}

string trim(string s) {
  int l = 0, r = s.length() - 1;
  while (true) {
    if (is_out_of_range(&s, l)) return "";
    else if (is_whitespace(s[l])) l++;
    else break;
  }
  while (true) {
    if (is_whitespace(s[r])) r--;
    else break;
  }
  return s.substr(l, r - l + 1);
}

void display(string s) {
  pair<int, int> p = { 0, 0 };
  p.second = s.find('{', p.first);
  cout << s.substr(p.first, p.second - p.first + 1) << endl;
  while (true) {
    p.first = p.second + 1;
    p.second = s.find(',', p.first + DISPLAY_WIDTH);
    if (p.second < 0) {
      p.second = s.find('}', p.first);
      cout << "\t" << s.substr(p.first, p.second - p.first) << endl;
      cout << s.substr(p.second, s.length() - p.second + 1) << endl;
      return;
    }
    else {
      cout << "\t" << s.substr(p.first, p.second - p.first + 1) << endl;
    }
  }
}