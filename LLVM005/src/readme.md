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

