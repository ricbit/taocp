#include <iostream>
#include "easyscip.h" 

using namespace std;
using namespace easyscip;

int main() {
  MIPSolver mip;
  vector<vector<Variable>> dice(3);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      dice[i].push_back(mip.integer_variable(1, 6, 0));
    }
  }
  vector<Variable> count;
  for (int i = 0; i < 3; i++) {
    count.push_back(mip.integer_variable(0, 36, 0));
  }
  vector<vector<vector<Variable>>> greater(3, vector<vector<Variable>>(6));
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 6; k++) {
        greater[i][j].push_back(mip.binary_variable(-1));
      }
    }
  }
  // greater[i][j][k] = 1 if dice[i][j] > dice[i+1 mod3][k]
  // 0 <= (dice[i1][k]-dice[i][j] + 6*greater[i][j][k]) < 6
  // A>B => dice[i][j]>dice[i1][k] => entre -1 e -5 => 5 1
  // A=B => dice[i][j]=dice[i1][k] => 0             => 0
  // A<B => dice[i][j]<dice[i1][k] => entre 5 e 1   => 5 1
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 6; k++) {
        Constraint cons = mip.constraint();
        cons.add_variable(dice[i][j], -1);
        cons.add_variable(dice[(i + 1) % 3][k], 1);
        cons.add_variable(greater[i][j][k], 6);
        cons.commit(0, 5);
      }
    }
  }
  // count[i] = sum(greater[i][j][k], i)
  for (int i = 0; i < 3; i++) {
    Constraint cons = mip.constraint();
    cons.add_variable(count[i], 1);
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 6; k++) {
        cons.add_variable(greater[i][j][k], -1);
      }
    }
    cons.commit(0, 0);
  }
  // count[i] > 20 (p(dice) > 5/9)
  for (int i = 0; i < 3; i++) {
    Constraint cons = mip.constraint();
    cons.add_variable(count[i], 1);
    cons.commit(21, 36);
  }
  // break symmetry inside a dice.
  // dice[i][j] <= dice[i][k] k > j
  // -6 <= dice[i][j] - dice[i][k] <= 0
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = j + 1; k < 6; k++) {
        Constraint cons = mip.constraint();
        cons.add_variable(dice[i][j], 1);
        cons.add_variable(dice[i][k], -1);
        cons.commit(-6, 0);
      }
    }
  }
  // break symmetry between dices.
  for (int i = 0; i < 2; i++) {
    Constraint cons = mip.constraint();
    cons.add_variable(dice[i][0], 1);
    cons.add_variable(dice[i+1][0], -1);
    cons.commit(-6, 1);
  }
  Solution solution = mip.solve();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      cout << "dice "  << i << " face " << j 
           << " value " << solution.value(dice[i][j]) << "\n";
    }
  }
  for (int i = 0; i < 3; i++) {
    cout << "count " << i << " value " << solution.value(count[i]) << "\n";
  }

  return 0;
}
