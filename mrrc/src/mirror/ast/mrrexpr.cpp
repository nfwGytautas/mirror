#include "mirror/ast/mrrexpr.hpp"
#include "mirror/utility/log.hpp"

namespace mirror {

	llvm::Function* get_func(const std::string& name) {
		llvm::Function* fn = compiler::get_current()->Module->getFunction(name);

		if (!fn) {
			log_error("Unknown function referenced %s", name.c_str());
			return nullptr;
		}

		return fn;
	}

	llvm::Constant* const_str(const std::string& val) {
		return compiler::get_current()->Builder->CreateGlobalStringPtr(llvm::StringRef(val));
	}

	llvm::Value* mirror::mrr_num_expr::codegen() {
		return llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(m_val));
	}

	llvm::Value* mirror::mrr_str_expr::codegen() {
		return const_str(m_val);
	}

	llvm::Value* mrr_var_expr::codegen() {
		// Look up
		if (compiler::get_current()->NamedValues.find(m_name) == compiler::get_current()->NamedValues.end()) {
			log_error("Unknown variable name referenced %s", m_name.c_str());
			return nullptr;
		}

		return compiler::get_current()->Builder->CreateLoad(compiler::get_current()->NamedValues[m_name]);
	}

	llvm::Value* mrr_bin_expr::codegen() {
		// Assignment
		if (m_op == '=') {
			mrr_var_expr* lhse = dynamic_cast<mrr_var_expr*>(m_lhs.get());
			if (!lhse) {
				log_error("Destination of '=' must be a variable");
				return nullptr;
			}

			llvm::Value* val = m_rhs->codegen();
			if (!val) {
				log_error("Assignment right hand side cannot be parsed");
				return nullptr;
			}

			// Look up name
			llvm::Value* var = compiler::get_current()->NamedValues[lhse->get_name()];
			if (!var) {
				log_error("Unknwon variable name %s", lhse->get_name().c_str());
				return nullptr;
			}

			compiler::get_current()->Builder->CreateStore(val, var);
			return val;
		}


		llvm::Value* l = m_lhs->codegen();
		llvm::Value* r = m_rhs->codegen();

		if (!l || !r) {
			return nullptr;
		}

		switch (m_op) {
		case '+':
			return compiler::get_current()->Builder->CreateFAdd(l, r, "addtmp");
		case '-':
			return compiler::get_current()->Builder->CreateFSub(l, r, "subtmp");
		case '*':
			return compiler::get_current()->Builder->CreateFMul(l, r, "multmp");
		case '<': {
			// Compare and then convert
			auto val = compiler::get_current()->Builder->CreateFCmpULT(l, r, "cmptmp");
			return compiler::get_current()->Builder->CreateUIToFP(val, llvm::Type::getDoubleTy(*compiler::get_current()->llvm), "booltmp");
		}
		default:
			log_error("Invalid binary operator %s", m_op);
			return nullptr;
		}
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
				log_error("Incorrect number of arguments passed got %ld expected %ld for function %s", m_args.size(), callee->arg_size(), m_callee);
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

		return compiler::get_current()->Builder->CreateCall(callee, argsv, "call_"+m_callee);
	}

	llvm::Value* mrr_var_create_expr::codegen() {
		llvm::Function* fn = compiler::get_current()->Builder->GetInsertBlock()->getParent();
		llvm::Value* initVal = nullptr;

		if (!m_init) {
			initVal = llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(0.0));
		}
		else {
			initVal = m_init->codegen();
		}

		if (initVal == nullptr) {
			return nullptr;
		}

		// Alloca instruction
		llvm::AllocaInst* aa = compiler::createEntryBlockAlloca(fn, m_name);
		compiler::get_current()->Builder->CreateStore(initVal, aa);

		// Add to symbol table
		compiler::get_current()->NamedValues[this->m_name] = aa;

		return initVal;
	}

	llvm::Value* mrr_ast_body_expr::codegen() {
		for (std::unique_ptr<mrr_ast_expr>& expr : m_expressions) {
			llvm::Value* val = expr->codegen();

			if (expr->get_type() == mrr_type::mrr_type_ret) {
				compiler::get_current()->Builder->CreateRet(val);
			}
		}

		return nullptr;
	}

	llvm::Function* mrr_ast_prototype::codegen(mrr_type rType) {
		// TODO: String argument support
		// TODO: Argument types

		std::vector<llvm::Type*> args(m_args.size(), llvm::Type::getDoubleTy(*compiler::get_current()->llvm));

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
		llvm::Function* fn = m_prototype->codegen(m_body->returnType());

		if (!fn) {
			return nullptr;
		}

		if (!fn->empty()) {
			log_error("Function cannot be redefined.");
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

	llvm::Value* mrr_ast_return_expr::codegen() {
		return m_expr->codegen();
	}

}
