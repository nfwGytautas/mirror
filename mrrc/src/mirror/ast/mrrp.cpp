#include "mrrp.hpp"
#include "mirror/lexer/mrrl.hpp"
#include "mirror/compiler/mrrc.hpp"
#include "mirror/utility/log.hpp"

namespace mirror {
	namespace parser {

		const char* rType_to_str(mrr_type rType) {
			switch (rType) {
			case mrr_type::mrr_type_num:
				return "Number";
			case mrr_type::mrr_type_str:
				return "String";
			default:
				return "Unknown";
			}
		}

		int get_precedence(char binop) {
			// Binary operator precedence
			switch (binop) {
			case '=':
				return 2;

			case '<':
			case '>':
				return 10;

			case '+':
			case '-':
				return 20;

			case '*':
				return 40;

			default:
				return -1;
			}
		}

		int get_tok_precedence() {
			int ctok = lexer::get_current()->Curtok;

			if (!isascii(ctok)) {
				return -1;
			}

			return get_precedence(ctok);
		}

		std::unique_ptr<mrr_ast_expr> parse_primary() {
			switch (lexer::get_current()->Curtok) {
			case mrrt_identifier:
				return parse_identifier();
			case mrrt_number:
				return parse_number();
			case mrrt_str:
				return parse_string();
			case mrrt_var:
				return parse_var_create();
			case mrrt_ret:
				return parse_return();
			case mrrt_loop:
				return parse_loop();
			case mrrt_match:
				return parse_match();
			case '{':
				return parse_body();
			case '(':
				return parse_parenthesis();
			default:
				log_error("Unknown token when expecting an expression");
				return nullptr;
			}
		}

		std::unique_ptr<mrr_ast_expr> parse_number() {
			// expr ::= number

			auto res = std::make_unique<mrr_num_expr>(lexer::get_current()->Num);
			lexer::next_token(lexer::get_current()); // Consume number
			return std::move(res);
		}

		std::unique_ptr<mrr_ast_expr> parse_string() {
			// expr ::= string

			auto res = std::make_unique<mrr_str_expr>(lexer::get_current()->IdentifierStr);
			lexer::next_token(lexer::get_current());
			return std::move(res);
		}

		std::unique_ptr<mrr_ast_expr> parse_parenthesis() {
			// expr ::= '(' expression ')'

			lexer::next_token(lexer::get_current()); // Consume '('

			auto v = parse_expression();

			if (!v) {
				return nullptr;
			}

			if (lexer::get_current()->Curtok != ')') {
				log_error("expected ')'");
				return nullptr;
			}

			lexer::next_token(lexer::get_current());
			return v;
		}

		std::unique_ptr<mrr_ast_expr> parse_identifier() {
			// expr ::= 
			//		identifier
			//		identifier '(' expression ')'

			std::string id_name = lexer::get_current()->IdentifierStr;

			lexer::next_token(lexer::get_current()); // Consume identifier

			if (lexer::get_current()->Curtok != '(') {
				return std::make_unique<mrr_var_expr>(id_name);
			}

			// Function call
			lexer::next_token(lexer::get_current()); // Consume '('
			std::vector<std::unique_ptr<mrr_ast_expr>> args;

			if (lexer::get_current()->Curtok != ')') {
				while (true) {
					if (auto arg = parse_expression()) {
						args.push_back(std::move(arg));
					}
					else {
						return nullptr;
					}

					// End of expression
					if (lexer::get_current()->Curtok == ')') {
						break;
					}

					if (lexer::get_current()->Curtok != ',') {
						log_error("Expected ')' or ',' in argument list");
						return nullptr;
					}

					lexer::next_token(lexer::get_current());
				}
			}

			lexer::next_token(lexer::get_current()); // Consume ')'

			return std::make_unique<mrr_fn_call_expr>(id_name, std::move(args));
		}

		std::unique_ptr<mrr_ast_expr> parse_expression() {
			// expr ::= primary binoprhs '}?'

			auto lhs = parse_primary();
			if (!lhs) {
				return nullptr;
			}

			return parse_bin_op_rhs(0, std::move(lhs));
		}

