#include "llvm/IR/Module.h"
#include <map>

template<class Container, class Function>
class applyCFE : public ::std::pair<Container*, Function*>
{
	typedef ::std::pair<Container*, Function*> _pair;
public:
	applyCFE(const _pair& other)
		:_pair(other){
	}
	template<class Ele>
	void operator() (Ele* pIns) throw()
	{
		_pair::second(pIns, _pair::first);
	}
};

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