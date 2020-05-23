set rows := {1..9};
set cols := {1..9};
set vals := {1..9};

param preset_cells[rows * cols * vals] := read "sudoku.txt" as "<1n, 2n, 3n>4n" comment "#" default 0;
var X[rows * cols * vals] binary;
var Y binary;

minimize nothing: Y;
subto cell_constraints: # only one value appears in one cell
    forall <row> in rows:
        forall <col> in cols:
            sum <val> in vals:
                X[row, col, val]
            == 1;

subto row_constraints: # each value appears only once in a row
    forall <row> in rows:
        forall <val> in vals:
            sum <col> in cols:
                X[row, col, val]
            == 1;

subto col_constraints: # each value appears only once in a column
    forall <col> in cols:
        forall <val> in vals:
            sum <row> in rows:
                X[row, col, val]
            == 1;

subto square_constraints: # each value appears once 3x3 squares
    forall <val> in vals:
        forall <i_row> in {1, 4, 7}:
            forall <i_col> in {1, 4, 7}:
                sum <d_row, d_col> in {0..2} * {0..2}:
                    X[i_row + d_row, i_col + d_col, val]
                == 1;

subto preset_cells: # there are cells that have predetermined value
    forall <row, col, val> in rows * cols * vals:
        X[row, col, val] >= preset_cells[row, col, val];


            