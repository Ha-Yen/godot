/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "diff_demo_window.h"
#include "scene/gui/v_box_container.h"
#include "scene/gui/h_box_container.h"
#include "scene/gui/label.h"
#include "scene/gui/panel_container.h"

void DiffDemoWindow::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_close_requested"), &DiffDemoWindow::_on_close_requested);
	ClassDB::bind_method(D_METHOD("_on_reset_pressed"), &DiffDemoWindow::_on_reset_pressed);
}

DiffDemoWindow::DiffDemoWindow() {
	set_title("Diff Review - Demo");
	set_size(Vector2i(1000, 600));
	set_position(Vector2i(100, 100));

	VBoxContainer *main_vbox = memnew(VBoxContainer);
	add_child(main_vbox);

	// Title
	Label *title = memnew(Label);
	title->set_text("Edit code below to see diff review in action (like VS Code)");
	main_vbox->add_child(title);

	// Buttons panel
	HBoxContainer *button_panel = memnew(HBoxContainer);
	main_vbox->add_child(button_panel);

	Button *reset_button = memnew(Button);
	reset_button->set_text("Reset to Original");
	reset_button->pressed.connect(Callable(this, "_on_reset_pressed"));
	button_panel->add_child(reset_button);

	// Code editor
	code_edit = memnew(CodeEdit);
	code_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	code_edit->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	// Original code example
	String original_code = 
		"func hello():\n"
		"    print(\"Hello World\")\n"
		"    var x = 10\n"
		"    return x\n";

	code_edit->set_text(original_code);
	main_vbox->add_child(code_edit);

	// Setup diff integration
	diff_integration = memnew(DiffIntegration);
	diff_integration->initialize(code_edit, main_vbox, original_code);

	close_requested.connect(Callable(this, "_on_close_requested"));
}

void DiffDemoWindow::_on_close_requested() {
	queue_free();
}

void DiffDemoWindow::_on_reset_pressed() {
	if (code_edit) {
		String original_code = 
			"func hello():\n"
			"    print(\"Hello World\")\n"
			"    var x = 10\n"
			"    return x\n";
		code_edit->set_text(original_code);
	}
}

void DiffDemoWindow::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			// Window is ready
		} break;
	}
}
