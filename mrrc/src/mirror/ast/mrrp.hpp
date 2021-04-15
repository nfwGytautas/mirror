#pragma once

#include "mirror/ast/mrrexpr.hpp"

namespace mirror {
	namespace parser {

		/**
		 * @brief Parses an expression
		*/
		std::unique_ptr<mrr_ast_expr> parse_primary();

		/**
		 * @brief Parse next token into a number
		*/
		std::unique_ptr<mrr_ast_expr> parse_number();

		/**
		 * @brief Parse next token into a string
		*/
		std::unique_ptr<mrr_ast_expr> parse_string();

		/**
		 * @brief Parse a parenthesis expression
		*/
		std::unique_ptr<mrr_ast_expr> parse_parenthesis();

		/**
		 * @brief Parse identifier expression
		*/
		std::unique_ptr<mrr_ast_expr> parse_identifier();

		/**
		 * @brief Parses a generic expression
		*/
		std::unique_ptr<mrr_ast_expr> parse_expression();

		/**
		 * @brief Parses a binary right hand side operation
		*/
		std::unique_ptr<mrr_ast_expr> parse_bin_op_rhs(int prec, std::unique_ptr<mrr_ast_expr> lhs);

		/**
		 * @brief Parse a function prototype
		*/
		std::unique_ptr<mrr_ast_prototype> parse_prototype();

		/**
		 * @brief Parse a function definition
		*/
		std::unique_ptr<mrr_ast_fn> parse_definition();

		/**
		 * @brief Parses a variable create
		*/
		std::unique_ptr<mrr_ast_expr> parse_var_create();

		/**
		 * @brief Parses a body
		*/
		std::unique_ptr<mrr_ast_body_expr> parse_body();

		/**
		 * @brief Parses a return statement
		*/
		std::unique_ptr<mrr_ast_expr> parse_return();

		/**
		 * @brief Parses a loop statement
		*/
		std::unique_ptr<mrr_ast_expr> parse_loop();
	}
}