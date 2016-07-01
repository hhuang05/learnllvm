set(LLVM_LINK_COMPONENTS
  Core
  ExecutionEngine
  Interpreter
  MC
  Support
  nativecodegen
  )

add_llvm_example(LambdaDriver
  driver.cpp
  expr.cpp
  lexer.cpp
  parser.cpp
  )
