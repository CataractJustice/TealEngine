#pragma once

namespace TealEngine {
	namespace Math {
		template<class T>
		//remaps "value" from "fromA" - "toA" range to "fromB" - "toB" range
		T map(T fromA, T toA, T fromB, T toB, T value) {
			return (value - fromA) / (toA - fromA) * (toB - fromB) + fromB;
		}
	}
}