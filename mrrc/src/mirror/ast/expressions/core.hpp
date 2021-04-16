#pragma once

namespace mirror {
	/**
	 * @brief Binary operator expression
	*/
	class mrr_bin_expr : public mrr_ast_expr {
	public:
		mrr_bin_expr(char op, std::unique_ptr<mrr_ast_expr> lhs, std::unique_ptr<mrr_ast_expr> rhs)
			: m_op(op), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

		virtual mrr_type get_type() override {
			return mrr_type::mrr_type_bop;
		}

		// Inherited via mrr_ast_expr
		virtual llvm::Value* codegen() override;
	private:
		char m_op;
		std::unique_ptr<mrr_ast_expr> m_lhs;
		std::unique_ptr<mrr_ast_expr> m_rhs;
	};

	/**
	 * @brief Body expression
	*/
	class mrr_ast_body_expr : public mrr_ast_expr {
	public:
		mrr_ast_body_expr(mrr_type rType, std::vector<std::unique_ptr<mrr_ast_expr>> expressions) 
			: m_retType(rType), m_expressions(std::move(expressions))
		{}

		/**
		 * @brief Nullptr if there was no return statement in the code
		*/
		virtual llvm::Value* codegen() override;

		mrr_type returnType() const {
			return m_retType;
		}
	private:
		std::vector<std::unique_ptr<mrr_ast_expr>> m_expressions;
		mrr_type m_retType;
	};
}
