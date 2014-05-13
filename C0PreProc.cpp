#include "llvm/ADT/Hashing.h"
#include "llvm/ADT/APInt.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/DataStream.h"
#include "llvm/Support/Regex.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "FixUp.h"
#include "WrapperSet.h"
#include "traits1.hpp"
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
using namespace llvm;

namespace C0{

	//typedef ::std::set<Attribute> AttrSet_t;

	/*void InitSupportedParaAttr(AttrSet_t& SupportedParaAttr, LLVMContext& context)
	{
		//SupportedParaAttr.insert(Attribute::get())
	}*/


	void ProduceWrapper(FunctionType* fnty, AttributeSet call_attr)
	{
		FunctionType::get(fnty->getReturnType(), ArrayRef<Type*>(fnty->param_begin(), fnty->param_end()), fnty->isVarArg());
	}


	void test(CallInst* pCallInst)
	{
		pCallInst->getAttributes();
	}

	/*SpawnWrapperPack MakeSpawnWrapperPack(FunctionType* pTy, size_t ith)
	{
	SmallVector<Type*, 4> StruParas;
	StruParas.push_back(pTy);
	StruParas.push_back(Type::getInt64Ty(getGlobalContext()));
	for (FunctionType::param_iterator iPara = pTy->param_begin(), iParaE = pTy->param_end(); iPara != iParaE; ++iPara)
	{
	//if ((*iPara)->i)
	StruParas.push_back(*iPara);
	}
	StructType* wrapper_struc = StructType::create(ArrayRef<Type*>(StruParas), "");
	errs() << "----------" << '\n' << \
	"wrapper created: " << *(cast<Type>(wrapper_struc)) << '\n' << \
	"----------" << '\n';
	//wrapper_struc->isSized()
	return SpawnWrapperPack(FuncPair(nullptr, nullptr), nullptr);
	}*/


}

void TransformC0Unit0(Module* M)
{
	typedef ::std::set<const Type*> TyPtrSet_t;
	typedef ::std::map<::std::string, TyPtrSet_t> TyPtrMap_t;
	typedef TyPtrMap_t::value_type TyPtrMapEnt_t;
	TyPtrMap_t TyPtrMap;
	for (Module::const_iterator iFunc = M->begin(), iFuncE = M->end(); iFunc != iFuncE; ++iFunc)
	{
		for (const_inst_iterator iIns = inst_begin(iFunc), iInsE = inst_end(iFunc); iIns != iInsE; ++iIns)
		{
			for (User::const_op_iterator iOp = iIns->op_begin(), iOpE = iIns->op_end(); iOp != iOpE; ++iOp)
			{
				::std::string Tyname;
				raw_string_ostream TyNameStream(Tyname);
				TyNameStream << *(iOp->get()->getType());
				TyNameStream.flush();
				errs() << Tyname << '\n';
				TyPtrMap_t::iterator iPtrSet = TyPtrMap.find(Tyname);
				if (iPtrSet == TyPtrMap.end())
				{
					iPtrSet = TyPtrMap.insert(TyPtrMapEnt_t(Tyname, TyPtrSet_t())).first;
				}
				iPtrSet->second.insert(iOp->get()->getType());
			}
		}
	}
	for (TyPtrMap_t::const_iterator iMap = TyPtrMap.cbegin(), iMapE = TyPtrMap.cend(); iMap != iMapE; ++iMap)
	{
		bool print_ptrs = false;
		if (iMap->second.size() > 1)
		{
			errs() << "Type " << iMap->first << " is NOT singleton!\n";
			print_ptrs = true;
		}
		else
		{
			errs() << "//Type " << iMap->first << " is singleton!\n";
		}
		for (TyPtrSet_t::const_iterator iSet = iMap->second.begin(), iSetE = iMap->second.end(); iSet != iSetE; ++iSet)
		{
			if (print_ptrs)
			{
				errs() << "  " << (void*)(*iSet) << "  typeID == " << (*iSet)->getTypeID();
			}
		}
	}
}

