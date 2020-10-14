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
./build/prolog-checker -i <prolog-program-path>
```

## Example

```bash
$ cat example.pl
module moduleName.

type typename1 a -> (b -> c).

x :- a [a, b, [H | T]]; b, c.
$ ./prolog-checker -i example.pl && cat example.pl.out
├─ Program
│ ├─ ModuleDecl: 'moduleName'
│ ├─ TypeDeclarationSequence
│ │ ├─ TypeDecl
│ │ │ ├─ TypeDecl: 'typename1'
│ │ │ ├─ Arrow
│ │ │ │ ├─ Atom
│ │ │ │ │ ├─ Identifier: 'a'
│ │ │ │ │ ├─ AtomSequence
│ │ │ │ ├─ Arrow
│ │ │ │ │ ├─ Atom
│ │ │ │ │ │ ├─ Identifier: 'b'
│ │ │ │ │ │ ├─ AtomSequence
│ │ │ │ │ ├─ Atom
│ │ │ │ │ │ ├─ Identifier: 'c'
│ │ │ │ │ │ ├─ AtomSequence
│ ├─ RelationSequence
│ │ ├─ Relation
│ │ │ ├─ Atom
│ │ │ │ ├─ Identifier: 'x'
│ │ │ │ ├─ AtomSequence
│ │ │ ├─ Disj
│ │ │ │ ├─ Atom
│ │ │ │ │ ├─ Identifier: 'a'
│ │ │ │ │ ├─ AtomSequence
│ │ │ │ │ │ ├─ List
│ │ │ │ │ │ │ ├─ ListItems
│ │ │ │ │ │ │ │ ├─ Atom
│ │ │ │ │ │ │ │ │ ├─ Identifier: 'a'
│ │ │ │ │ │ │ │ │ ├─ AtomSequence
│ │ │ │ │ │ │ │ ├─ Atom
│ │ │ │ │ │ │ │ │ ├─ Identifier: 'b'
│ │ │ │ │ │ │ │ │ ├─ AtomSequence
│ │ │ │ │ │ │ │ ├─ List
│ │ │ │ │ │ │ │ │ ├─ ListHeadTail
│ │ │ │ │ │ │ │ │ │ ├─ Variable: 'H'
│ │ │ │ │ │ │ │ │ │ ├─ Variable: 'T'
│ │ │ │ ├─ Conj
│ │ │ │ │ ├─ Atom
│ │ │ │ │ │ ├─ Identifier: 'b'
│ │ │ │ │ │ ├─ AtomSequence
│ │ │ │ │ ├─ Atom
│ │ │ │ │ │ ├─ Identifier: 'c'
│ │ │ │ │ │ ├─ AtomSequence
```
