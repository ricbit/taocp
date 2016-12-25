#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <random>
#include <chrono>

using namespace std;

typedef unsigned int uint;

const uint FLIP = 1U << 31;

class SATData {
 public:
  void read() {
    string line;
    // Skip header
    getline(cin, line); 
    // Read all clauses.
    while (getline(cin, line)) {
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
        clause.push_back(index | (flip ? 0 : FLIP));
      }
      clauses.push_back(clause);
    }
    cout << "Variables: " << name.size() << "\n";
    cout << "Clauses: " << clauses.size() << "\n";
  }

  uint getvar(const string& var) {
    auto it = name.find(var);
    if (it == name.end()) {
      int index = name.size();
      value[index] = var;
      return name[var] = index;
    } else {
      return it->second;
    }
  }
  unordered_map<string, uint> name;
  unordered_map<uint, string> value;
  vector<vector<uint>> clauses;
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

class AlgorithmP {
 public:
  AlgorithmP(SATData& data_) : 
      data(data_), vars(data_.name.size()), 
      m(data_.clauses.size()), n(data_.name.size()) {
  }
  bool run(int N) {
    for (int i = 0; i < n; i++) {
      vars[i] = static_cast<bool>(rnd.rand(0, 1)); 
    }
    j = 0, t = 0;
    s = 0;
P2:
    if (s == m) {
      //dump();
      return true;
    }
    j = (j + 1) % m;
    if (satisfied(vars, j)) {
      s++;
      goto P2;
    }
    if (t == N) {
      return false;
    }
    int i = rnd.rand(0, data.clauses[j].size() - 1);
    uint v = data.clauses[j][i];
    vars[v & ~FLIP] = v & FLIP;
    s = 1;
    t++;
    goto P2;
  }
  void dump() {
    for (int i = 0; i < n; i++) {
      if (vars[i]) {
        const string s = data.value[i];
        cout << s << " ";
      }
    }
    cout << "\n";
  }
  bool satisfied(const vector<bool>& vars, int clause) {
    for (uint v : data.clauses[clause]) {
      if (!!(v & FLIP) == vars[v & (~FLIP)]) {
        return true;
      }
    }
    return false;
  }
  SATData& data;
  vector<bool> vars;
  int m, n, j, s, t;
};

int main() {
  SATData data;
  data.read();
  int steps = 0;
  int M = 2000000;
  for (int i = 0; i < M; i++) {
    AlgorithmP p(data);
    p.run(20000);
    steps += p.t;
  }
  //cout << (p.run(20000000)? "found" : "not found");
  //cout << " in " << p.t << "\n";
  cout << "average " << double(steps)/double(M) << "\n";
  return 0;
}
