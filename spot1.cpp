#include "llvm/Support/raw_ostream.h"
#include "WrapperSet.h"
#include "traits1.hpp"
#include "Exceptions.h"
#include <exception>

using namespace llvm;

#ifdef _MSC_VER
#define noexcept throw()
#endif

namespace C0{

	class ProcException : public ::std::exception{
	public:
		ProcException(const char* _message) noexcept: message(_message){
		}
		virtual ~ProcException() noexcept{}
		virtual const char* what() const noexcept{ return message; }
	private:
		const char* message;
	};

	SpawnWrapperPack_t Make2(CallEntryType_t pack)
	{
		FunctionType* fnty = pack.first;
		if (fnty->isVarArg())
		{
			throw ::std::exception("varg function not supported!");
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


}