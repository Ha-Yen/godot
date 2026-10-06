/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#ifndef DIFF_INTEGRATION_H
#define DIFF_INTEGRATION_H

#include "core/object/ref_counted.h"
#include "scene/gui/control.h"
#include "scene/gui/code_edit.h"
#include "scene/gui/tab_container.h"
#include "diff_review_panel.h"
#include "diff_review_controller.h"

class DiffIntegration : public RefCounted {
	GDCLASS(DiffIntegration, RefCounted);

private:
	CodeEdit *code_edit = nullptr;
	String original_text;
	String current_text;
	Ref<DiffReviewController> controller;
	DiffReviewPanel *diff_panel = nullptr;
	Control *parent_container = nullptr;
	TabContainer *tab_container = nullptr;

	void _on_code_changed();
	void _on_diff_panel_closed();
	void _setup_diff_panel();

protected:
	static void _bind_methods();

public:
	DiffIntegration();

	void initialize(CodeEdit *p_code_edit, Control *p_parent, const String &p_original_text = "");
	void update_diff();
	void show_diff_panel();
	void hide_diff_panel();
	void set_original_text(const String &p_text);

	Ref<DiffReviewController> get_controller() { return controller; }
};

#endif // DIFF_INTEGRATION_H
