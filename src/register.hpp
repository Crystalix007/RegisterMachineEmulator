#pragma once

#include <cstdint>
#include <gmpxx.h>
#include <utility>
#include <vector>
#include <ios>

using Register = mpz_class;
using RegIndex = uint16_t;

template <bool mapZero>
std::pair<Register, Register> toPair(Register unsplit);
std::vector<Register> toList(Register unsplit);
std::ostream& printInstruction(std::ostream& sout, Register instruction);
