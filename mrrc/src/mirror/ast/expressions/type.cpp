#include "mirror/ast/mrrexpr.hpp"
#include "mirror/utility/log.hpp"

namespace mirror {

	llvm::Value* mrr_var_expr::codegen() {
		// Look up
		if (compiler::get_current()->NamedValues.find(m_name) == compiler::get_current()->NamedValues.end()) {
			log_error("Unknown variable name referenced %s", m_name.c_str());
			return nullptr;
		}

		return compiler::get_current()->Builder->CreateLoad(compiler::get_current()->NamedValues[m_name]);
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

	llvm::Value* mirror::mrr_num_expr::codegen() {
		return llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(m_val));
	}

	llvm::Value* mirror::mrr_str_expr::codegen() {
		return const_str(m_val);
	}

}
