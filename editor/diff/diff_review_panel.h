/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#ifndef DIFF_REVIEW_PANEL_H
#define DIFF_REVIEW_PANEL_H

#include "scene/gui/split_container.h"
#include "scene/gui/code_edit.h"
#include "diff_review_controller.h"
#include "diff_overlay_widget.h"

class DiffReviewPanel : public HSplitContainer {
	GDCLASS(DiffReviewPanel, HSplitContainer);

private:
	CodeEdit *original_editor = nullptr;
	CodeEdit *modified_editor = nullptr;
	DiffOverlayWidget *overlay_widget = nullptr;

	Ref<DiffReviewController> controller;

	void _on_hunk_changed(Ref<DiffHunk> p_hunk);
	void _on_hunks_updated();

	void color_hunk_in_editor(CodeEdit *p_editor, Ref<DiffHunk> p_hunk, Color p_color);
	Color color_for_kind(int p_kind);

	void clear_all_highlighting(CodeEdit *p_editor);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	DiffReviewPanel();

	void setup(Ref<DiffReviewController> p_controller);
};

#endif // DIFF_REVIEW_PANEL_H
