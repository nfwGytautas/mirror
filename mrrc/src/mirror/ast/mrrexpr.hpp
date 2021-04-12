#pragma once

#include <string>
#include <memory>
#include <vector>

#include "mirror/compiler/mrrc.hpp"

namespace mirror {
	
	enum mrr_type {
		mrr_type_unspecified = 0,
		mrr_type_num = 1,
		mrr_type_str = 2
	};

	/**
	 * @brief Base class for all mirror expressions
	*/
	class mrr_ast_expr {
	public:
		virtual ~mrr_ast_expr() {};

		virtual mrr_type get_type() { return mrr_type_unspecified; };

		/**
		 * @brief Generate LLVM code for the expression
		*/
		virtual llvm::Value* codegen() = 0;
	};

	/**
	 * @brief Number expression
	*/
	class mrr_num_expr : public mrr_ast_expr {
	public:
		mrr_num_expr(double val) : m_val(val) {}

		virtual mrr_type get_type() override { return mrr_type_num; }

		// Inherited via mrr_ast_expr
		virtual llvm::Value* codegen() override;
	private:
		double m_val;
	};

	/**
	 * @brief String expression
	*/
	class mrr_str_expr : public mrr_ast_expr {
	public:
		mrr_str_expr(const std::string& val) : m_val(val) {}

		virtual mrr_type get_type() override { return mrr_type_str; }

		// Inherited via mrr_ast_expr
		virtual llvm::Value* codegen() override;
	private:
		std::string m_val;
	};

	/**
	 * @brief Variable reference expression
	*/
	class mrr_var_expr : public mrr_ast_expr {
	public:
		mrr_var_expr(const std::string& name) 
			: m_name(name)
		{}

		std::string get_name() const {
			return m_name;
		}

		virtual mrr_type get_type() override { 
			return m_type;
		}

		// Inherited via mrr_ast_expr
		virtual llvm::Value* codegen() override;
	private:
		std::string m_name;
		mrr_type m_type;
	};

	/**
	 * @brief Binary operator expression
	*/
	class mrr_bin_expr : public mrr_ast_expr {
	public:
		mrr_bin_expr(char op, std::unique_ptr<mrr_ast_expr> lhs, std::unique_ptr<mrr_ast_expr> rhs)
			: m_op(op), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

		// Inherited via mrr_ast_expr
		virtual llvm::Value* codegen() override;
	private:
		char m_op;
		std::unique_ptr<mrr_ast_expr> m_lhs;
		std::unique_ptr<mrr_ast_expr> m_rhs;
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
	 * @brief Variable creation expression
	*/
	class mrr_var_create_expr : public mrr_ast_expr {
	public:
		mrr_var_create_expr(std::string name, std::unique_ptr<mrr_ast_expr>& init)
			: m_name(name), m_init(std::move(init))
		{}

		virtual mrr_type get_type() override {
			return m_init->get_type();
		}

		virtual llvm::Value* codegen() override;
	private:
		std::string m_name;
		std::unique_ptr<mrr_ast_expr> m_init;
	};

	/**
	 * @brief Function prototype
	*/
	class mrr_ast_prototype {
	public:
		mrr_ast_prototype(const std::string& name, std::vector<std::string> args)
			: m_name(name), m_args(std::move(args)) {}

		llvm::Function* codegen();
	private:
		std::string m_name;
		std::vector<std::string> m_args;
	};

	/**
	 * @brief Function definition
	*/
	class mrr_ast_fn {
	public:
		mrr_ast_fn(std::unique_ptr<mrr_ast_prototype> prototype, std::vector<std::unique_ptr<mrr_ast_expr>> body)
			: m_prototype(std::move(prototype)), m_body(std::move(body)) {}

		llvm::Function* codegen();
	private:
		std::unique_ptr<mrr_ast_prototype> m_prototype;
		std::vector<std::unique_ptr<mrr_ast_expr>> m_body;
	};
}
