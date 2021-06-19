# nicoLISP

A very basic 'Lisp' interpreter, built so I can learn how to use C better.

## How to setup

Clone this repo (and its submodules):

```bash
$ git clone --recursive https://github.com/nicolaiskye/nicolisp
$ cd nicolisp
```

Build the project:

```bash
make
```

Run the REPL

```bash
./nicolisp
```

*Optional*
Clean up build files

```bash
make clean
```

## How to use nicoLISP

At its current stage, it can't do much.
It has a basic understanding of integers and arithmetic operations (+ - * /).
The following are some examples of operations you can perform in the REPL.

```plaintext
NicoLISP Version 0.0.1
Press Ctrl+c to Exit

nicoLSIP> + 2 2
4
nicoLISP> * (+ 2 2) (+ 4 4)
32
nicoLISP> / (* 10 10) (* 5 5)
4
```

One thing to notice if you're new to Lisp:
the 'operation' you want to perform comes __first__, followed by any number of operands (i.e. numbers). 
Where you may be used to the operator being in the middle (2 + 2), this prefix-based operator has some benefits.
You can provide as many operands to the operator as you want without needing to repeat the operator over and over.

```plaintext
Infix: 2 + 2 + 2 + 2 + 2
Prefix: + 2 2 2 2 2
```

As you can see, prefix notation makes this expression a bit cleaner and uses fewer characters.

Coming soon:

- Define your own functions
- 'If' statements for flow control
- Loops
- Float, String, and Boolean data types

