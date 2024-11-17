// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#pragma once

namespace FG24 {


class InputManager {
public:
	bool ProcessInputs(); // returns false when stop request is called

	// Temporary input method
	bool input_w;
	bool input_d;
	bool input_s;
	bool input_a;
};
}
