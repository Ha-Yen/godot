/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#ifndef DIFF_DEMO_WINDOW_H
#define DIFF_DEMO_WINDOW_H

#include "scene/gui/window.h"
#include "scene/gui/button.h"
#include "scene/gui/code_edit.h"
#include "diff_integration.h"

class DiffDemoWindow : public Window {
	GDCLASS(DiffDemoWindow, Window);

private:
	CodeEdit *code_edit = nullptr;
	Ref<DiffIntegration> diff_integration;

	void _on_close_requested();
	void _on_reset_pressed();

protected:
	static void _bind_methods();

public:
	DiffDemoWindow();

	void _notification(int p_what);
};

#endif // DIFF_DEMO_WINDOW_H
