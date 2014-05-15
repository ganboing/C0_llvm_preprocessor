#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/DataStream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "WrapperSet.h"
#include "traits1.hpp"
#include "Exceptions.h"
#include <functional>

using namespace llvm;

namespace C0{

	void IterOverSpawnFunc(Module* M,const ::std::function<void(Instruction*)>& F)
	{

	}

	void IterOverSpawnFunc_Replace(Module* M,const ::std::function<Instruction*(Instruction*)>& F)
	{

	}

	class collect1{
	public:
		explicit collect1(SpawnFuncMap_t* p) : pMap(p){}
		collect1(const collect1& other) : pMap(other.pMap){}
		void operator() (Instruction* pIns)
		{
			CallInst* pCall;
			if ((pCall = dyn_cast<CallInst>(pIns)))
			{
				const AttributeSet* pCallAttr = &(pCall->getAttributes());
				PointerType* pFuncTyP;
				if ((pFuncTyP = dyn_cast<PointerType>(pCall->getCalledValue()->getType())))
				{
					FunctionType* pFuncTy;
					if ((pFuncTy = dyn_cast<FunctionType>(pFuncTyP->getElementType())))
					{
						CallEntryType_t CallEntTy(::std::make_pair(pFuncTy, pCallAttr));
						SpawnFuncMap_t::iterator iMap = pMap->find(CallEntTy);
						if (iMap == pMap->end())
						{
							return;
						}
						try{
							pMap->insert(::std::make_pair(CallEntTy, /*Make2(CallEntTy)*/SpawnWrapperPack_t()));
						}
						catch (...){
							return;
						}
					}
				}
			}

		}
	private:
		SpawnFuncMap_t* pMap;
	};

	/*SpawnWrapperPack_t Make2(CallEntryType_t pack)
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
	*/



	void test1(Module* M){
		SpawnFuncMap_t Map;
		collect1 collector(&Map);
		IterOverSpawnFunc(M, collector);
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

	C0::test1(M.get());

	return 0;
}