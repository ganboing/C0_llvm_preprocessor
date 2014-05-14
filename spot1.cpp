#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "WrapperSet.h"
#include "traits1.hpp"
#include "Exceptions.h"

using namespace llvm;

namespace C0{
	SpawnWrapperPack_t Make2(CallEntryType_t pack)
	{
		FunctionType* fnty = pack.first;
		if (fnty->isVarArg())
		{
			throw ProcException("varg function not supported!");
		}
		AttributeSet attrset = pack.second;
		typedef SmallVector<Type*, 2> TypeArr_t;
		typedef ::std::map<unsigned, unsigned> i2s_map_t;
		i2s_map_t i2s_map;
		for (unsigned iSlot = 0, iSlotE = attrset.getNumSlots(); iSlot != iSlotE; ++iSlot)
		{
			i2s_map.insert(::std::make_pair(attrset.getSlotIndex(iSlot), iSlot));
		}
		errs() << "listing para attrs\n";
		for (i2s_map_t::iterator i = i2s_map.begin(), iE = i2s_map.end(); i != iE; ++i)
		{
			unsigned index = i->first;
			errs() << "at index: " << index;
			unsigned slot = i->second;
			for (AttributeSet::iterator j = attrset.begin(slot), jE = attrset.end(slot); j != jE; ++j)
			{
				errs() << j->getAsString() << '\n';
			}
		}
		TypeArr_t TypeArr;
		size_t ith_para = 0;
		TypeArr.push_back(fnty);
		TypeArr.push_back(Type::getInt64Ty(fnty->getContext()));
		for (FunctionType::param_iterator iPara = fnty->param_begin(), iParaE = fnty->param_end(); iPara != iParaE; ++iPara, ++ith_para)
		{
			if (attrset.hasAttribute(ith_para + 1, Attribute::AttrKind::ByVal))
			{
				TypeArr.push_back(cast<PointerType>(*iPara)->getElementType());
			}
			else
			{
				TypeArr.push_back(*iPara);
			}
		}
		Function::Create(fnty, GlobalValue::LinkageTypes::ExternalLinkage);
	}


	void collect1(Instruction* pIns, SpawnFuncMap_t* pmap) noexcept
	{
		CallInst* pCall;
		if (pCall = dyn_cast<CallInst>(pIns))
		{
			const AttributeSet* pCallAttr = &(pCall->getAttributes());
			PointerType* pFuncTyP;
			if (pFuncTyP = dyn_cast<PointerType>(pCall->getCalledValue()->getType()))
			{
				FunctionType* pFuncTy;
				if (pFuncTy = dyn_cast<FunctionType>(pFuncTyP->getElementType()))
				{
					CallEntryType_t CallEntTy(::std::make_pair(pFuncTy, pCallAttr));
					SpawnFuncMap_t::iterator iMap = pmap->find(CallEntTy);
					if (iMap == pmap->end())
					{
						return;
					}
					try{
						pmap->insert(::std::make_pair(CallEntTy, Make2(CallEntTy)));
					}
					catch (...){
						return;
					}
				}
			}
		}
	}

	void test1(Module* M){
		SpawnFuncMap_t Map;
		applyCFE apply1(::std::make_pair(&Map, collect1));
	}
}