#include "register.hpp"
#include <iomanip>
#include <iostream>
#include <map>

Register& getOrDefault(std::map<RegIndex, Register>& mapping, const RegIndex& lookup);

int main() {
	const constexpr size_t registerIndexPadding = 6;
	bool executing = true;

	std::cout << "Please enter the program" << std::endl;

	RegIndex pc = 0;
	Register reg;
	std::map<RegIndex, Register> registers{};

	std::cin >> reg;
	auto registerStates = toList(reg);

	std::cout << "\nRegister states at startup:\n";
	RegIndex i = 0;

	for (; i < registerStates.size(); i++) {
		const auto registerState = registerStates[i];

		registers.insert(std::make_pair(i, registerState));
		std::cout << std::setw(registerIndexPadding) << i << ": " << registerState << " => ";
 		printInstruction(std::cout, registerState) << std::endl;
	}

	std::cout << std::setw(registerIndexPadding) << i << ": 0...\n\n";
	std::cout << "Beginning Execution..." << std::endl;

	registers.insert(std::make_pair(pc, reg));

	while (executing) {
		std::cout << "L" << pc << "\t";
		Register instruction = getOrDefault(registers, pc);
		printInstruction(std::cout, instruction) << std::endl;

		if (instruction == 0) { /* HALT instruction. */
			executing = false;
			continue;
		}

		auto splitInstruction = toPair<false>(instruction);

		if (splitInstruction.first % 2 == 0) { /* R+ instruction */
			RegIndex sourceRegisterIndex = splitInstruction.first.get_ui();
			RegIndex destinationLabel = splitInstruction.second.get_ui();

			getOrDefault(registers, sourceRegisterIndex)++;
			pc = destinationLabel;
		} else {
			RegIndex sourceRegisterIndex = splitInstruction.first.get_ui();

			auto branches = toPair<true>(splitInstruction.second);
			RegIndex trueLabel = branches.first.get_ui();
			RegIndex falseLabel = branches.second.get_ui();

			auto& sourceRegister = getOrDefault(registers, sourceRegisterIndex);

			if (sourceRegister > 0) {
				sourceRegister--;
				pc = trueLabel;
			} else
				pc = falseLabel;
		}
	}
}

Register& getOrDefault(std::map<RegIndex, Register>& mapping, const RegIndex& lookup) {
	auto valueIter = mapping.try_emplace(lookup, Register{ 0 });
	return valueIter.first->second;
}
