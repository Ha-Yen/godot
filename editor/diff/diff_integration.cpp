/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "diff_integration.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/button.h"
#include "scene/gui/h_box_container.h"

void DiffIntegration::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "code_edit", "parent", "original_text"), 
		&DiffIntegration::initialize, DEFVAL(""));
	ClassDB::bind_method(D_METHOD("update_diff"), &DiffIntegration::update_diff);
	ClassDB::bind_method(D_METHOD("show_diff_panel"), &DiffIntegration::show_diff_panel);
	ClassDB::bind_method(D_METHOD("hide_diff_panel"), &DiffIntegration::hide_diff_panel);
	ClassDB::bind_method(D_METHOD("set_original_text", "text"), &DiffIntegration::set_original_text);
	ClassDB::bind_method(D_METHOD("get_controller"), &DiffIntegration::get_controller);
	ClassDB::bind_method(D_METHOD("_on_code_changed"), &DiffIntegration::_on_code_changed);
}

DiffIntegration::DiffIntegration() {
}

void DiffIntegration::initialize(CodeEdit *p_code_edit, Control *p_parent, const String &p_original_text) {
	code_edit = p_code_edit;
	parent_container = p_parent;
	original_text = p_original_text;
	current_text = p_original_text;

	if (code_edit && code_edit->is_connected("text_changed", Callable(this, "_on_code_changed"))) {
		code_edit->disconnect("text_changed", Callable(this, "_on_code_changed"));
	}

	if (code_edit) {
		code_edit->connect("text_changed", Callable(this, "_on_code_changed"));
	}
}

void DiffIntegration::set_original_text(const String &p_text) {
	original_text = p_text;
	update_diff();
}

void DiffIntegration::_on_code_changed() {
	if (!code_edit) {
		return;
	}

	current_text = code_edit->get_text();
	update_diff();
}

void DiffIntegration::update_diff() {
	if (original_text == current_text) {
		hide_diff_panel();
		return;
	}

	if (!controller.is_valid()) {
		controller = memnew(DiffReviewController(original_text, current_text));
	} else {
		controller->initialize(original_text, current_text);
	}

	if (controller->get_hunks_count() == 0) {
		hide_diff_panel();
		return;
	}

	_setup_diff_panel();
	show_diff_panel();
}

void DiffIntegration::_setup_diff_panel() {
	if (!diff_panel && parent_container) {
		diff_panel = memnew(DiffReviewPanel);
		parent_container->add_child(diff_panel);
		diff_panel->set_anchors_preset(Control::PRESET_FULL_RECT);
		diff_panel->set_margin(SIDE_TOP, 100);
	}

	if (diff_panel && controller.is_valid()) {
		diff_panel->setup(controller);
	}
}

void DiffIntegration::show_diff_panel() {
	if (diff_panel) {
		diff_panel->show();
	}
}

void DiffIntegration::hide_diff_panel() {
	if (diff_panel) {
		diff_panel->hide();
	}
}
