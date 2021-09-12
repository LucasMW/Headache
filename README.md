# Headache
> Headache is a programming language that compiles to 8-Bit Brainfuck.

## Documentation

For documentation, please visit `Headache`'s wiki: https://github.com/LucasMW/Headache/wiki

## Support Headache

Help Headache launch on Homebrew. This repository needs __50 stargazers__ to become a `brew` package.

## Build

You may build the project by running `make`.

## Install

Run `make install` and the include `$HOME/.Headache/` in your `PATH` variable in order to enable `hac` and `bfi` in your shell.

## Usage

##### Compiling hac
In order to compile `headache` (.ha) files to `brainfuck`, run:

```
./hac file.ha
```

It generates and executes a brainfuck file named `a.bf`.

##### Options

###### Lexical analyzer
To run a lexical analyzer, execute:
```
./hac -lex
```

##### Syntactical Analyzer
To run a syntactical analyzer, execute:
```
./hac -syntax
```

##### AST
To output the current program's AST, execute:
```
./hac -tree
```

##### Warning and errors checking
To check for errors and warnings without compiling, execute:
```
./hac -check
```

## Tests

Execute `make test` to run the default tests.

##### Other test options

For checking the file only, run:
```
make testchecks
```

For testing only the lexical scope, run:
```
make testlexical
```

For testing only the syntactical scope, run:
```
make testsyntax
```

For testing only programs that are compiled and run successfully, run:
```
make testrunnable
```

### Donation Addresses:

Support Headache with cryptocurrency:

  - `BTC`: 1NcdcAjkKj9ywiCfinwFi1W3QfsvFSFsAH
  - `Doge`: D5wEBGcwLeBSQvwnBckco1EvQQcuXCBvRj
  - `LTC`: Ldj6ax1CzdYFRbSiZjpCvcPoxQ2BvX42Kb
  - `ZEC`: t1PsMPGuoZfD5YwAgmyd3j2cb8siysxR9fE
  - `Dash`: Xt7qYtLiLx6mHR1fUeQVYiHgEbX7YNugM2
  - `BCH`: 1NCYSnFb44ybHtzwG51cUYWLa67PZdsREG
  - `XMR`: 4CTKuktQ8eQFriMu3xDALEawuu9iWQhFYffgffqgC3RHCWgcLGTG9oyNWgvLiAoDPpYUvsJmRkTAudGSXNqnRFsuT9LNU3Up81F2x3fzkj
