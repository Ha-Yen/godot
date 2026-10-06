/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#ifndef DIFF_HUNK_H
#define DIFF_HUNK_H

#include "core/object/ref_counted.h"
#include "core/string/ustring.h"

enum DiffHunkKind {
	DIFF_HUNK_INSERT,
	DIFF_HUNK_DELETE,
	DIFF_HUNK_MODIFY
};

class DiffHunk : public RefCounted {
	GDCLASS(DiffHunk, RefCounted);

public:
	DiffHunkKind kind;
	int original_start = 0;
	int original_end = 0;
	int modified_start = 0;
	int modified_end = 0;

	String original_text;
	String modified_text;

	DiffHunk() = default;
	DiffHunk(DiffHunkKind p_kind, int o_start, int o_end, int m_start, int m_end);

	bool is_empty() const { return original_start == original_end && modified_start == modified_end; }

	protected:
	static void _bind_methods();
};

#endif // DIFF_HUNK_H
