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

// Creates entry function from module
llvm::Function *createEntryFunction(llvm::Module *module, llvm::LLVMContext &context);

// Invokes the JIT
void JIT(llvm::ExecutionEngine* engine, llvm::Function* function, int arg);


int main(int argc, char** argv) {
	if (argc != 2) {
		llvm::errs() << "Inform	an argument to yonur expression.\n";
		return 1;
	} else {
		llvm::InitializeNativeTarget();
		llvm::LLVMContext context;

		std::unique_ptr<llvm::Module> Owner = llvm::make_unique<llvm::Module>("DCC888 test", context);
		llvm::Module *M = Owner.get();

		llvm::Function *function = createEntryFunction(M, context);
		M->dump();
		llvm::ExecutionEngine *engine = llvm::EngineBuilder(std::move(Owner)).create();
                llvm::outs() << "Input expression: " << argv[1] << "\n";
		JIT(engine, function, atoi(argv[1]));
	}
}

llvm::Function *createEntryFunction(llvm::Module *module, llvm::LLVMContext &context) {
	llvm::Function * function = llvm::cast<llvm::Function>(
			module->getOrInsertFunction("fun", llvm::Type::getInt32Ty(context), //Which one is the return and the formal input?
					llvm::Type::getInt32Ty(context), (llvm::Type *) 0)); //Why return pointer to 0?
	llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "entry", function);
	llvm::IRBuilder<> builder(bb);
	// Tell the basic block builder to attach itself to the new basic block
	builder.SetInsertPoint(bb);

	llvm::Argument *argX = function->arg_begin();
	argX->setName("x");
	VarExpr::varValue = argX;
	Lexer lexer;
	Parser parser(&lexer);
	Expr* expr = parser.parseExpr();
	llvm::Value* retVal = expr->gen(&builder, context);
	builder.CreateRet(retVal);
	return function;
}

void JIT(llvm::ExecutionEngine* engine, llvm::Function* function, int arg)
{
	std::vector<llvm::GenericValue> Args(1);
	llvm::GenericValue(Args[0]).IntVal = llvm::APInt(32, arg);
	llvm::GenericValue retVal = engine->runFunction(function, Args);
	llvm::outs() << "Result:	" << retVal.IntVal << "\n";
}
