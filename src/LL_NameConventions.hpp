#pragma once
#include "System.hpp"

/*
	____namespace:
		...
	___object:
		...
	__function:
		...

	**examples**

	____namespace___object__function:
		...
	____namespace__function:
		...
	___object__function:
		...
	__function:
		...
*/

inline std::string ConvertToLL(std::string ConvName) {
	if (!System::Text::ContainsSpecialChar(ConvName, "0123456789")) return "__" + ConvName;
	return System::Text::replace(System::Text::replace(System::Text::replace(ConvName, "@", "____"), ":", "___"), ".", "__");
}
inline std::string ConvertToHL(std::string ConvName) {
	if (System::Text::StartsWih(ConvName, "__") && System::Text::occurrence(ConvName, "_") == 2) return ConvName.substr(2);
	return System::Text::replace(System::Text::replace(System::Text::replace(ConvName, "____", "@"), "___", ":"), "__", ".");
}
