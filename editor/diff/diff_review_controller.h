/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#ifndef DIFF_REVIEW_CONTROLLER_H
#define DIFF_REVIEW_CONTROLLER_H

#include "core/object/ref_counted.h"
#include "core/containers/vector.h"
#include "core/string/ustring.h"
#include "diff_hunk.h"

class DiffReviewController : public RefCounted {
	GDCLASS(DiffReviewController, RefCounted);

public:
	DiffReviewController();
	DiffReviewController(const String &p_original, const String &p_modified);

	void initialize(const String &p_original, const String &p_modified);

	Vector<Ref<DiffHunk>> get_hunks() const { return hunks; }
	Ref<DiffHunk> get_current_hunk() const;
	int get_current_index() const { return current_index; }
	int get_hunks_count() const { return hunks.size(); }
	String get_original_text() const { return original_text; }
	String get_modified_text() const { return modified_text; }

	void next_hunk();
	void previous_hunk();
	void accept_hunk(Ref<DiffHunk> p_hunk);
	void reject_hunk(Ref<DiffHunk> p_hunk);

	String get_result_text() const;

protected:
	static void _bind_methods();

	String original_text;
	String modified_text;
	Vector<String> current_lines;
	Vector<Ref<DiffHunk>> hunks;
	int current_index = -1;
};

#endif // DIFF_REVIEW_CONTROLLER_H
