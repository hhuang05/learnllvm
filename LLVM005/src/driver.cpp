#include "lexer.h"
#include "parser.h"
#include "expr.h"
#include <memory>
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/PassManager.h"	
#include "llvm/IR/DataLayout.h"	
#include "llvm/Transforms/Scalar.h"

using namespace llvm;

// Creates entry function from module
Function *createEntryFunction(Module *module, LLVMContext &context);

void optimizeFunction(ExecutionEngine* engine, Module *module, Function* function);

// Invokes the JIT
void JIT(ExecutionEngine* engine, Function* function, int arg);


int main(int argc, char** argv) {
	if (argc != 2) {
		::errs() << "Inform	an argument to yonur expression.\n";
		return 1;
	} else {
		InitializeNativeTarget();
		LLVMContext context;

		std::unique_ptr<Module> Owner = make_unique<Module>("DCC888 test", context);
		Module *M = Owner.get();

		Function *function = createEntryFunction(M, context);
		M->dump();
		ExecutionEngine *engine = EngineBuilder(std::move(Owner)).create();
		JIT(engine, function, atoi(argv[1]));
	}
}

Function *createEntryFunction(Module *module, LLVMContext &context) {
	Function * function = cast<Function>(
			module->getOrInsertFunction("fun", Type::getInt32Ty(context), //Which one is the return and the formal input?
					Type::getInt32Ty(context), (Type *) 0)); //Why return pointer to 0?
	BasicBlock *bb = BasicBlock::Create(context, "entry", function);
	IRBuilder<> builder(bb);
	// Tell the basic block builder to attach itself to the new basic block
	builder.SetInsertPoint(bb);

	Argument *argX = function->arg_begin();
	argX->setName("x");
	VarExpr::varValue = argX;
	Lexer lexer;
	Parser parser(&lexer);
	Expr* expr = parser.parseExpr();
	Value* retVal = expr->gen(&builder, context);
	builder.CreateRet(retVal);
	return function;
}

void JIT(ExecutionEngine* engine, Function* function, int arg)
{
	std::vector<::GenericValue> Args(1);
	Args[0].IntVal = APInt(32, arg);
	GenericValue retVal = engine->runFunction(function, Args);
	outs() << "Result:	" << retVal.IntVal << "\n";
}

void optimizeFunction(ExecutionEngine* engine, Module *module, Function* function)
{
  FunctionPassManager passManager(module);	
  passManager.addPass(new DataLayout(*engine->getDataLayout()));
  passManager.addPass(createInstructionCombiningPass());
  passManager.addPass(createReassociatePass());
  passManager.addPass(createGVNPass());
  passManager.addPass(createCFGSimplificationPass());
  passManager.run(*function);	
}	
