#include "mirror/ast/mrrexpr.hpp"
#include "mirror/utility/log.hpp"

namespace mirror {

	llvm::Value* mrr_ast_loop_expr::codegen() {
		switch (this->m_expr->get_type()) {
		case mrr_type::mrr_type_num: {
			llvm::Value* startVal = llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(0.0));
			llvm::Value* endVal = this->m_expr->codegen();
			llvm::Value* StepVal = llvm::ConstantFP::get(*compiler::get_current()->llvm, llvm::APFloat(1.0));

			llvm::Function* TheFunction = compiler::get_current()->Builder->GetInsertBlock()->getParent();

			// Emit the body of the loop.  This, like any other expr, can change the
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

			llvm::Value* CurVar =
				compiler::get_current()->Builder->CreateLoad(ai->getAllocatedType(), ai, c_InnerLoopValue);
			llvm::Value* NextVar = compiler::get_current()->Builder->CreateFAdd(CurVar, StepVal, "nextvar");
			compiler::get_current()->Builder->CreateStore(NextVar, ai);

			llvm::Value* EndCond = compiler::get_current()->Builder->CreateFCmpONE(
				NextVar, endVal, "loopcond");

			// Create the "after loop" block and insert it.
			llvm::BasicBlock* AfterBB =
				llvm::BasicBlock::Create(*compiler::get_current()->llvm, "afterloop", TheFunction);

			// Insert the conditional branch into the end of LoopEndBB.
			compiler::get_current()->Builder->CreateCondBr(EndCond, LoopBB, AfterBB);

			// Any new code will be inserted in AfterBB.
			compiler::get_current()->Builder->SetInsertPoint(AfterBB);

			return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(*compiler::get_current()->llvm));
		}
		default:
			return nullptr;
		}
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

		// Create blocks
		std::vector<llvm::BasicBlock*> bbs;
		for (size_t i = 0; i < m_expressions.size(); i++) {
			bbs.push_back(llvm::BasicBlock::Create(*compiler::get_current()->llvm, "mc_" + std::to_string(i), fn));
		}

		llvm::BasicBlock* defaultBB = nullptr;
		if (m_default) {
			defaultBB = llvm::BasicBlock::Create(*compiler::get_current()->llvm, "match_default", fn);
		}
		
		llvm::BasicBlock* merge = llvm::BasicBlock::Create(*compiler::get_current()->llvm, "match_merge", fn);

		int i = 0;
		llvm::BasicBlock* lastBB = nullptr;
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

			if (i + 1 < bbs.size()) {
				compiler::get_current()->Builder->CreateCondBr(condV, bbs[i], bbs[i + 1]);
				lastBB = bbs[i + 1];
			}
			else if(defaultBB) {
				compiler::get_current()->Builder->CreateCondBr(condV, bbs[i], defaultBB);
				lastBB = defaultBB;
			}
			else {
				compiler::get_current()->Builder->CreateCondBr(condV, bbs[i], merge);
			}

			i++;
		}

		// Generate match bodies
		std::vector<llvm::Value*> iValues;
		for (size_t i = 0; i < m_expressions.size(); i++) {
			compiler::get_current()->Builder->SetInsertPoint(bbs[i]);
			llvm::Value* val = m_expressions[i].second->codegen();
			compiler::get_current()->Builder->CreateBr(merge);
			bbs[i] = compiler::get_current()->Builder->GetInsertBlock();
			iValues.push_back(val);
		}

		// Generate default
		llvm::Value* defVal = nullptr;
		if (defaultBB) {
			compiler::get_current()->Builder->SetInsertPoint(defaultBB);
			llvm::Value* val = m_default->codegen();
			compiler::get_current()->Builder->CreateBr(merge);
			defaultBB = compiler::get_current()->Builder->GetInsertBlock();
			defVal = val;
		}

		// Generate merge
		compiler::get_current()->Builder->SetInsertPoint(merge);
		llvm::PHINode* pn = compiler::get_current()->Builder->CreatePHI(llvm::Type::getDoubleTy(*compiler::get_current()->llvm), 2, "match_tmp");
		
		for (size_t i = 0; i < iValues.size(); i++) {
			if (iValues[i]) {
				pn->addIncoming(iValues[i], bbs[i]);
			}
		}

		if (defVal) {
			pn->addIncoming(defVal, defaultBB);
		}

		return pn;
	}

}
