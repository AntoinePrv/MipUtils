# MipUtils
Utilities to work with MIP files.

Utilities make use of Cplex which is a commercial software.

## Mip Convert
Use Cplex to convert between different file type. The format is inferred by the
file extension by Cplex.

The usage is the following:
```
mip-convert input-file output-file
```

## Mip Coefficients
Extracts all information from the Mip and output it in a numerical only format.

### Usage
The usage is the following:
```
mip-coefs [options] input-file [output-file]
Allowed arguments:
  -h [ --help ]            Produce this help message.
  -i [ --input-file ] arg  Specifies input file.
  -s [ --sparse ]          Write solution in sparse format.
  -o [ --output-file ] arg Specifies output file for the coefficients.
```

### Output macro structure
The output has the following structure:
  - First line is the number of rows (constraints) and the number of variables
    separated by a comma.
  - Then come the objective on one line, in either sparse or dense format.
  - Next, there is one line per constraint, in either sparse or dense format.
    The difference with the objective is the two last elements of the comma
    separated list are used for the (in)equality. The rightmost value is the
    right hand side of the constraint. The value just before encodes the
    (in)equality.
    - `-1` for a lower bound (`<=`)
    - `0` for an equality (`=`)
    - `1` for an upper bound (`>=`)
  - Finally, we have the description of every variable, one variable per line.
    The type, the lower bound, and the upper bound are comma separated.
    The type is encoded with:
    -  `1` are integer variables
    -  `2` are continuous variables
    -  `3` are binary variables

### Sparse and dense

In a dense representation, all coefficients are separated by a comma. No
coefficients are omitted so this representation can take a lot of space when
there are a lot of zeros. Its also easier to understand.

In a sparse representation, we output only the non-zero coefficients. We output
`index:value` in a comma separated list. This representation can save a lot of
space.
