// Creates entry function from module
llvm::Function *createEntryFunction(llvm::Module *module, llvm::LLVMContext &context);

// Creates execution engine
llvm::ExecutionEngine* createEngine(llvm::Module *module);

// Invokes the JIT
void JIT(llvm::ExecutionEngine* engine, llvm::Function* function, int arg);


int main(int argc, char** argv) {
	if (argc != 2) {
		llvm::errs() << "Inform	an argument to your expression.\n";
		return 1;
	} else {
		llvm::LLVMContext context;
		llvm::Module *module = new llvm::Module("Example", context);
		llvm::Function *function = createEntryFunction(module, context);
		module->dump();
		llvm::ExecutionEngine* engine = createEngine(module);
		JIT(engine, function, atoi(argv[1]));
	}
}

llvm::Function *createEntryFunction(llvm::Module *module, llvm::LLVMContext &context) {
	llvm::Function * function = llvm::cast<llvm::Function>(
			module->getOrInsertFunction("fun", llvm::Type::getInt32Ty(context), //Which one is the return and the formal input?
					llvm::Type::getInt32Ty(context), (llvm::Type *) 0)); //Why return pointer to 0?
	llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "entry", function);
	llvm::IRBuilder<> builder(context);
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

llvm::ExecutionEngine* createEngine(llvm::Module *module) {
	llvm::InitializeNativeTarget();
	std::string errStr;
	llvm::ExecutionEngine * engine =
	 llvm::EngineBuilder(module)
	 .setErrorStr(&errStr)
	 .setEngineKind(llvm::EngineKind::JIT)
	 .create();
	if (!engine) {
		llvm::errs() << "Failed to construct ExecutionEngine:" << errStr << "\n";
	} else if (llvm::verifyModule(*module)) {
		llvm::errs() << "Error constructing function!\n";
	}
	return engine;
}

void JIT(llvm::ExecutionEngine* engine, llvm::Function* function, int arg)
{
	std::vector<llvm::GenericValue> Args(1);
	llvm::GenericValue(Args[0]).IntVal = llvm::APInt(32, arg);
	llvm::GenericValue retVal = engine->runFunction(function, Args);
	llvm::outs() << "Result:	" << retVal.IntVal << "\n";
}
