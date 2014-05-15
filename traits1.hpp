#ifndef C0_TRAITS
#define C0_TRAITS

#include "llvm/IR/Module.h"
#include "llvm/Support/InstIterator.h"
#include "Exceptions.h"
#include <map>
#include <functional>

/*template<class Container, class Function>
class applyCFE : public ::std::pair<Container*, Function>
{
	typedef ::std::pair<Container*, Function> _pair;
public:
	applyCFE(const _pair& other)
		:_pair(other){
		::std::function<>();
	}
	template<class Ele>
	void operator() (Ele* pIns) noexcept
	{
		_pair::second(pIns, _pair::first);
	}
};*/

template<class F>
void applyFuncToInsIter(llvm::Module* M, F apply)
{
	for (llvm::Module::iterator iFunc = M->begin(), iFuncE = M->end(); iFunc != iFuncE; ++iFunc)
	{
		for (llvm::inst_iterator iIns = inst_begin(iFunc), iInsE = inst_end(iFunc); iIns != iInsE; ++iIns)
		{
			apply(&*iIns);
		}
	}
}

#endif