		std::unique_ptr<mrr_ast_expr> parse_bin_op_rhs(int prec, std::unique_ptr<mrr_ast_expr> lhs) {
			// Find precedence
			while (true) {
				if (lexer::get_current()->Curtok == '}') {
					return lhs;
				}

				int tok_prec = get_tok_precedence();

				if (tok_prec < prec) {
					return lhs;
				}

				int op = lexer::get_current()->Curtok;
				lexer::next_token(lexer::get_current()); // Consume binary operation

				// Parse expression
				auto rhs = parse_primary();
				if (!rhs) {
					return nullptr;
				}

				// If the next token has higher precedence then the next expression takes this rhs as its lhs
				int next_prec = get_tok_precedence();
				if (tok_prec < next_prec) {
					// Parse the entire right hand side
					rhs = parse_bin_op_rhs(tok_prec + 1, std::move(rhs));
					if (!rhs) {
						return nullptr;
					}
				}

				// Merge into lhs
				lhs = std::make_unique<mrr_bin_expr>(op, std::move(lhs), std::move(rhs));
			}
		}

		std::unique_ptr<mrr_ast_prototype> parse_prototype() {
			// prot ::= name '(' (id : type)*, ')' ('->' type)?

			if (lexer::get_current()->Curtok != mrrt_identifier) {
				log_error("Expected function name in prototype");
				return nullptr;
			}

			std::string fnName = lexer::get_current()->IdentifierStr;
			lexer::next_token(lexer::get_current()); // Consume name

			if (lexer::get_current()->Curtok != '(') {
				log_error("Expected '(' in prototype");
				return nullptr;
			}

			// Arguments
			std::vector<std::string> argNames;
			std::vector<std::string> argTypes;

			while (true) {
				if (lexer::next_token(lexer::get_current()) == ')') {
					lexer::next_token(lexer::get_current()); // Consume ')'
					break;
				}

				if (lexer::get_current()->Curtok == ',') {
					lexer::next_token(lexer::get_current()); // Consume ','
				}

				if (lexer::get_current()->Curtok == mrrt_identifier) {
					argNames.push_back(lexer::get_current()->IdentifierStr);
                    lexer::next_token(lexer::get_current()); // Consume 'id'
                    lexer::next_token(lexer::get_current()); // Consume ':'
                    argTypes.push_back(lexer::get_current()->IdentifierStr);
				}
				else {
					log_error("Incorrect function definition");
					return nullptr;
				}
			}

			// Return
			std::string rType = "";
			if (lexer::get_current()->Curtok == mrrt_rarrow) {
                lexer::next_token(lexer::get_current()); // Consume '->'
                rType = lexer::get_current()->IdentifierStr;
                lexer::next_token(lexer::get_current()); // Consume type
			}

			return std::make_unique<mrr_ast_prototype>(fnName, std::move(argNames), std::move(argTypes), rType);
		}

		std::unique_ptr<mrr_ast_fn> parse_definition() {
			// def ::= 'fn' prototype body

			std::unique_ptr<mrr_ast_fn> ret;
			std::unique_ptr<mrr_ast_body_expr> body;

			lexer::next_token(lexer::get_current()); // Consume fn

			auto proto = parse_prototype();
			if (!proto) {
				return nullptr;
			}

			body = std::move(parse_body());

			ret = std::make_unique<mrr_ast_fn>(std::move(proto), std::move(body));
			return ret;
		}

		std::unique_ptr<mrr_ast_expr> parse_var_create() {
			// expr ::= 'var' identifier '=' expression

			lexer::next_token(lexer::get_current()); // Consume 'var'

			if (lexer::get_current()->Curtok != mrrt_identifier) {
				log_error("Expected indentifier after var");
				return nullptr;
			}

			std::string name = lexer::get_current()->IdentifierStr;
			lexer::next_token(lexer::get_current()); // Consume 'identifier'

			if (lexer::get_current()->Curtok != '=') {
				log_error("Expected = after var identifier");
				return nullptr;
			}

			lexer::next_token(lexer::get_current()); // Consume '='

			std::unique_ptr<mrr_ast_expr> init = parse_expression();

			if (!init) {
				log_error("Failed to parse variable assignment expression");
				return nullptr;
			}

			return std::make_unique<mrr_var_create_expr>(name, std::move(init));
		}

