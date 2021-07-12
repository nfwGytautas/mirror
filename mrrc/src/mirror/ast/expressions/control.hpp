#pragma once

namespace mirror {
	/**
	 * @brief Loop expression
	*/
	class mrr_ast_loop_expr : public mrr_ast_expr {
	public:
		mrr_ast_loop_expr(std::unique_ptr<mrr_ast_expr> expr, std::unique_ptr<mrr_ast_body_expr> body)
			: m_body(std::move(body)), m_expr(std::move(expr))
		{}

		virtual llvm::Value* codegen() override;
	private:
		std::unique_ptr<mrr_ast_expr> m_expr;
		std::unique_ptr<mrr_ast_body_expr> m_body;
	};

	/**
	 * @brief Match expression
	*/
	class mrr_ast_match_expr : public mrr_ast_expr {
	public:
		mrr_ast_match_expr(
			std::unique_ptr<mrr_ast_expr> val,
			std::vector<std::pair<std::unique_ptr<mrr_ast_expr>, std::unique_ptr<mrr_ast_body_expr>>> expressions, 
			std::unique_ptr<mrr_ast_body_expr> dExpr)
			: m_val(std::move(val)), m_expressions(std::move(expressions)), m_default(std::move(dExpr))
		{}

		virtual llvm::Value* codegen() override;

	private:
		std::unique_ptr<mrr_ast_expr> m_val;
		std::vector<std::pair<std::unique_ptr<mrr_ast_expr>, std::unique_ptr<mrr_ast_body_expr>>> m_expressions;
		std::unique_ptr<mrr_ast_body_expr> m_default;
	};
}
