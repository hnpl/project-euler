set rows := {1..15};
set cols := {1..15};

param weights[rows * cols] := read "input.txt" as "<1n, 2n>3n" comment "#" default 0;
var X[rows * cols] binary;

maximize matrix_sum: 
    sum <row, col> in rows*cols: weights[row, col] * X[row, col];

subto row_constraints: # each row only has one chosen element
    forall <row> in rows:
        sum <col> in cols:
            X[row, col]
        == 1;

subto col_constraints: # each column only has one chosen element
    forall <col> in cols:
        sum <row> in rows:
            X[row, col]
        == 1;