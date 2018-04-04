#pragma once
#include "stdafx.h"
#include "ui_object.h"
#include "button.h"

class Menu : public UIObject {
protected:
	std::vector<Button> options;
public:
	Menu(Coord location);
	~Menu();

	void Render() const {}
	void Interact() {}

	void addButton(Button button);
};

