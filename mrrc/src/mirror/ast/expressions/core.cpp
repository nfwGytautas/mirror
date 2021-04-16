#include "mirror/ast/mrrexpr.hpp"
#include "mirror/utility/log.hpp"

namespace mirror {

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
		case '>': {
			// Compare and then convert
			auto val = compiler::get_current()->Builder->CreateFCmpUGT(l, r, "cmptmp");
			return compiler::get_current()->Builder->CreateUIToFP(val, llvm::Type::getDoubleTy(*compiler::get_current()->llvm), "booltmp");
		}
		default:
			log_error("Invalid binary operator %s", m_op);
			return nullptr;
		}
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
}
