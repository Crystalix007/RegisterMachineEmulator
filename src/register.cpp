#include "register.hpp"

template <>
std::pair<Register, Register> toPair<false>(Register unsplit) {
	Register x = 0;

	/* Skip to the first 1 bit in representation */
	while (unsplit % 2 != 1) {
		unsplit >>= 1;
		x++;
	}

	unsplit >>= 1;

	return std::make_pair(x, unsplit);
}

template <>
std::pair<Register, Register> toPair<true>(Register unsplit) {
	Register x = 0;

	/* Skip past the first 1 bits in representation */
	while (unsplit % 2 == 1) {
		unsplit >>= 1;
		x++;
	}

	unsplit >>= 1;

	return std::make_pair(x, unsplit);
}

std::vector<Register> toList(Register unsplit) {
	std::vector<Register> list{};

	while (unsplit != 0) {
		const auto splitPair = toPair<false>(unsplit);
		unsplit = splitPair.second;
		list.push_back(splitPair.first);
	}

	return list;
}
