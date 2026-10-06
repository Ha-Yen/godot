/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "diff_engine.h"
#include "core/math/math_funcs.h"

Vector<String> DiffEngine::split_lines(const String &p_text) {
	return p_text.split("\n", false);
}

int DiffEngine::lcs_length(const Vector<String> &a, const Vector<String> &b, int i, int j, Vector<Vector<int>> &memo) {
	if (i == 0 || j == 0) {
		return 0;
	}
	if (memo[i][j] != -1) {
		return memo[i][j];
	}

	if (a[i - 1] == b[j - 1]) {
		memo[i][j] = 1 + lcs_length(a, b, i - 1, j - 1, memo);
	} else {
		memo[i][j] = MAX(lcs_length(a, b, i - 1, j, memo), lcs_length(a, b, i, j - 1, memo));
	}
	return memo[i][j];
}

void DiffEngine::backtrack_lcs(const Vector<String> &a, const Vector<String> &b, int i, int j,
								 Vector<Vector<int>> &lcs_table, Vector<Ref<DiffHunk>> &out_hunks) {
	while (i > 0 && j > 0) {
		if (i > 0 && j > 0 && a[i - 1] == b[j - 1]) {
			i--;
			j--;
		} else if (i > 0 && (j == 0 || lcs_table[i - 1][j] >= lcs_table[i][j - 1])) {
			i--;
		} else if (j > 0) {
			j--;
		}
	}
}

Vector<Ref<DiffHunk>> DiffEngine::compute_hunks(const String &p_original, const String &p_modified) {
	Vector<String> orig_lines = split_lines(p_original);
	Vector<String> mod_lines = split_lines(p_modified);

	int n = orig_lines.size();
	int m = mod_lines.size();

	// Simplified diff: compare line by line and create hunks
	Vector<Ref<DiffHunk>> hunks;

	int i = 0, j = 0;
	while (i < n || j < m) {
		if (i < n && j < m && orig_lines[i] == mod_lines[j]) {
			// Lines match, move forward
			i++;
			j++;
		} else if (i < n && (j >= m || orig_lines[i] != mod_lines[j])) {
			// Line deleted or modified
			int start_i = i;
			int start_j = j;

			while (i < n && j < m && orig_lines[i] != mod_lines[j]) {
				i++;
				j++;
			}

			if (j == start_j) {
				// Pure deletion
				Ref<DiffHunk> hunk = memnew(DiffHunk(DIFF_HUNK_DELETE, start_i, i, start_j, start_j));
				hunks.push_back(hunk);
			} else if (i == start_i) {
				// Pure insertion
				Ref<DiffHunk> hunk = memnew(DiffHunk(DIFF_HUNK_INSERT, start_i, start_i, start_j, j));
				hunks.push_back(hunk);
			} else {
				// Modification
				Ref<DiffHunk> hunk = memnew(DiffHunk(DIFF_HUNK_MODIFY, start_i, i, start_j, j));
				hunks.push_back(hunk);
			}
		} else if (j < m) {
			// Line inserted
			int start_j = j;
			while (j < m && (i >= n || orig_lines[i] != mod_lines[j])) {
				j++;
			}
			Ref<DiffHunk> hunk = memnew(DiffHunk(DIFF_HUNK_INSERT, i, i, start_j, j));
			hunks.push_back(hunk);
		}
	}

	return hunks;
}
