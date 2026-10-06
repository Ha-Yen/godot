/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Godot Engine contributors.
 *  Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "diff_hunk.h"

void DiffHunk::_bind_methods() {
	CLASSDB_BIND_PROPERTY(PropertyInfo(Variant::INT, "kind"), "set", "get");
	CLASSDB_BIND_PROPERTY(PropertyInfo(Variant::INT, "original_start"), "set", "get");
	CLASSDB_BIND_PROPERTY(PropertyInfo(Variant::INT, "original_end"), "set", "get");
	CLASSDB_BIND_PROPERTY(PropertyInfo(Variant::INT, "modified_start"), "set", "get");
	CLASSDB_BIND_PROPERTY(PropertyInfo(Variant::INT, "modified_end"), "set", "get");
}

DiffHunk::DiffHunk(DiffHunkKind p_kind, int o_start, int o_end, int m_start, int m_end) :
		kind(p_kind),
		original_start(o_start),
		original_end(o_end),
		modified_start(m_start),
		modified_end(m_end) {
}
