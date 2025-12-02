#ifndef INCLUDE_RFIM_RFI_STRATEGY
#define INCLUDE_RFIM_RFI_STRATEGY

#include"TimeFrequency.h"

namespace rfim {

	// Define: 
	// using DataType_t = DataType;
	// in every concrete class

	template <typename Derived>
	class RfiStrategy {
	public:
		template<typename TimeFrequencyType>
		std::size_t process(TimeFrequencyType& buffer)
		{
			return static_cast<Derived*>(this)->processImpl(buffer);
		}
	};

} // namespace: rfim
#endif