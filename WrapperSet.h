#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "FixUp.h"
#include <set>

namespace C0{

	typedef ::std::pair<llvm::FunctionType*, llvm::AttributeSet> CallEntryType_t;
	typedef ::std::set<CallEntryType_t> CallEntTypeSet_t;

	typedef ::std::pair<llvm::Function*, llvm::Function*> FuncPair_t;
	typedef ::std::pair<FuncPair_t, llvm::StructType*> SpawnWrapperPack_t;
	typedef ::std::map<CallEntryType_t, SpawnWrapperPack_t> SpawnFuncSet_t;

}