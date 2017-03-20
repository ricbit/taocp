#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

typedef unsigned int uint;

struct SATData {
  SATData(uint variables_, uint literals_)
      : variables(variables_), literals(literals_), 
        size(2 + 2 * variables + literals),
        L(size), F(size), B(size), C(size) {
  }
  uint variables, literals, size;
  vector<uint> L, F, B, C;
};

class SATParser {
 public:
  SATData parse(auto& istream) {
    string line;
    // Skip header
    getline(istream, line); 
    // Read all clauses.
    while (getline(istream, line)) {
      istringstream iss(line);
      vector<uint> clause;
      for (string var; getline(iss, var, ' ');) {
        if (var.empty()) {
          continue;
        }
        bool flip = var[0] == '~';
        if (flip) {
          var = var.substr(1, string::npos);
        }
        uint index = getvar(var);
        clause.push_back(2 * index + (flip ? 1 : 0));
        literals++;
      }
      clauses.push_back(clause);
    }
    cout << "Variables: " << name.size() << "\n";
    cout << "Clauses: " << clauses.size() << "\n";
    cout << "Literals: " << literals << "\n";
    return build();
  }

  SATData build() {
    SATData data(name.size(), literals);
    vector<int> varcount(name.size(), 0);
    int pos = data.L.size() - 1;
    int clause_number = 1;
    // Fill vectors L and C.
    for (auto clause : clauses) {
      sort(clause.begin(), clause.end());
      for (auto literal : clause) {
        data.C[literal]++;
        data.C[pos] = clause_number;
        data.L[pos] = literal;
        pos--;
      }
      clause_number++;
    }
    // Fill vector F.
    for (uint literal = 2; literal <= 2 + 2 * name.size() - 1; literal++) {
      int last = literal;
      for (uint pos = data.L.size() - 1; pos >= 2 + 2 * name.size(); pos--) {
        if (data.L[pos] == literal) {
          data.B[pos] = last;
          last = pos;
        }
      }
      data.B[literal] = last;
    }
    // Fill vector B.
    for (uint literal = 2; literal <= 2 + 2 * name.size() - 1; literal++) {
      int last = literal;
      for (uint pos = 2 + 2 * name.size(); pos < data.L.size(); pos++) {
        if (data.L[pos] == literal) {
          data.F[pos] = last;
          last = pos;
        }
      }
      data.F[literal] = last;
    }
    return data;
  }

  uint getvar(const string& var) {
    auto it = name.find(var);
    if (it == name.end()) {
      int index = 1 + name.size();
      value[index] = var;
      return name[var] = index;
    } else {
      return it->second;
    }
  }
  unordered_map<string, uint> name;
  unordered_map<uint, string> value;
  vector<vector<uint>> clauses;
  uint literals{0};
};

class Random {
 public:
  int rand(int a, int b) {
    return uniform_int_distribution<int>(a, b)(generator);
  }
  default_random_engine generator{
        static_cast<long unsigned int>(
            chrono::high_resolution_clock::now().time_since_epoch().count())};
};

Random rnd;

int main() {
  SATData data = SATParser().parse(cin);
  for (uint s = 0; s < data.C.size(); s++) {
    cout << s << " " << data.L[s] << " " << data.F[s] << " "
         << data.B[s] << " " << data.C[s] << "\n";
  }
  return 0;
}
