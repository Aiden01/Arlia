# Project Source
##### Changes are often made, but the project is moving forward.

# Project Concept
The compiler is built so that it is as fast and efficient as possible.
The compiler processes everything in one pass, and without storing chips in a container. He has a method that allows him to retrieve the chips one by one and on each call. In this way, each token is analyzed as soon as it leaves the lexer. If there are no errors, the token is analyzed and added to the AST. Once the AST is built, it is optimized if it has no semantic errors, then is directly translated into assembler code. There's no bytecode. Here is a diagram of the global operation:

![](https://raw.githubusercontent.com/vanaur/Arlia/master/examples/images/df.PNG)