void TransformC0Unit1(Module* M)
{
	static Regex l0_spawn_block_begin("l0_spawn_call_begin_[0-9]+_[0-9]+");
	typedef ::std::set<const FunctionType*> FuncTySet_t;
	FuncTySet_t SpawnFuncProtoSet;
	C0::SpawnWrapperSet SpawnSet2;
	for (Module::iterator iFunc = M->begin(), iFuncE = M->end(); iFunc != iFuncE; ++iFunc)
	{
		for (Function::iterator iBB = iFunc->begin(), iBBE = iFunc->end(); iBB != iBBE; ++iBB)
		{
			errs() << "  " << iBB->getName() << '\n';
			if (l0_spawn_block_begin.match(iBB->getName()))
			{
				errs() << "--> begin" << '\n';
				for (BasicBlock::iterator iIns = iBB->end(), iInsB = iBB->begin(); iIns != iInsB;)
				{
					--iIns;
					CallInst* pCallIns;
					if (pCallIns = dyn_cast<CallInst>(&*iIns))
					{
						Type* pRawFuncTy = pCallIns->getCalledValue()->getType();
						FunctionType* pFuncTy = cast<FunctionType>(cast<PointerType>(pRawFuncTy)->getElementType());

						pCallIns->getAttributes();
						//errs() << "func type: " << *(pCallIns->getCalledValue()->getType()) << '\n';
						//FIXME:We made an assumption that argument types are in a ptr array
						/*if ((*pFuncTy) == (*pFuncTy))
						{

						}*/
						//SpawnSet2.get(pFuncTy);
						//SpawnFuncProtoSet.insert(pFuncTy);
						//errs() << "is VarArg? " << pFuncTy->isVarArg() << '\n';
						/*if (pCallIns->getCalledFunction())
						{
						const AttributeSet& FuncAttrs = pCallIns->getCalledFunction()->getAttributes();
						errs() << "func attrs: " << FuncAttrs.getAsString(AttributeSet::FunctionIndex) << '\n';
						}
						errs() << "        ";
						for (unsigned int iOp = 0, iOpE = pCallIns->getNumArgOperands(); iOp != iOpE; ++iOp)
						{
						errs() << *(pCallIns->getArgOperand(iOp)->getType()) << " | ";
						}
						errs() << '\n';
						errs() << "    " << *pCallIns << '\n';*/

						//iIns->eraseFromParent();

						//ReplaceInstWithValue(iBB->getInstList(), iIns, Constant::getNullValue());
						break;
						//ReplaceInstWithValue(iBB->getInstList(), iIns, Constant::getNullValue(PointerType::getUnqual(Type::getInt32Ty(getGlobalContext()))));
					}
				}
			}
		}
	}
	//errs() << *M;
	errs() << "----------------\ncaptured func types:\n";
	for (FuncTySet_t::const_iterator iFuncTySet = SpawnFuncProtoSet.cbegin(), iFuncTySetE = SpawnFuncProtoSet.cend(); iFuncTySet != iFuncTySetE; ++iFuncTySet)
	{
		errs() << *const_cast<FunctionType*>(*iFuncTySet) << '\n';
	}
	errs() << "----------------\n";
}

typedef ::std::pair<int, ::std::string> AttrNodeIdxPair_t;

bool Comp_AttrNodeSlotPair_t(const AttrNodeIdxPair_t& a, const AttrNodeIdxPair_t& b)
{
	return (a.first < b.first);
}


/*static_assert(sizeof(AttributeSet) == sizeof(void*), "");
uintptr_t ConvAttrSet2Ptr(AttributeSet set)
{
return *reinterpret_cast<const uintptr_t*>(&set);
}*/


