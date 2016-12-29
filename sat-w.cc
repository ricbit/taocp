#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <random>
#include <chrono>
#include <limits>

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
  double real(double a, double b) {
    return uniform_real_distribution<double>(a, b)(generator);
  }
  default_random_engine generator{
        static_cast<long unsigned int>(
            chrono::high_resolution_clock::now().time_since_epoch().count())};
};

Random rnd;

class AlgorithmW {
 public:
  AlgorithmW(SATData& data_, double p_) : 
      data(data_), p(p_),
      m(data_.clauses.size()), n(data_.name.size()),
      x(n), c(n), k(m), f(m) {
  }
  bool run(int N) {
    for (int i = 0; i < n; i++) {
      x[i] = static_cast<bool>(rnd.rand(0, 1)); 
    }
    t = 0;
    update();
W2:
    if (!r) {
      ///dump();
      return true;
    }
    if (t == N) {
      return false;
    }
    int j = f[rnd.rand(0, r - 1)];
    int c = smallest_cost(j);
    int l;
    if (c == 0 || (c >= 1 && rnd.real(0, 1) >= p)) {
      l = choose_from_cost(j, c);
    } else {
      l = rnd.rand(0, data.clauses[j].size() - 1);
    }
    int var = data.clauses[j][l] & ~FLIP;
    x[var] = !x[var];
    update();
    t++;
    goto W2;
  }
  uint choose_from_cost(uint clause, uint cost) {
    uint total = 0;
    for (uint v : data.clauses[clause]) {
      if (cost == c[v & ~FLIP]) {
        total++;
      }
    }
    int pick = rnd.rand(0, total - 1);
    int cur = 0;
    for (int i = 0; i < int(data.clauses[clause].size()); i++) {
      if (cost == c[data.clauses[clause][i] & ~FLIP]) {
        if (cur == pick) {
          return i;
        }
        cur++;
      }
    }
    return 0;
  }
  void update() {
    r = 0;
    fill(c.begin(), c.end(), 0);
    for (int j = 0; j < m; j++) {
      k[j] = count_true_literals(j);
      if (!k[j]) {
        f[r] = j;
        r++;
      } else if (k[j] == 1) {
        increment_cost(j);
      }
    }
  }
  uint smallest_cost(int clause) {
    uint smallest = numeric_limits<uint>::max();
    for (uint v : data.clauses[clause]) {
      if (smallest > c[v & (~FLIP)]) {
        smallest = c[v & (~FLIP)];
      }
    }
    return smallest;
  }
  uint count_true_literals(int clause) {
    uint count = 0;
    for (uint v : data.clauses[clause]) {
      if ((!!(v & FLIP)) == x[v & (~FLIP)]) {
        count++;
      }
    }
    return count;
  }
  void increment_cost(int clause) {
    for (uint v : data.clauses[clause]) {
      if ((!!(v & FLIP)) == x[v & (~FLIP)]) {
        c[v & (~FLIP)]++;
        return;
      }
    }
  }
  void dump() {
    for (int i = 0; i < n; i++) {
      if (x[i]) {
        const string s = data.value[i];
        cout << s << " ";
      }
    }
    cout << "\n";
  }
  bool satisfied(int clause) {
    for (uint v : data.clauses[clause]) {
      if ((!!(v & FLIP)) == x[v & (~FLIP)]) {
        return true;
      }
    }
    return false;
  }
  SATData& data;
  double p;
  int m, n;
  vector<bool> x;
  vector<uint> c, k, f;
  int r, t;
};

int main() {
  SATData data;
  data.read();
  int steps = 0;
  int M = 2000000;
  for (int i = 0; i < M; i++) {
    AlgorithmW w(data, 0.57);
    w.run(2000000);
    steps += w.t;
  }
  //cout << (p.run(20000000)? "found" : "not found");
  //cout << " in " << p.t << "\n";
  cout << "average " << double(steps)/double(M) << "\n";
  return 0;
}
