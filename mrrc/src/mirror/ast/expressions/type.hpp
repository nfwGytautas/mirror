#pragma once

#include <vector>
#include "mirror/ast/expressions/base.hpp"

namespace mirror {
	
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
	 * @brief Variable creation expression
	*/
	class mrr_var_create_expr : public mrr_ast_expr {
	public:
		mrr_var_create_expr(std::string name, std::unique_ptr<mrr_ast_expr>& init)
			: m_name(name), m_init(std::move(init))
		{}

        mrr_var_create_expr(std::string name, std::unique_ptr<mrr_ast_expr> init)
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
     * @brief Typedef expression used to generate a new type
    */
    class mrr_typedef_expr {
    public:
        struct field {
            std::string type;
            std::string name;
        };

        mrr_typedef_expr(const std::string& name, std::vector<field> fields) : m_name(name), m_fields(std::move(fields)) {}

        mrr_type get_type() { return mrr_type_custom; }

        llvm::Type* codegen();
    private:
        std::string m_name;
        std::vector<field> m_fields;
    };

    /**
	 * @brief Field reference expression
	*/
    class mrr_field_expr : public mrr_ast_expr {
    public:
        mrr_field_expr(const std::string& object, const std::string& field)
                : m_object(object), m_field(field)
        {}

        virtual mrr_type get_type() override {
            return mrr_type_custom;
        }

        // Inherited via mrr_ast_expr
        virtual llvm::Value* codegen() override;
    private:
        std::string m_object;
        std::string m_field;
    };
}
