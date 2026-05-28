
# Complexity Analysis

# Native Python Array (PyArray)

## Approximate Operations Formula

T(n) = (U + D + R)n

## Description

Variables:

- C = creation cost
- R = read cost
- U = update cost
- D = delete cost

### Create

- Individual append operations are O(1) amortized
- Creating n particles requires n appends and object constructions

T_C(n) = Cn

### Read

- Reading/rendering requires traversal of all particles

T_R(n) = Rn

### Update

- Each particle position and lifetime are modified once

T_U(n) = Un

### Delete

Deletion consists of:

1. Traversal
2. Survival check
3. Append surviving particles to a new list
4. Replace old list reference

T_D(n) = Tn + As

where:

- T = traversal cost
- A = append cost
- s = surviving particles

Since:

s ≤ n

T_D(n) = O(n)

---

# Linked List

## Approximate Operations Formula

T(n) = (U + T + R)n + Pd

Variables:

- C = creation cost
- R = read cost
- U = update cost
- T = traversal cost
- P = pointer reassignment cost

### Create

- Tail insertion allows O(1) insertion per node
- Creating n particles requires n node allocations

T_C(n) = Cn

### Read

T_R(n) = Rn

### Update

T_U(n) = Un

### Delete

Deletion consists of:

1. Traversal to locate nodes
2. Pointer reassignment

T_D(n) = Tn + Pd

where:

- d = deleted particles

Since:

d ≤ n

T_D(n) = O(n)

---

# NumPy Vectorized Array (VectArray)

## Approximate Operations Formula

T(n) = (U + M + F + R)n

Variables:

- C = initialization cost
- U = vectorized update cost
- M = mask generation cost
- F = filtering/copy cost
- R = rendering/read cost

### Create

- Bulk allocation and initialization of arrays

T_C(n) = Cn

### Read

- Reading/rendering traverses all particles

T_R(n) = Rn

### Update

- Vectorized operations are applied simultaneously across arrays

T_U(n) = Un

### Delete

Deletion consists of:

1. Boolean mask creation
2. Array filtering
3. Array reconstruction

T_D(n) = Mn + Fa

where:

- a = surviving particles

Since:

a ≤ n

T_D(n) = O(n)