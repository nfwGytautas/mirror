#include "mirror/ast/mrrexpr.hpp"

namespace mirror {
	

	
	llvm::Function* get_func(const std::string& name) {
		llvm::Function* fn = compiler::get_current()->Module->getFunction(name);

		if (!fn) {
			log_error("Unknown function referenced %s", name.c_str());
			return nullptr;
		}

		return fn;
	}

	llvm::Constant* const_str(const std::string& val) {
		return compiler::get_current()->Builder->CreateGlobalStringPtr(llvm::StringRef(val));
	}

}
