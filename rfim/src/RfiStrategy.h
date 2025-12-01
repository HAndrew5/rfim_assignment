#ifndef INCLUDE_RFIM_RFI_STRATEGY
#define INCLUDE_RFIM_RFI_STRATEGY

#include"TimeFrequency.h"

namespace rfim {

	template <typename Derived>
	class RfiStrategy {
	public:
		template<typename TimeFrequencyType>
		void process(TimeFrequencyType& buffer) 
		{
			static_cast<Derived*>(this)->processImpl(buffer);
		}
	};

} // namespace: rfim
#endif