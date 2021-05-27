#include "mirror/ast/mrrexpr.hpp"
#include "mirror/utility/log.hpp"

namespace mirror {

	llvm::Value* mrr_ast_loop_expr::codegen() {
        llvm::Value* startVal = llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(0.0));
        llvm::Value* StepVal = llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(1.0));

        llvm::Function* TheFunction = compiler::get_current()->Builder->GetInsertBlock()->getParent();

        // Emit the body of the loop. This, like any other expr, can change the value
        llvm::AllocaInst* ai = compiler::createEntryBlockAlloca(TheFunction, c_InnerLoopValue);
        compiler::get_current()->NamedValues[c_InnerLoopValue] = ai;

        // Store the value into the alloca.
        compiler::get_current()->Builder->CreateStore(startVal, ai);

        llvm::BasicBlock* LoopBB = llvm::BasicBlock::Create(*compiler::get_current()->llvm, "loop", TheFunction);

        // Insert an explicit fall through from the current block to the LoopBB.
        compiler::get_current()->Builder->CreateBr(LoopBB);

        // Start insertion in LoopBB.
        compiler::get_current()->Builder->SetInsertPoint(LoopBB);

        m_body->codegen();

        // _it value set
        llvm::Value* CurVar =
                compiler::get_current()->Builder->CreateLoad(ai->getAllocatedType(), ai, c_InnerLoopValue);
        llvm::Value* NextVar = compiler::get_current()->Builder->CreateFAdd(CurVar, StepVal, "nextvar");
        compiler::get_current()->Builder->CreateStore(NextVar, ai);

        // Evaluate end value
        llvm::Value* endVal = this->m_expr->codegen();

        // Evaluate end condition
        llvm::Value* EndCond = nullptr;
		switch (this->m_expr->get_type()) {
            case mrr_type::mrr_type_num: {
                // Just check if current _it == endVal
                EndCond = compiler::get_current()->Builder->CreateFCmpONE(
                    NextVar, endVal, "loopcond_num");

                break;
            }
            case mrr_type::mrr_type_bop: {
                // Check if the condition evaluates to false (0.0)
                EndCond = compiler::get_current()->Builder->CreateFCmpONE(
                        endVal, llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(0.0)),
                        "loopcond_bop");
                break;
            }
            default:
                return nullptr;
		}

        // Create the "after loop" block and insert it.
        llvm::BasicBlock* AfterBB =
                llvm::BasicBlock::Create(*compiler::get_current()->llvm, "afterloop", TheFunction);

        // Insert the conditional branch into the end of LoopEndBB.
        compiler::get_current()->Builder->CreateCondBr(EndCond, LoopBB, AfterBB);

        // Any new code will be inserted in AfterBB.
        compiler::get_current()->Builder->SetInsertPoint(AfterBB);

        return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(*compiler::get_current()->llvm));
	}

	llvm::Value* mrr_ast_match_expr::codegen() {
		// Get value to match
		llvm::Value* mVal = m_val->codegen();
		if (!mVal) {
			log_error("Invalid value for match");
			return nullptr;
		}

		// Now for matching
		llvm::Function* fn = compiler::get_current()->Builder->GetInsertBlock()->getParent();

		llvm::AllocaInst* any_matched = compiler::createEntryBlockAlloca(fn, "any_matched");
		compiler::get_current()->Builder->CreateStore(llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(0.0)), any_matched);
		llvm::Value* match_inc = llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(1.0));

		// Create blocks
		std::vector<llvm::BasicBlock*> bbs;
		std::vector<llvm::BasicBlock*> bbse;
		for (size_t i = 0; i < m_expressions.size(); i++) {
			bbs.push_back(llvm::BasicBlock::Create(*compiler::get_current()->llvm, "mc_" + std::to_string(i), fn));
			bbse.push_back(llvm::BasicBlock::Create(*compiler::get_current()->llvm, "mce_" + std::to_string(i), fn));
		}
		
		llvm::BasicBlock* merge = llvm::BasicBlock::Create(*compiler::get_current()->llvm, "match_merge", fn);

		int i = 0;
		for (auto& ms : m_expressions) {
			// Expression to match
			llvm::Value* cVal = ms.first->codegen();
			if (!cVal) {
				log_error("Match expression value null");
				return nullptr;
			}

			// Compare match
			llvm::Value* condV = nullptr;
			if (ms.first->get_type() == mrr_type::mrr_type_bop) {
				// Check if true or false from binop
				condV = compiler::get_current()->Builder->CreateFCmpONE(cVal, llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(0.0)), "mcond");
			}
			else {
				// Compare values
				condV = compiler::get_current()->Builder->CreateFCmpOEQ(mVal, cVal, "mcond");
			}

			if (i < bbs.size()) {
				compiler::get_current()->Builder->CreateCondBr(condV, bbs[i], bbse[i]);
			}

			compiler::get_current()->Builder->SetInsertPoint(bbse[i]);
			i += 1;
		}

		// Jump to merge at last match block
		if (bbse.size() > 0) {
			compiler::get_current()->Builder->CreateBr(merge);
		}

		// Generate match bodies
		std::vector<llvm::Value*> iValues;
		for (size_t i = 0; i < m_expressions.size(); i++) {
			compiler::get_current()->Builder->SetInsertPoint(bbs[i]);
			llvm::Value* val = m_expressions[i].second->codegen();

			// Increment match count
			llvm::Value* any_matched_val = compiler::get_current()->Builder->CreateLoad(any_matched);
			llvm::Value* v = compiler::get_current()->Builder->CreateFAdd(any_matched_val, match_inc, "add_any_matched");
			compiler::get_current()->Builder->CreateStore(v, any_matched);

			compiler::get_current()->Builder->CreateBr(bbse[i]);
			bbs[i] = compiler::get_current()->Builder->GetInsertBlock();

			if (val) {
				iValues.push_back(val);
			}
		}

		// Generate merge
		compiler::get_current()->Builder->SetInsertPoint(merge);

		// Generate default
		llvm::Value* defVal = nullptr;
		if (m_default) {
			llvm::Value* val = m_default->codegen();
			
			if (val) {
				iValues.push_back(val);
			}
		}

		if (iValues.size() == 0) {
			return nullptr;
		}

		llvm::PHINode* pn = compiler::get_current()->Builder->CreatePHI(llvm::Type::getDoubleTy(*compiler::get_current()->llvm), iValues.size(), "match_tmp");

		for (size_t i = 0; i < iValues.size(); i++) {
			if (iValues[i]) {
				pn->addIncoming(iValues[i], bbs[i]);
			}
		}

		return pn;
	}

}