		std::unique_ptr<mrr_ast_body_expr> parse_body() {
			// epxr ::= '{' *expressions '}'
			std::vector<std::unique_ptr<mrr_ast_expr>> body;

			if (lexer::get_current()->Curtok != '{') {
				log_error("Expected '{' in function expression");
				return nullptr;
			}
			lexer::next_token(lexer::get_current()); // Consume '{'

			while (lexer::get_current()->Curtok != '}') {
				if (auto e = parse_expression()) {
					body.push_back(std::move(e));
				}
			}
			lexer::next_token(lexer::get_current()); // Consume '}'

			return std::make_unique<mrr_ast_body_expr>(std::move(body));
		}

		std::unique_ptr<mrr_ast_expr> parse_return() {
			// expr ::= return expr
			lexer::next_token(lexer::get_current()); // Consume 'return'
			return std::make_unique<mrr_ast_return_expr>(std::move(parse_expression()));
		}
		
		std::unique_ptr<mrr_ast_expr> parse_loop() {
			// expr ::= 'loop' '(' expression ')' body
			
			// TODO: Compiler errors
			lexer::next_token(lexer::get_current()); // Consume 'loop'
			lexer::next_token(lexer::get_current()); // Consume '('

			std::unique_ptr<mrr_ast_expr> expr = parse_expression();
			if (!expr) {
				return nullptr;
			}

			lexer::next_token(lexer::get_current()); // Consume ')'

			std::unique_ptr<mrr_ast_body_expr> body = parse_body();
			
			return std::make_unique<mrr_ast_loop_expr>(std::move(expr), std::move(body));
		}
		
		std::unique_ptr<mrr_ast_expr> parse_match(){
			// expr ::= 'match' '(' var ')' '{' '(' expr ')' 'to' body '}'

			lexer::next_token(lexer::get_current()); // Consume 'match'
			lexer::next_token(lexer::get_current()); // Consume '('

			std::unique_ptr<mrr_ast_expr> val = parse_expression();

			if (!val) {
				log_error("Match on invalid value");
				return nullptr;
			}

			lexer::next_token(lexer::get_current()); // Consume ')'
			lexer::next_token(lexer::get_current()); // Consume '{'

			std::vector<std::pair<std::unique_ptr<mrr_ast_expr>, std::unique_ptr<mrr_ast_body_expr>>> expressions;
			std::unique_ptr<mrr_ast_body_expr> dExpr = nullptr;
			while (lexer::get_current()->Curtok != '}') {
				lexer::next_token(lexer::get_current()); // Consume '('

				// Default
				if (lexer::get_current()->IdentifierStr == "_") {
					// Check if not more than 1 default
					if (dExpr) {
						log_error("Match expression can only have 1 default case");
						return nullptr;
					}

					lexer::next_token(lexer::get_current()); // Consume '_'
					lexer::next_token(lexer::get_current()); // Consume ')'
					lexer::next_token(lexer::get_current()); // Consume 'to'

					dExpr = parse_body();
					if (!dExpr) {
						log_error("Match expression requires body");
						return nullptr;
					}
				}
				else {
					std::unique_ptr<mrr_ast_expr> mExpr = parse_expression();
					if (!mExpr) {
						log_error("Invalid match expression");
						return nullptr;
					}

					lexer::next_token(lexer::get_current()); // Consume ')'
					lexer::next_token(lexer::get_current()); // Consume 'to'

					std::unique_ptr<mrr_ast_body_expr> body = parse_body();
					if (!body) {
						log_error("Match expression requires body");
						return nullptr;
					}

					expressions.push_back(
						std::make_pair<std::unique_ptr<mrr_ast_expr>, std::unique_ptr<mrr_ast_body_expr>>(
							std::move(mExpr), 
							std::move(body)
							)
					);
				}
			}

			lexer::next_token(lexer::get_current()); // Consume '}'
			
			return std::make_unique<mrr_ast_match_expr>(std::move(val), std::move(expressions), std::move(dExpr));
		}
	}
}