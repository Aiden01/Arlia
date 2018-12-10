# The Arlia programming language

![Arlia icon, designed by Mesabloo](http://image.noelshack.com/fichiers/2018/47/6/1543061036-arlia-gemstone-2.png)

### Introduction

Arlia is a pragmatic functional and semi-object oriented programming language for general use and will benefit, in the future, from the hyper-framework "Charm", itself written in Arlia.

### Goals

Arlia wants to be a pretty, pleasant and readable language offering many possibilities in the world of programming, adopting concepts and concepts interesting to use for any type of project.

### Features

- Algebraic types ;
- Higher order functions;
- Partial immutability ;
- Lambda expressions;
- Generic types ;
- Static types ;
- Type inference ;
- Type class ;
- Pattern matching;
- Recursion ;
- Exception handling ;
- String interpolation ;
- Interfaces
- Matrix operations.

### Project status

Arlia is in full development. There may be some significant changes, but the language and its objectives will be met during the alpha versions.
The current version is the version **0.01**. Indeed, the language and its implementation are still very young.

### The *Charm* hyper-framework

The hyper-framework *Charm* is a collection of mini-framework of all kinds for various uses, including elementary IO management tools, as well as GUI creation and simulation / motion / sound elements, and also projecting a framework for artificial intelligence, via deep learning. Web tools are also being considered, as well as database management tools.
Arlia, and her elementary functions will of course be able to use this past of *Charm*. But *Charm* is an essential tool for projects of all kinds.
Note: *Charm* will be implemented after the language creation, be patient! 

### Web site

[Visit the official Arlia fundation website.](https://vanaur.github.io/)

### About the author

I am a young student, very interested in the computer field, and especially in programming languages and artificial intelligence. I am alone on this project, but I sincerely hope that others will like it and that others will contribute to it.

### Functional and object Hello World

```hs
import charm.dio

type Greeter(private name: String) {
   func greet() {
      printfn("Welcome " ^ name ^ "!")
   }
}

let greeter = new Greeter("John")
greeter.greet()
```

### Functional fibonacci with exception

```hs
type Exception (msg: String)

type Result =
    | Success (Integer)
    | Failure (Exception)

let fibonacci n -> Result =
    match n with
    | 0 => Success 0
    | 1 => Success 1
    | _ => Success (fibonacci(n - 1) + fibonacci(n - 2))
    Failure (new Exception("Can't execute function"))

try fibonacci 0.3 with
    | Success value => printfn "Value: " ^ value.string()
    | Failure exception => printfn "Error: " ^ exception.msg
```

### Contribute!

The language is still very young, maybe you have found some flaws in the design? Feel free to **fork** the project, or to suggest improvements or submit comments via the [project issues](https://github.com/vanaur/Arlia/issues).

