/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "diff_overlay_widget.h"
#include "scene/gui/label.h"
#include "scene/gui/style_box_flat.h"

DiffOverlayWidget::DiffOverlayWidget() {
	Ref<StyleBoxFlat> style = memnew(StyleBoxFlat);
	style->set_bg_color(Color(0.2f, 0.2f, 0.2f, 0.9f));
	style->set_border_color(Color(0.4f, 0.4f, 0.4f));
	add_theme_style_override("panel", style);

	HBoxContainer *hbox = memnew(HBoxContainer);
	add_child(hbox);

	Label *label = memnew(Label);
	label->set_text("Change: ");
	hbox->add_child(label);

	accept_button = memnew(Button);
	accept_button->set_text("Keep");
	accept_button->pressed.connect(Callable(this, "_on_accept_pressed"));
	hbox->add_child(accept_button);

	reject_button = memnew(Button);
	reject_button->set_text("Revert");
	reject_button->pressed.connect(Callable(this, "_on_reject_pressed"));
	hbox->add_child(reject_button);

	hbox->add_child(memnew(Control)); // Spacer

	next_button = memnew(Button);
	next_button->set_text("→ Next");
	next_button->pressed.connect(Callable(this, "_on_next_pressed"));
	hbox->add_child(next_button);

	prev_button = memnew(Button);
	prev_button->set_text("← Prev");
	prev_button->pressed.connect(Callable(this, "_on_prev_pressed"));
	hbox->add_child(prev_button);

	close_button = memnew(Button);
	close_button->set_text("✕");
	close_button->pressed.connect(Callable(this, "_on_close_pressed"));
	hbox->add_child(close_button);

	hide();
}

void DiffOverlayWidget::setup(Ref<DiffReviewController> p_controller, Ref<DiffHunk> p_hunk) {
	controller = p_controller;
	current_hunk = p_hunk;

	if (controller.is_valid()) {
		if (!controller->is_connected("hunk_changed", Callable(this, "_on_hunk_changed"))) {
			controller->connect("hunk_changed", Callable(this, "_on_hunk_changed"));
		}
	}

	show();
}

void DiffOverlayWidget::update_position(Vector2 p_pos) {
	set_global_position(p_pos);
}

void DiffOverlayWidget::_on_accept_pressed() {
	if (controller.is_valid() && current_hunk.is_valid()) {
		controller->accept_hunk(current_hunk);
		controller->next_hunk();
	}
}

void DiffOverlayWidget::_on_reject_pressed() {
	if (controller.is_valid() && current_hunk.is_valid()) {
		controller->reject_hunk(current_hunk);
		controller->next_hunk();
	}
}

void DiffOverlayWidget::_on_next_pressed() {
	if (controller.is_valid()) {
		controller->next_hunk();
	}
}

void DiffOverlayWidget::_on_prev_pressed() {
	if (controller.is_valid()) {
		controller->previous_hunk();
	}
}

void DiffOverlayWidget::_on_close_pressed() {
	hide();
}

void DiffOverlayWidget::_on_hunk_changed(Ref<DiffHunk> p_hunk) {
	current_hunk = p_hunk;
}

void DiffOverlayWidget::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			// Initialization if needed
		} break;
	}
}

void DiffOverlayWidget::_bind_methods() {
	ClassDB::bind_method(D_METHOD("setup", "controller", "hunk"), &DiffOverlayWidget::setup);
	ClassDB::bind_method(D_METHOD("update_position", "pos"), &DiffOverlayWidget::update_position);
	ClassDB::bind_method(D_METHOD("_on_accept_pressed"), &DiffOverlayWidget::_on_accept_pressed);
	ClassDB::bind_method(D_METHOD("_on_reject_pressed"), &DiffOverlayWidget::_on_reject_pressed);
	ClassDB::bind_method(D_METHOD("_on_next_pressed"), &DiffOverlayWidget::_on_next_pressed);
	ClassDB::bind_method(D_METHOD("_on_prev_pressed"), &DiffOverlayWidget::_on_prev_pressed);
	ClassDB::bind_method(D_METHOD("_on_close_pressed"), &DiffOverlayWidget::_on_close_pressed);
	ClassDB::bind_method(D_METHOD("_on_hunk_changed", "hunk"), &DiffOverlayWidget::_on_hunk_changed);
}