/*void PrintAttrSet(AttributeSet attrset, raw_ostream& os)
{
typedef SmallVector<AttrNodeIdxPair_t, 4> SmallAttrVect_t;
SmallAttrVect_t attrvect;
for (unsigned int i = 0, iE = attrset.getNumSlots(); i != iE; ++i)
{
unsigned int index = attrset.getSlotIndex(i);
attrvect.push_back(::std::make_pair((signed int)index, attrset.getAsString(index)));
::std::sort(attrvect.begin(), attrvect.end(), Comp_AttrNodeSlotPair_t);
}
os << "------------------------\n";
os << "ptr == " << (int*)ConvAttrSet2Ptr(attrset) << '\n';
for (SmallAttrVect_t::iterator i = attrvect.begin(), iE = attrvect.end(); i != iE; ++i)
{
os << i->first << ": " << i->second << '\n';
}
os << "------------------------\n";
}*/


void Conv1(CallInst* pCall)
{
	FunctionType* fnty = cast<FunctionType>(cast<PointerType>(pCall->getCalledValue()->getType())->getElementType());
	typedef SmallVector<Type*, 2> TypeArr_t;
	TypeArr_t TypeArr;
	size_t ith_para = 0;
	TypeArr.push_back(fnty);
	TypeArr.push_back(Type::getInt64Ty(pCall->getContext()));
	Function* fn = pCall->getCalledFunction();
	errs() << "processing function: ";
	if (fn)
	{
		errs() << fn->getName() << '\n';
	}
	else
	{
		errs() << "<indirect>\n";
	}
	for (FunctionType::param_iterator iPara = fnty->param_begin(), iParaE = fnty->param_end(); iPara != iParaE; ++iPara, ++ith_para)
	{
		AttributeSet attrset = pCall->getAttributes();
		if (attrset.hasAttribute(ith_para + 1, Attribute::AttrKind::ByVal))
		{
			TypeArr.push_back(cast<PointerType>(*iPara)->getElementType());
		}
		else
		{
			TypeArr.push_back(*iPara);
		}
	}
	StructType* new_struc = StructType::create(TypeArr);
	errs() << "making struct \n" << *new_struc << '\n';
	for (TypeArr_t::iterator iArr = TypeArr.begin(), iArrE = TypeArr.end(); iArr != iArrE; ++iArr)
	{
		errs() << **iArr << '\n';
	}
}



void collect1(C0::CallEntTypeSet_t* pset, Instruction* pIns) throw()
{
	CallInst* pCall;
	if (pCall = dyn_cast<CallInst>(pIns))
	{
		PointerType* FuncTyP;
		if (FuncTyP = dyn_cast<PointerType>(pCall->getCalledValue()->getType()))
		{
			FunctionType* FuncTy;
			if (FuncTy = dyn_cast<FunctionType>(FuncTyP->getElementType()))
			{

			}
		}
	}
	
}

void Make3(Module* M)
{
	//applyCFE < CallEntTypeSet_t,
}

void Make1(Module* M)
{
	for (Module::iterator iFunc = M->begin(), iFuncE = M->end(); iFunc != iFuncE; ++iFunc)
	{
		for (inst_iterator iIns = inst_begin(iFunc), iInsE = inst_end(iFunc); iIns != iInsE; ++iIns)
		{
			Instruction* pIns = &*iIns;
			CallInst* pCall;
			if (pCall = dyn_cast<CallInst>(pIns))
			{
				Conv1(pCall);
			}
		}
	}
}

void CollectCallInfo(Module* M)
{
	for (Module::iterator iF = M->begin(), iFE = M->end(); iF != iFE; ++iF)
	{
		FunctionType* fnty = iF->getFunctionType();
		ArrayRef<Type*> a;
		FunctionType* newty = FunctionType::get(fnty->getReturnType(), ArrayRef<Type*>(fnty->param_begin(), fnty->param_end()), fnty->isVarArg());
		errs() << "captured function type: \n" << \
			(int*)fnty << '\n' << newty << '\n';
	}
}

