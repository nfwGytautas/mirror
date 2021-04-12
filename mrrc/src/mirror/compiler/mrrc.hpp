#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

#include "mirror/lexer/mrrl.hpp"

namespace mirror {

	namespace compiler {
		/**
		 * @brief Mirror compiler state
		*/
		struct mrrc {
			llvm::LLVMContext* llvm = nullptr;
			llvm::IRBuilder<>* Builder = nullptr;
			llvm::Module* Module = nullptr;
			std::unordered_map<std::string, llvm::AllocaInst*> NamedValues;
			llvm::legacy::FunctionPassManager* FPM = nullptr;
		};

		/**
		 * @brief Initialize mirror compiler
		*/
		void mrrc_init();

		/**
		 * @brief Creates a new mirror compiler state
		 * @return mrrc instance
		*/
		mrrc* mrrc_new();

		/**
		 * @brief Frees mrrc compiler
		 * @param c Compiler instance
		*/
		void free_mrrc(mrrc* c);

		/**
		 * @brief Compiles specified input
		 * @param c mrrc instance
		 * @param l mrrl instance
		 * @return True if compilation was successful, false otherwise
		*/
		bool compile(mrrc* c, lexer::mrrl* l);

		/**
		 * @brief Emits the current mrrc module contained information to the specified file
		 * @param c mrrc instance
		 * @param file Destination file
		*/
		void emit_to_file(mrrc* c, const std::string& file);

		/**
		 * @brief Returns the current compiler instance
		 * @return mrrc instance
		*/
		mrrc* get_current();

		/**
		 * @brief Sets the current compiler instance
		 * @param l mrrc instance to set
		*/
		void set_current(mrrc* c);

		/**
		 * @brief Creates a AllocaInst instruction instance
		 * @param fn Function to create for
		 * @param var Variable name
		*/
		llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* fn, const std::string& var);
	}

}