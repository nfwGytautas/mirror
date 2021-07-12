#pragma once

namespace mirror {

	/**
	 * @brief Function prototype
	*/
	class mrr_ast_prototype {
	public:
		mrr_ast_prototype(const std::string& name, std::vector<std::string> args, std::vector<std::string> types, const std::string& rType)
			: m_name(name), m_args(std::move(args)), m_types(std::move(types)), m_retType(type_from_string(rType)) {}

		llvm::Function* codegen(mrr_type rType);

		mrr_type return_type() const {
		    return m_retType;
		}
	private:
		std::string m_name;
		mrr_type m_retType;
		std::vector<std::string> m_args;
        std::vector<std::string> m_types;
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
		mrr_ast_return_expr(std::unique_ptr<mrr_ast_expr> expr)
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
