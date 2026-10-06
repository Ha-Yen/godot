/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#ifndef DIFF_ENGINE_H
#define DIFF_ENGINE_H

#include "core/object/ref_counted.h"
#include "core/containers/vector.h"
#include "core/string/ustring.h"
#include "diff_hunk.h"

class DiffEngine : public RefCounted {
	GDCLASS(DiffEngine, RefCounted);

public:
	static Vector<Ref<DiffHunk>> compute_hunks(const String &p_original, const String &p_modified);

private:
	static Vector<String> split_lines(const String &p_text);
	static int lcs_length(const Vector<String> &a, const Vector<String> &b, int i, int j, Vector<Vector<int>> &memo);
	static void backtrack_lcs(const Vector<String> &a, const Vector<String> &b, int i, int j,
						   Vector<Vector<int>> &lcs_table, Vector<Ref<DiffHunk>> &out_hunks);

public:
	static Vector<String> get_split_lines(const String &p_text) { return split_lines(p_text); }
};

#endif // DIFF_ENGINE_H
