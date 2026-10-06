/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#ifndef DIFF_OVERLAY_WIDGET_H
#define DIFF_OVERLAY_WIDGET_H

#include "scene/gui/panel_container.h"
#include "scene/gui/h_box_container.h"
#include "scene/gui/button.h"
#include "diff_review_controller.h"
#include "diff_hunk.h"

class DiffOverlayWidget : public PanelContainer {
	GDCLASS(DiffOverlayWidget, PanelContainer);

private:
	Button *accept_button = nullptr;
	Button *reject_button = nullptr;
	Button *next_button = nullptr;
	Button *prev_button = nullptr;
	Button *close_button = nullptr;

	Ref<DiffReviewController> controller;
	Ref<DiffHunk> current_hunk;

	void _on_accept_pressed();
	void _on_reject_pressed();
	void _on_next_pressed();
	void _on_prev_pressed();
	void _on_close_pressed();

	void _on_hunk_changed(Ref<DiffHunk> p_hunk);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	DiffOverlayWidget();

	void setup(Ref<DiffReviewController> p_controller, Ref<DiffHunk> p_hunk);
	void update_position(Vector2 p_pos);
};

#endif // DIFF_OVERLAY_WIDGET_H
