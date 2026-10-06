/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "diff_review_controller.h"
#include "diff_engine.h"

void DiffReviewController::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "original", "modified"), &DiffReviewController::initialize);
	ClassDB::bind_method(D_METHOD("get_hunks"), &DiffReviewController::get_hunks);
	ClassDB::bind_method(D_METHOD("get_current_hunk"), &DiffReviewController::get_current_hunk);
	ClassDB::bind_method(D_METHOD("get_current_index"), &DiffReviewController::get_current_index);
	ClassDB::bind_method(D_METHOD("get_hunks_count"), &DiffReviewController::get_hunks_count);
	ClassDB::bind_method(D_METHOD("next_hunk"), &DiffReviewController::next_hunk);
	ClassDB::bind_method(D_METHOD("previous_hunk"), &DiffReviewController::previous_hunk);
	ClassDB::bind_method(D_METHOD("accept_hunk", "hunk"), &DiffReviewController::accept_hunk);
	ClassDB::bind_method(D_METHOD("reject_hunk", "hunk"), &DiffReviewController::reject_hunk);
	ClassDB::bind_method(D_METHOD("get_result_text"), &DiffReviewController::get_result_text);
	ClassDB::bind_method(D_METHOD("get_original_text"), &DiffReviewController::get_original_text);
	ClassDB::bind_method(D_METHOD("get_modified_text"), &DiffReviewController::get_modified_text);

	ADD_SIGNAL(MethodInfo("hunk_changed", PropertyInfo(Variant::OBJECT, "hunk", PROPERTY_HINT_RESOURCE_TYPE, "DiffHunk")));
	ADD_SIGNAL(MethodInfo("hunks_updated"));
}

DiffReviewController::DiffReviewController() {
}

DiffReviewController::DiffReviewController(const String &p_original, const String &p_modified) {
	initialize(p_original, p_modified);
}

void DiffReviewController::initialize(const String &p_original, const String &p_modified) {
	original_text = p_original;
	modified_text = p_modified;

	Vector<String> lines = DiffEngine::get_split_lines(original_text);
	current_lines = lines;

	hunks = DiffEngine::compute_hunks(original_text, modified_text);

	if (!hunks.is_empty()) {
		current_index = 0;
		emit_signal("hunk_changed", hunks[current_index]);
	} else {
		current_index = -1;
	}
}

Ref<DiffHunk> DiffReviewController::get_current_hunk() const {
	if (current_index < 0 || current_index >= (int)hunks.size()) {
		return nullptr;
	}
	return hunks[current_index];
}

void DiffReviewController::next_hunk() {
	if (hunks.is_empty()) {
		return;
	}
	current_index = (current_index + 1) % hunks.size();
	emit_signal("hunk_changed", hunks[current_index]);
}

void DiffReviewController::previous_hunk() {
	if (hunks.is_empty()) {
		return;
	}
	current_index = (current_index - 1 + (int)hunks.size()) % (int)hunks.size();
	emit_signal("hunk_changed", hunks[current_index]);
}

void DiffReviewController::accept_hunk(Ref<DiffHunk> p_hunk) {
	ERR_FAIL_NULL(p_hunk);

	Vector<String> lines = DiffEngine::get_split_lines(get_result_text());
	Vector<String> mod_lines = DiffEngine::get_split_lines(modified_text);

	switch (p_hunk->kind) {
		case DIFF_HUNK_INSERT: {
			for (int i = p_hunk->modified_start; i < p_hunk->modified_end && i < (int)mod_lines.size(); i++) {
				lines.insert(p_hunk->original_start + (i - p_hunk->modified_start), mod_lines[i]);
			}
		} break;

		case DIFF_HUNK_DELETE: {
			int count = p_hunk->original_end - p_hunk->original_start;
			for (int i = 0; i < count; i++) {
				if (p_hunk->original_start < (int)lines.size()) {
					lines.remove_at(p_hunk->original_start);
				}
			}
		} break;

		case DIFF_HUNK_MODIFY: {
			int replace_count = p_hunk->original_end - p_hunk->original_start;
			for (int i = 0; i < replace_count; i++) {
				if (p_hunk->original_start < (int)lines.size()) {
					lines.remove_at(p_hunk->original_start);
				}
			}
			for (int i = p_hunk->modified_start; i < p_hunk->modified_end && i < (int)mod_lines.size(); i++) {
				lines.insert(p_hunk->original_start + (i - p_hunk->modified_start), mod_lines[i]);
			}
		} break;
	}

	current_lines = lines;
	emit_signal("hunks_updated");
}

void DiffReviewController::reject_hunk(Ref<DiffHunk> p_hunk) {
	Vector<Ref<DiffHunk>> remaining_hunks;
	for (Ref<DiffHunk> h : hunks) {
		if (h != p_hunk) {
			remaining_hunks.push_back(h);
		}
	}
	hunks = remaining_hunks;

	if (!hunks.is_empty()) {
		current_index = MIN(current_index, (int)hunks.size() - 1);
	}

	emit_signal("hunks_updated");
}

String DiffReviewController::get_result_text() const {
	String result;
	for (int i = 0; i < (int)current_lines.size(); i++) {
		result += current_lines[i];
		if (i < (int)current_lines.size() - 1) {
			result += "\n";
		}
	}
	return result;
}
