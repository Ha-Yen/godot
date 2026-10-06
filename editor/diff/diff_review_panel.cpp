/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "diff_review_panel.h"
#include "scene/gui/v_box_container.h"
#include "scene/gui/label.h"
#include "diff_engine.h"

DiffReviewPanel::DiffReviewPanel() {
	VBoxContainer *left_panel = memnew(VBoxContainer);
	add_child(left_panel);

	Label *original_label = memnew(Label);
	original_label->set_text("Original");
	left_panel->add_child(original_label);

	original_editor = memnew(CodeEdit);
	original_editor->set_custom_minimum_size(Size2(400, 400));
	original_editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	original_editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	left_panel->add_child(original_editor);

	VBoxContainer *right_panel = memnew(VBoxContainer);
	add_child(right_panel);

	Label *modified_label = memnew(Label);
	modified_label->set_text("Modified");
	right_panel->add_child(modified_label);

	modified_editor = memnew(CodeEdit);
	modified_editor->set_custom_minimum_size(Size2(400, 400));
	modified_editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	modified_editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	right_panel->add_child(modified_editor);

	overlay_widget = memnew(DiffOverlayWidget);
	add_child(overlay_widget);
}

void DiffReviewPanel::setup(Ref<DiffReviewController> p_controller) {
	controller = p_controller;

	if (controller.is_valid()) {
		original_editor->set_text(controller->get_original_text());
		modified_editor->set_text(controller->get_modified_text());

		if (!controller->is_connected("hunk_changed", Callable(this, "_on_hunk_changed"))) {
			controller->connect("hunk_changed", Callable(this, "_on_hunk_changed"));
		}
		if (!controller->is_connected("hunks_updated", Callable(this, "_on_hunks_updated"))) {
			controller->connect("hunks_updated", Callable(this, "_on_hunks_updated"));
		}

		overlay_widget->setup(controller, controller->get_current_hunk());

		Ref<DiffHunk> first_hunk = controller->get_current_hunk();
		if (first_hunk.is_valid()) {
			_on_hunk_changed(first_hunk);
		}
	}
}

Color DiffReviewPanel::color_for_kind(int p_kind) {
	switch (p_kind) {
		case DIFF_HUNK_INSERT:
			return Color(0.2f, 0.8f, 0.35f, 0.25f); // Green
		case DIFF_HUNK_DELETE:
			return Color(0.9f, 0.25f, 0.25f, 0.25f); // Red
		case DIFF_HUNK_MODIFY:
			return Color(0.3f, 0.6f, 1.0f, 0.25f); // Blue
		default:
			return Color::TRANSPARENT;
	}
}

void DiffReviewPanel::clear_all_highlighting(CodeEdit *p_editor) {
	ERR_FAIL_NULL(p_editor);
	for (int i = 0; i < p_editor->get_line_count(); i++) {
		p_editor->set_line_background_color(i, Color::TRANSPARENT);
	}
}

void DiffReviewPanel::color_hunk_in_editor(CodeEdit *p_editor, Ref<DiffHunk> p_hunk, Color p_color) {
	ERR_FAIL_NULL(p_editor);
	ERR_FAIL_COND(!p_hunk.is_valid());

	int line_start = p_hunk->original_start;
	int line_end = p_hunk->original_end;

	if (line_end <= line_start) {
		line_end = line_start + 1;
	}

	for (int line = line_start; line < MIN(line_end, p_editor->get_line_count()); line++) {
		p_editor->set_line_background_color(line, p_color);
	}
}

void DiffReviewPanel::_on_hunk_changed(Ref<DiffHunk> p_hunk) {
	if (!p_hunk.is_valid()) {
		return;
	}

	clear_all_highlighting(original_editor);
	clear_all_highlighting(modified_editor);

	Color color = color_for_kind(p_hunk->kind);
	color_hunk_in_editor(original_editor, p_hunk, color);

	if (p_hunk->kind == DIFF_HUNK_INSERT) {
		Ref<DiffHunk> insert_hunk = memnew(DiffHunk(DIFF_HUNK_INSERT, 0, 0, p_hunk->modified_start, p_hunk->modified_end));
		color_hunk_in_editor(modified_editor, insert_hunk, color);
	} else {
		color_hunk_in_editor(modified_editor, p_hunk, color);
	}

	if (original_editor->get_line_count() > p_hunk->original_start) {
		original_editor->set_caret_line(p_hunk->original_start);
		original_editor->center_viewport_to_caret();
	}

	if (modified_editor->get_line_count() > p_hunk->modified_start) {
		modified_editor->set_caret_line(p_hunk->modified_start);
		modified_editor->center_viewport_to_caret();
	}
}

void DiffReviewPanel::_on_hunks_updated() {
	Ref<DiffHunk> current = controller->get_current_hunk();
	if (current.is_valid()) {
		_on_hunk_changed(current);
	}
}

void DiffReviewPanel::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			// Initialization if needed
		} break;
	}
}

void DiffReviewPanel::_bind_methods() {
	ClassDB::bind_method(D_METHOD("setup", "controller"), &DiffReviewPanel::setup);
	ClassDB::bind_method(D_METHOD("_on_hunk_changed", "hunk"), &DiffReviewPanel::_on_hunk_changed);
	ClassDB::bind_method(D_METHOD("_on_hunks_updated"), &DiffReviewPanel::_on_hunks_updated);
}
