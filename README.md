
## Rotate (C++)
> **Warning** Work is still in progress do not use for any serious project 
- inspired by C, odin, Jai and zig
```odin
io :: import("std/io");

main :: fn() {
	io.print("Hello, World\n");
}
```


## Completed Progress
- [x] Lexer
- [] Parser
- [] TypeChecker 
- [] Analyzer 
- [] Maybe? Optimizer
- [] CodeGenerator (maybe custom Arch) 

## C Style 
- Most of the code is pure C-Style except with some syntax sugar from C++ to make code easier to develop
- The only util i use from C++ stdlib is the std::vector (for now)


## Goals
- TODO: make the compiler compile on C++11 or maybe older versions
- TODO: Reduce as much macros as possible
- TODO: Make the language usable for personal projects 
- TODO: Remove the C++ std library dependency 
- Rely on C std library only
