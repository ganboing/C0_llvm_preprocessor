#include "llvm/IR/Attributes.h"

namespace llvm{
	static_assert(sizeof(AttributeSet) == sizeof(void*), "implmentation changed?");
	//XXX:implements the operator< of AttributeSet
	static inline bool operator< (const AttributeSet& lhs, const AttributeSet& rhs)
	{
		uintptr_t lhs_int;
		uintptr_t rhs_int;
		memcpy(&lhs_int, &lhs, sizeof(uintptr_t));
		memcpy(&rhs_int, &rhs, sizeof(uintptr_t));
		return lhs_int < rhs_int;
	}
}