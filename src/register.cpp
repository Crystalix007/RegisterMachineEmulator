#include "register.hpp"
#include <iostream>

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

std::ostream& printInstruction(std::ostream& sout, Register instruction) {
	if (instruction == 0)
		return sout << "BREAK";

	const auto splitInstruction = toPair<false>(instruction);
	const auto command = splitInstruction.first / 2;

	if (splitInstruction.first % 2 == 0) {
		/* We have an increment instruction, so second parameter is just label. */
			return sout << "R" << command << "\u207a \u21a3 " << splitInstruction.second;
	}

	const auto labels = toPair<true>(splitInstruction.second);

	return sout << "R" << command << "\u207b \u21a3 " << labels.first << ", " << labels.second;
}
