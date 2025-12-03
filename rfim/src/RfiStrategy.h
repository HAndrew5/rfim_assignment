#ifndef INCLUDE_RFIM_RFI_STRATEGY
#define INCLUDE_RFIM_RFI_STRATEGY

#include"TimeFrequency.h"

namespace rfim {

	// 
	/*
	This class is the CRTP pattern base class representing some strategy for 
	RFI removal. 
	When defining new derived classes:
	Define: 
		using DataType_t = DataType;
	This allows the templated type of the derived class (e.g float, uint16_t) to be captured

		size_t processImpl(TimeFrequency<DataType>& data_buffer)
	This method should accept a TimeFrequency which will be cleaned in place.
	It should return the number of RFI instances detected and cleaned.

	(See MadRfi.h or MedianStandardDeviationRfi.h for examples)
	It is assumed that an instance of an RfiStrategy derived class always operates on
	TimeFrequencies of the same size and type.
	*/
	template <typename Derived>
	class RfiStrategy {
	public:
		template<typename TimeFrequencyType>
		size_t process(TimeFrequencyType& buffer)
		{
			return static_cast<Derived*>(this)->processImpl(buffer);
		}
	};

} // namespace: rfim
#endif