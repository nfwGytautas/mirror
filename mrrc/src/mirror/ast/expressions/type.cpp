#include "mirror/ast/mrrexpr.hpp"
#include "mirror/utility/log.hpp"
#include "type.hpp"


namespace mirror {

	llvm::Value* mrr_var_expr::codegen() {
		// Look up
		if (compiler::get_current()->NamedValues.find(m_name) == compiler::get_current()->NamedValues.end()) {
			log_error("Unknown variable name referenced %s", m_name.c_str());
			return nullptr;
		}

		return compiler::get_current()->Builder->CreateLoad(compiler::get_current()->NamedValues[m_name]);
	}

	llvm::Value* mrr_var_create_expr::codegen() {
		llvm::Function* fn = compiler::get_current()->Builder->GetInsertBlock()->getParent();
		llvm::Value* initVal = nullptr;

		if (!m_init) {
			initVal = llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(0.0));
		}
		else {
			initVal = m_init->codegen();
		}

		if (initVal == nullptr) {
			return nullptr;
		}

		// Alloca instruction
		llvm::AllocaInst* aa = compiler::createEntryBlockAlloca(fn, m_name);
		compiler::get_current()->Builder->CreateStore(initVal, aa);

		// Add to symbol table
		compiler::get_current()->NamedValues[this->m_name] = aa;

		return initVal;
	}

	llvm::Value* mirror::mrr_num_expr::codegen() {
		return llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(m_val));
	}

	llvm::Value* mirror::mrr_str_expr::codegen() {
		return const_str(m_val);
	}

    llvm::Type* mrr_typedef_expr::codegen() {
        // Create struct type
        llvm::StructType* type = llvm::StructType::create(*compiler::get_current()->llvm);
        type->setName(this->m_name);

        // Add type
        compiler::get_current()->Typedefs[this->m_name] = type;

        std::vector<llvm::Type*> fields;
        fields.reserve(m_fields.size());
        for(int i=0; i < m_fields.size(); i++){
            fields.push_back(compiler::get_type(m_fields[i].type));
        }
        type->setBody(fields);
        compiler::get_current()->FieldMap[m_name] = m_fields;

        // Create constructor
        std::vector<llvm::Type*> args;
        llvm::FunctionType* ft = llvm::FunctionType::get(type, args, false);

        llvm::Function* fn = llvm::Function::Create(ft, (llvm::GlobalValue::LinkageTypes)llvm::Function::ExternalLinkage, m_name, compiler::get_current()->Module);

        if (!fn) {
            log_error("Failed to create constructor");
            return nullptr;
        }

        // Create basic block
        llvm::BasicBlock* bb = llvm::BasicBlock::Create(*compiler::get_current()->llvm, this->m_name + "_ctor", fn);
        compiler::get_current()->Builder->SetInsertPoint(bb);

        // TODO: Should probably clean this up cause this will cause double return instruction
        llvm::AllocaInst* allocaInst = compiler::get_current()->Builder->CreateAlloca(type, 0, "object");
        compiler::get_current()->Builder->CreateRet(compiler::get_current()->Builder->CreateLoad(allocaInst));
        llvm::verifyFunction(*fn);
        compiler::get_current()->FPM->run(*fn);

        fn->print(llvm::errs());

        return type;
    }

    llvm::Value *mrr_field_expr::codegen() {
	    // TODO: Fix type reference, this requires to know the type of the object which the field references but only have the name of it

	    // Load value
        llvm::Value* obj = compiler::get_current()->Builder->CreateLoad(compiler::get_current()->NamedValues[m_object]);

        int field_idx = -1;
        for(size_t i = 0; i < compiler::get_current()->FieldMap[m_object].size(); i++) {
            if (compiler::get_current()->FieldMap[m_object][i].name == m_field) {
                field_idx = i;
                break;
            }
        }

        llvm::Value* val = compiler::get_current()->Builder->CreateStructGEP(obj, field_idx);

        // Look up
        return compiler::get_current()->Builder->CreateLoad(val, m_object + "_" + m_field);
    }
}
