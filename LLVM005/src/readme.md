# Language

Lambda expressions

1. A program is a function application
2. A function contains only one argument: x
3. Only the integer type exists
4. The function body contains only additions, multiplications,
references to x, and integer constants in polish notation:

    (lambda x. + x x) 4 = 16

    (lambda x. + x * x 2) 4 = 12

    (lambda x. * x + x 2) 4 = 24

# Building

1. Currently this is built within an LLVM source tree. Change directory into `llvm/examples`
2. Run the following commands `ln -s <source location> DCC888`, this will create a link folder in the `examples` directory.
3. Now change directory to the build folder for llvm, on `blackhole`, this is located at `/data/hhuang04/llvm3.7_build`. Now run `cmake ../llvm_src` which will recreate the build files. The current binaries will not be wiped!
4. Run `make LambdaDriver -j16` to build the binary with 16 cores. 
