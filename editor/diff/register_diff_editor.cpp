/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "register_diff_editor.h"
#include "diff_integration.h"
#include "diff_demo_window.h"
#include "diff_review_panel.h"
#include "diff_review_controller.h"
#include "diff_hunk.h"

void register_diff_editor_classes() {
	ClassDB::register_class<DiffHunk>();
	ClassDB::register_class<DiffReviewController>();
	ClassDB::register_class<DiffIntegration>();
	ClassDB::register_class<DiffReviewPanel>();
	ClassDB::register_class<DiffOverlayWidget>();
	ClassDB::register_class<DiffDemoWindow>();
}
