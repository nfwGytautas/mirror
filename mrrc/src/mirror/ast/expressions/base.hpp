#pragma once

namespace mirror {
	
	constexpr const char* c_InnerLoopValue = "_it";

	enum mrr_type {
		mrr_type_unspecified = 0,
		mrr_type_num = 1,
		mrr_type_str = 2,

		// Meta type, meaning this is a return expression so look at it's child for actual return type
		mrr_type_ret = 3,

		// Meta type, meaning this is a binary operation
		mrr_type_bop = 4,

        // Meta type, meaning this is a custom type either created or imported
        mrr_type_custom = -5
	};

    /**
     * Returns mrr_type from specified string
     * @param type Type to parse
     * @return mrr_type enum
     */
	mrr_type type_from_string(const std::string& type);

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

	llvm::Function* get_func(const std::string& name);

	llvm::Constant* const_str(const std::string& val);
}
