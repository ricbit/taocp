CLAUSES = [
    [1, 2, -3],
    [2, 3, -4],
    [3, 4, 1],
    [4, -1, 2],
    [-1, -2, 3],
    [-2, -3, 4],
    [-3, -4, -1]
]

def bin4(value):
    return ("0" * 4 + bin(value)[2:])[-4:]

def match_literal(state, literal):
    if literal > 0:
        return state[literal - 1] == "1"
    else:
        return state[-literal - 1] == "0"

solution = []
variables = []
for state in xrange(16):
    binstate = bin4(state)
    variables.append("g" + binstate)
    print binstate
    violated = []
    for clause in CLAUSES:        
        if not any(match_literal(binstate, literal) for literal in clause):
            print clause
            next_states = []
            for literal in clause:
                next_state = list(binstate)
                next_state[abs(literal) - 1] = "1" if literal > 0 else "0"
                next_states.append("g" + "".join(next_state))
            violated.append(
                "g" + binstate + " == z/3(" + "+".join(next_states) + ")")
    if violated:
        solution.append("".join(violated))
    else:
        solution.append("g" + binstate + " == 1")
print "Solve[{" + " , ".join(solution) + "}, {" + ",".join(variables) + "}]"
print "+".join(variables)
