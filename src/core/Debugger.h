#pragma once

#include <set>

#include "util/Types.h"

class Memory;

class Debugger {
	Memory& mem;

	bool canContinue = true;
	bool isStepping = false;
	size_t steps = 0;

	std::set<u16> breakpoints;
	using BreakpointIter = std::set<u16>::iterator;

public:
	Debugger(Memory& mem) : mem(mem) {}

	void addBreakpoint(u16 PC) {
		breakpoints.insert(PC);
	}

	BreakpointIter removeBreakpoint(BreakpointIter PC) {
		return breakpoints.erase(PC);
	}

	void removeBreakpoint(u16 PC) {
		breakpoints.erase(PC);
	}

	std::set<u16>& getBreakpoints() {
		return breakpoints;
	}

	void continuing(bool cont) {
		canContinue = cont;
	}

	// to prevent the Show Debugger button from stopping the continue after hiding
	bool isContinuing() {
		return canContinue;
	}

	void step(size_t steps_) {
		isStepping = true;
		steps = steps_;
	}

	size_t amountToStep(u16 PC) {
		if (!breakpoints.empty()) {
			if (breakpoints.find(PC) != breakpoints.end()) {
				continuing(false);
				return 0;
			}
		}

		if (isStepping) {
			canContinue = false; //don't continue after stepping
			isStepping = false;
			return steps;
		}

		if (canContinue) {
			return 1;
		}

		return 0;
	}
};