void TransformC0Unit2(Module* M)
{
	for (Module::iterator iF = M->begin(), iFE = M->end(); iF != iFE; ++iF)
	{
		errs() << "func: " << iF->getName() << '\n';
		//errs() << "functype: " << *(iF->getType()) << '\n';
		errs() << "arguments are: \n" << \
			"\treturn attr: " << iF->getAttributes().getAsString(AttributeSet::ReturnIndex) << '\n' << \
			"\tfunc attr: " << iF->getAttributes().getAsString(AttributeSet::FunctionIndex) << '\n';
		for (unsigned i = 0; i < iF->getFunctionType()->getNumParams(); i++)
		{
			errs() << "\t arg" << i << ": " << iF->getAttributes().getAsString(i + 1) << '\n';
		}
	}
	errs() << "----------------------------\n";
	typedef ::std::set<AttributeSet> AttrCollectionSet_t;
	AttrCollectionSet_t AttrCollection;
	for (Module::iterator iF = M->begin(), iFE = M->end(); iF != iFE; ++iF)
	{
		iF->getCallingConv();
		for (inst_iterator I = inst_begin(iF), E = inst_end(iF); I != E; ++I)
		{
			CallInst* pCall;
			if (pCall = dyn_cast<CallInst>(&*I))
			{
				pCall->getCallingConv();
				CallSite sCall(pCall);
				Value* f = sCall.getCalledValue();
				sCall.getCallingConv();
				AttributeSet attr = sCall.getAttributes();
				AttrCollection.insert(attr);
				errs() << "arguments are: \n" << \
					"\treturn attr: " << attr.getAsString(AttributeSet::ReturnIndex) << '\n' << \
					"\tfunc attr: " << attr.getAsString(AttributeSet::FunctionIndex) << '\n';
				for (unsigned Iarg = 0, IargE = sCall.arg_size(); Iarg != IargE; ++Iarg)
				{
					errs() << "\t arg" << Iarg << ": " << attr.getAsString(Iarg + 1) << '\n';
				}
				for (Value::use_iterator i = f->use_begin(), iE = f->use_end(); i != iE; ++i)
				{
					errs() << "\tvalue type: " << (i->getName()) << '\n';
				}
				errs() << "call inst function type:" << *(sCall.getCalledValue()->getType()) << '\n';
			}
		}
	}
	for (AttrCollectionSet_t::iterator i = AttrCollection.begin(), iE = AttrCollection.end(); i != iE; ++i)
	{
		//PrintAttrSet(*i, errs());
	}
}



int main(int argc, const char* const* argv)
{
	LLVMContext CurrentContext;

	::std::string ErrorMessage;
	OwningPtr<Module> M;

	// Use the bitcode streaming interface
	DataStreamer *streamer = getDataFileStreamer(argv[1], &ErrorMessage);
	if (streamer) {
		M.reset(getStreamedBitcodeModule(argv[1], streamer, CurrentContext,
			&ErrorMessage));
		if (M.get() != 0 && M->MaterializeAllPermanently(&ErrorMessage)) {
			M.reset();
		}
	}

	Make1(M.get());

	return 0;
}

int main2() {
	LLVMContext Context;

	// Create the "module" or "program" or "translation unit" to hold the
	// function
	Module *M = new Module("test", Context);

	// Create the main function: first create the type 'int ()'
	FunctionType *FT =
		FunctionType::get(Type::getInt32Ty(Context), /*not vararg*/false);

	// By passing a module as the last parameter to the Function constructor,
	// it automatically gets appended to the Module.
	Function *F = Function::Create(FT, Function::ExternalLinkage, "main", M);

	// Add a basic block to the function... again, it automatically inserts
	// because of the last argument.
	BasicBlock *BB = BasicBlock::Create(Context, "EntryBlock", F);

	// Get pointers to the constant integers...
	Value *Two = ConstantInt::get(Type::getInt32Ty(Context), 2);
	Value *Three = ConstantInt::get(Type::getInt32Ty(Context), 3);

	// Create the add instruction... does not insert...
	Instruction *Add = BinaryOperator::Create(Instruction::Add, Two, Three,
		"addresult");

	// explicitly insert it into the basic block...
	BB->getInstList().push_back(Add);

	// Create the return instruction and add it to the basic block
	BB->getInstList().push_back(ReturnInst::Create(Context, Add));

	// Output the bitcode file to stdout
	WriteBitcodeToFile(M, outs());

	// Delete the module and all of its contents.
	delete M;
	return 0;
}
