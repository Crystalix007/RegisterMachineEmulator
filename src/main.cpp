#include "register.hpp"
#include <iomanip>
#include <iostream>
#include <map>

using RegMap = std::map<RegIndex, Register>;

Register& getOrDefault(std::map<RegIndex, Register>& mapping, const RegIndex& lookup);
std::pair<RegMap, uint32_t> mapFromRegister(Register listRegister);

int main(int argc, char* argv[]) {
	const constexpr size_t registerIndexPadding = 6, registerContentPadding = registerIndexPadding;
	bool seperateInstructions = false;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--seperate-instructions"))
			seperateInstructions = true;
	}

	const char instructionLabel = (seperateInstructions ? 'L' : 'R');
	bool executing = true;

	std::cout << "Please enter the program" << std::endl;

	RegIndex pc = 0;
	Register programReg;
	std::map<RegIndex, Register> registers{}, program{};

	if (!(std::cin >> programReg)) {
		std::cerr << "Failed to read program from input" << std::endl;
		return -1;
	}

	auto& programMap = seperateInstructions ? program : registers;
	auto& registerMap = seperateInstructions ? registers : program;
	uint32_t instructionCount, registerCount;

	std::tie(programMap, instructionCount) = mapFromRegister(programReg);

	if (seperateInstructions) {
		std::cout << "Please enter the registers" << std::endl;

		if (!(std::cin >> programReg)) {
			std::cerr << " Failed to read register states from input" << std::endl;
			return -2;
		}

		std::tie(registerMap, registerCount) = mapFromRegister(programReg);
	}

	std::cout << "\nProgram state at startup:\n";

	if (seperateInstructions)
		std::cout << "Program:\n";

	for (auto&& instruction : programMap) {
		std::cout << instructionLabel << std::setw(registerIndexPadding) << instruction.first << ": " << std::setw(registerContentPadding) << instruction.second << " ==> ";
		printInstruction(std::cout, instruction.second) << "\n";
	}

	std::cout << instructionLabel << std::setw(registerIndexPadding) << instructionCount << ": " << std::setw(registerContentPadding) << "0" << "... HALT\n\n";

	if (seperateInstructions) {
		std::cout << "Registers:\n";

		for (auto&& instruction : registerMap) {
			std::cout << "R" << std::setw(registerIndexPadding) << instruction.first << ": " << std::setw(registerContentPadding) << instruction.second << "\n";
		}

		std::cout << "R" << std::setw(registerIndexPadding) << registerCount << ": " << std::setw(registerContentPadding) << "0" << "...\n\n";
	}

	std::cout << "Beginning Execution..." << std::endl;

	registers.insert(std::make_pair(pc, programReg));

	while (executing) {
		std::cout << instructionLabel << pc << "\t";
		Register instruction = getOrDefault(programMap, pc);
		printInstruction(std::cout, instruction) << std::endl;

		if (instruction == 0) { /* HALT instruction. */
			executing = false;
			continue;
		}

		auto splitInstruction = toPair<false>(instruction);

		if (splitInstruction.first % 2 == 0) { /* R+ instruction */
			RegIndex sourceRegisterIndex = splitInstruction.first.get_ui() / 2;
			RegIndex destinationLabel = splitInstruction.second.get_ui();

			getOrDefault(registerMap, sourceRegisterIndex)++;
			pc = destinationLabel;
		} else {
			RegIndex sourceRegisterIndex = splitInstruction.first.get_ui() / 2;

			auto branches = toPair<true>(splitInstruction.second);
			RegIndex trueLabel = branches.first.get_ui();
			RegIndex falseLabel = branches.second.get_ui();

			auto& sourceRegister = getOrDefault(registerMap, sourceRegisterIndex);

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

std::pair<RegMap, uint32_t> mapFromRegister(Register listRegister) {
	RegMap map;
	auto registerStates = toList(listRegister);

	RegIndex i = 0;

	for (; i < registerStates.size(); i++) {
		const auto registerState = registerStates[i];

		map.insert(std::make_pair(i, registerState));
	}

	return std::make_pair(map, i);
}
