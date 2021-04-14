#include "mirror/compiler/mrrc.hpp"
#include "mirror/ast/mrrp.hpp"
#include "mirror/utility/log.hpp"

#include <iostream>

namespace mirror {
	namespace compiler {
		mrrc* g_ci = nullptr;

		bool handle_definition() {
			if (auto fnAST = parser::parse_definition()) {
				if (auto* fnIR = fnAST->codegen()) {
					fnIR->print(llvm::errs());
					fprintf(stderr, "\n");
				}

				// TODO: Add if failed to codegen
				return true;
			}
			else {
				// Error recovery
				return false;
			}
		}

		void add_std(mrrc* c) {
			llvm::FunctionCallee print = c->Module->getOrInsertFunction(
				"printf",
				llvm::FunctionType::get(
					llvm::IntegerType::getInt32Ty(*c->llvm),
					llvm::PointerType::get(llvm::Type::getInt8Ty(*c->llvm), 0), true));
		}

		void entry_point(mrrc* c) {
			// No args
			std::vector<llvm::Type*> args;
			std::vector<llvm::Value*> argsv;
			llvm::FunctionType* ft = llvm::FunctionType::get(llvm::Type::getVoidTy(*c->llvm), args, false);
			llvm::Function* entry = llvm::Function::Create(ft, (llvm::GlobalValue::LinkageTypes)llvm::Function::ExternalLinkage, "_start", c->Module);

			auto bb = llvm::BasicBlock::Create(*c->llvm, "_start", entry);
			c->Builder->SetInsertPoint(bb);

			llvm::Function* mainFunc = c->Module->getFunction("main");

			if (!mainFunc) {
				log_error("No main function specified");
				return;
			}

			c->Builder->CreateCall(mainFunc, argsv, "");
			c->Builder->CreateRet(nullptr);

			llvm::verifyFunction(*entry);
			entry->print(llvm::errs());
			fprintf(stderr, "\n");
		}

		bool compile(mrrc* c, lexer::mrrl* l) {
			// Prepare by setting globals
			compiler::set_current(c);
			lexer::set_current(l);
			lexer::next_token(l);
			while (1) {
				switch (l->Curtok) {
				case mrrt_eof:
					return true;
				case mrrt_fn:
					if (!handle_definition()) {
						return false;
					}
					break;
				}
			}
		}

		void mrrc_init() {
			// Emitters
			//llvm::InitializeAllTargetInfos();
			//llvm::InitializeAllTargets();
			//llvm::InitializeAllTargetMCs();
			//llvm::InitializeAllAsmParsers();
			//llvm::InitializeAllAsmPrinters();
			llvm::InitializeNativeTarget();
			llvm::InitializeNativeTargetAsmParser();
			llvm::InitializeNativeTargetAsmPrinter();
		}

		mrrc* mrrc_new() {
			mrrc* result = new mrrc();
			result->llvm = new llvm::LLVMContext();
			result->Builder = new llvm::IRBuilder<>(*result->llvm);
			result->Module = new llvm::Module("mirror", *result->llvm);
			result->FPM = new llvm::legacy::FunctionPassManager(result->Module);

			// Optimizations
			//result->FPM.add(llvm::createInstructionCombiningPass());
			//result->FPM->add(llvm::createPromoteMemoryToRegisterPass());
			//result->FPM.add(llvm::createReassociatePass());
			//result->FPM.add(llvm::createGVNPass());
			//result->FPM.add(llvm::createCFGSimplificationPass());

			result->FPM->doInitialization();

			add_std(result);

			return result;
		}

		void free_mrrc(mrrc* c) {
			delete c->llvm;
			delete c->Module;
			delete c->Builder;
			delete c->FPM;

			delete c;
		}

		void emit_to_file(mrrc* c, const std::string& file) {
			// Invoke entry
			entry_point(c);

			std::string CPU = "generic";
			std::string features = "";
			std::string error = "";

			auto targetTriple = llvm::sys::getDefaultTargetTriple();
			auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

			if (!target) {
				llvm::errs() << error;
				return;
			}

			llvm::TargetOptions opt;
			auto rm = llvm::Optional<llvm::Reloc::Model>();
			auto targetMachine = target->createTargetMachine(targetTriple, CPU, features, opt, rm);

			c->Module->setDataLayout(targetMachine->createDataLayout());
			c->Module->setTargetTriple(targetTriple);

			std::error_code EC;
			llvm::raw_fd_ostream dest(file, EC, llvm::sys::fs::OF_None);

			if (EC) {
				log_error("Could not open file: %s", EC.message().c_str())
				return;
			}

			llvm::legacy::PassManager pass;
			auto FileType = llvm::CGFT_ObjectFile;

			if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
				log_error("TargetMachine can't emit a file of this type");
				return;
			}

			pass.run(*c->Module);
			dest.flush();
		}

		mrrc* get_current() {
			return g_ci;
		}

		void set_current(mrrc* c) {
			g_ci = c;
		}

		llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* fn, const std::string& var) {
			// TODO: Types
			llvm::IRBuilder<> tmpb(&fn->getEntryBlock(), fn->getEntryBlock().begin());
			return tmpb.CreateAlloca(llvm::Type::getDoubleTy(*g_ci->llvm), 0, var.c_str());
		}
	}
}
