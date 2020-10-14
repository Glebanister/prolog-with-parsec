# Prolog checker

Checks prlog program correctness with parsec.

## Install

```bash
git clone https://github.com/Glebanister/prolog-with-parsec --recursive
mkdir build && cd build
cmake ..
make
```

## Test

Empty output if and only if checker have passed all tests.

```bash
./build/test-prolog-checker
```

## Run

```bash
./build/prolog-checker -i <prolog-program-path> [OPTIONS]
```

| Option        | Description                                  | Value  |
|---------------|----------------------------------------------|--------|
| `-i, --input` | Specify input file path     (required)       | string |
| `-h, --help`  | Get help                                     | -      |
| `--atom`      | Check if input is correct atom               | -      |
| `--typeexpr`  | Check if input is correct typeexpr           | -      |
| `--type`      | Check if input is correct type declaration   | -      |
| `--module`    | Check if input is correct module declaration | -      |
| `--relation`  | Check if input is correct relation           | -      |
| `--list`      | Check if input is correct list               | -      |
| `--prog`      | Check if input is correct program            | -      |

## Example

```bash
$ cat stress.pl
module moduleName.
type x a -> b.
a :- b.
$ ./prolog-checker -i stress.pl && cat stress.pl.out
├─ Program
│ ├─ ModuleDecl: 'moduleName'
│ ├─ TypeDeclarationSequence
│ │ ├─ TypeDecl
│ │ │ ├─ TypeDecl: 'x'
│ │ │ ├─ Arrow
│ │ │ │ ├─ Atom
│ │ │ │ │ ├─ Identifier: 'a'
│ │ │ │ │ ├─ AtomSequence
│ │ │ │ ├─ Atom
│ │ │ │ │ ├─ Identifier: 'b'
│ │ │ │ │ ├─ AtomSequence
│ ├─ RelationSequence
│ │ ├─ Relation
│ │ │ ├─ Atom
│ │ │ │ ├─ Identifier: 'a'
│ │ │ │ ├─ AtomSequence
│ │ │ ├─ Atom
│ │ │ │ ├─ Identifier: 'b'
│ │ │ │ ├─ AtomSequence
```
