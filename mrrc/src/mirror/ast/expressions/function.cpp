#include "mirror/ast/mrrexpr.hpp"
#include "mirror/utility/log.hpp"

namespace mirror {

	llvm::Function* mrr_ast_prototype::codegen(mrr_type rType) {
		// TODO: String argument support

		std::vector<llvm::Type*> args;
		args.reserve(m_args.size());

		// Parse args
		for (const std::string& type : m_types) {
		    switch(type_from_string(type)) {
		        case mrr_type_num:
		            args.push_back(llvm::Type::getDoubleTy(*compiler::get_current()->llvm));
		            break;
		        default:
		            log_error("Unknown type in function argument '%'", type.c_str());
		            return nullptr;
		    }
		}

		llvm::FunctionType* ft = nullptr;
		switch (rType) {
		case mrr_type::mrr_type_num:
			ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(*compiler::get_current()->llvm), args, false);
			break;
		case mrr_type::mrr_type_str:
			// TODO: String type return
			break;
		default:
			ft = llvm::FunctionType::get(llvm::Type::getVoidTy(*compiler::get_current()->llvm), args, false);
			break;
		}

		llvm::Function* f = llvm::Function::Create(ft, (llvm::GlobalValue::LinkageTypes)llvm::Function::ExternalLinkage, m_name, compiler::get_current()->Module);

		// Argument names
		unsigned int i = 0;
		for (auto& arg : f->args()) {
			arg.setName(m_args[i++]);
		}

		return f;
	}

	llvm::Function* mrr_ast_fn::codegen() {
		llvm::Function* fn = m_prototype->codegen(m_prototype->return_type());

		if (!fn) {
			return nullptr;
		}

		if (!fn->empty()) {
			log_error("Function cannot be redefined");
			return nullptr;
		}

		// Create basic block
		llvm::BasicBlock* bb = llvm::BasicBlock::Create(*compiler::get_current()->llvm, "func_bb", fn);
		compiler::get_current()->Builder->SetInsertPoint(bb);

		// Clear variable names
		compiler::get_current()->NamedValues.clear();

		for (auto& arg : fn->args()) {
			// Create alloca, store initial value and add arguments to symbol table
			llvm::AllocaInst* ai = compiler::createEntryBlockAlloca(fn, arg.getName().str());
			compiler::get_current()->Builder->CreateStore(&arg, ai);
			compiler::get_current()->NamedValues[arg.getName().str()] = ai;
		}

		// TODO: Should probably clean this up cause this will cause double return instruction
		llvm::Value* val = m_body->codegen();
		compiler::get_current()->Builder->CreateRet(val);
		llvm::verifyFunction(*fn);
		compiler::get_current()->FPM->run(*fn);

		return fn;
	}

	llvm::Value* mrr_fn_call_expr::codegen() {
		llvm::Function* callee = nullptr;
		std::vector<llvm::Value*> argsv;

		if (m_callee == "print") {
			callee = get_func("printf");

			if (!callee) {
				return nullptr;
			}

			std::string format = "";

			// Construct print format
			for (size_t i = 0; i < m_args.size(); i++) {
				llvm::Value* val = m_args[i]->codegen();

				if (!val) {
					return nullptr;
				}

				argsv.push_back(val);

				// Format
				if (val->getType()->isDoubleTy()) {
					format += "%f ";
				}
				// TODO: Might not work for string types
				else if (val->getType()->isIntOrPtrTy()) {
					format += "%s ";
				}
				else {
					log_error("Unknown variable type for print statement");
					return nullptr;
				}
			}

			format += "\n";

			argsv.emplace(argsv.begin(), const_str(format));
		}
		else {
			callee = get_func(m_callee);

			if (!callee) {
				return nullptr;
			}

			if (callee->arg_size() != m_args.size()) {
				log_error("Incorrect number of arguments passed got %ld expected %ld for function %s", m_args.size(), callee->arg_size(), m_callee.c_str());
				return nullptr;
			}

			for (size_t i = 0; i < m_args.size(); i++) {
				argsv.push_back(m_args[i]->codegen());

				// Check if the last inserted value was nullptr
				if (!argsv.back()) {
					return nullptr;
				}
			}
		}

		return compiler::get_current()->Builder->CreateCall(callee, argsv, "call_" + m_callee);
	}

	llvm::Value* mrr_ast_return_expr::codegen() {
		return m_expr->codegen();
	}
}
