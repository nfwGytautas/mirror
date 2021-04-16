#pragma once

namespace mirror {

	/**
	 * @brief Function prototype
	*/
	class mrr_ast_prototype {
	public:
		mrr_ast_prototype(const std::string& name, std::vector<std::string> args)
			: m_name(name), m_args(std::move(args)) {}

		llvm::Function* codegen(mrr_type rType);
	private:
		std::string m_name;
		std::vector<std::string> m_args;
	};

	/**
	 * @brief Function definition
	*/
	class mrr_ast_fn {
	public:
		mrr_ast_fn(std::unique_ptr<mrr_ast_prototype> prototype, std::unique_ptr<mrr_ast_body_expr> body)
			: m_prototype(std::move(prototype)), m_body(std::move(body)) {}

		llvm::Function* codegen();
	private:
		std::unique_ptr<mrr_ast_prototype> m_prototype;
		std::unique_ptr<mrr_ast_body_expr> m_body;
	};

	/**
	 * @brief Function call expression
	*/
	class mrr_fn_call_expr : public mrr_ast_expr {
	public:
		mrr_fn_call_expr(const std::string& callee, std::vector<std::unique_ptr<mrr_ast_expr>> args)
			: m_callee(callee), m_args(std::move(args)) {}

		// Inherited via mrr_ast_expr
		virtual llvm::Value* codegen() override;
	private:
		std::string m_callee;
		std::vector<std::unique_ptr<mrr_ast_expr>> m_args;
	};

	/**
	 * @brief Return expression
	*/
	class mrr_ast_return_expr : public mrr_ast_expr {
	public:
		mrr_ast_return_expr(std::unique_ptr<mrr_ast_expr>& expr)
			: m_expr(std::move(expr))
		{}

		virtual mrr_type get_type() override {
			return mrr_type::mrr_type_ret;
		}

		mrr_type get_actual_type() {
			return m_expr->get_type();
		}

		llvm::Value* codegen() override;

	private:
		std::unique_ptr<mrr_ast_expr> m_expr;
	};

}
