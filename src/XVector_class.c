/****************************************************************************
 *                Low-level manipulation of XVector objects                 *
 *                            Author: H. Pag\`es                            *
 ****************************************************************************/
#include "XVector.h"


/****************************************************************************
 * C-level slot getters.
 *
 * Be careful that these functions do NOT duplicate the returned slot.
 * Thus they cannot be made .Call entry points!
 */

static SEXP
	shared_symbol = NULL,
	offset_symbol = NULL,
	length_symbol = NULL;

SEXP _get_XVector_shared(SEXP x)
{
	INIT_STATIC_SYMBOL(shared)
	return GET_SLOT(x, shared_symbol);
}

int _get_XVector_offset(SEXP x)
{
	INIT_STATIC_SYMBOL(offset)
	return INTEGER(GET_SLOT(x, offset_symbol))[0];
}

int _get_XVector_length(SEXP x)
{
	INIT_STATIC_SYMBOL(length)
	return INTEGER(GET_SLOT(x, length_symbol))[0];
}

/* Not a strict "slot getter" but very much like. */
SEXP _get_XVector_tag(SEXP x)
{
	return _get_SharedVector_tag(_get_XVector_shared(x));
}


/****************************************************************************
 * Caching.
 */

Chars_holder _hold_XRaw(SEXP x)
{
	Chars_holder x_holder;
	SEXP tag;
	int offset;

	tag = _get_XVector_tag(x);
	offset = _get_XVector_offset(x);
	x_holder.ptr = (const char *) (RAW(tag) + offset);
	x_holder.length = _get_XVector_length(x);
	return x_holder;
}

Ints_holder _hold_XInteger(SEXP x)
{
	Ints_holder x_holder;
	SEXP tag;
	int offset;

	tag = _get_XVector_tag(x);
	offset = _get_XVector_offset(x);
	x_holder.ptr = (const int *) (INTEGER(tag) + offset);
	x_holder.length = _get_XVector_length(x);
	return x_holder;
}

Doubles_holder _hold_XDouble(SEXP x)
{
	Doubles_holder x_holder;
	SEXP tag;
	int offset;

	tag = _get_XVector_tag(x);
	offset = _get_XVector_offset(x);
	x_holder.ptr = (const double *) (REAL(tag) + offset);
	x_holder.length = _get_XVector_length(x);
	return x_holder;
}


/****************************************************************************
 * C-level slot setters.
 *
 * Be careful that these functions do NOT duplicate the assigned value!
 */

static void set_XVector_shared(SEXP x, SEXP value)
{
	INIT_STATIC_SYMBOL(shared)
	SET_SLOT(x, shared_symbol, value);
	return;
}

static void set_XVector_offset(SEXP x, SEXP value)
{
	INIT_STATIC_SYMBOL(offset)
	SET_SLOT(x, offset_symbol, value);
	return;
}

static void set_XVector_length(SEXP x, SEXP value)
{
	INIT_STATIC_SYMBOL(length)
	SET_SLOT(x, length_symbol, value);
	return;
}

static void set_XVector_slots(SEXP x, SEXP shared, SEXP offset, SEXP length)
{
	set_XVector_shared(x, shared);
	set_XVector_offset(x, offset);
	set_XVector_length(x, length);
}


/****************************************************************************
 * C-level constructors.
 *
 * Be careful that these functions do NOT duplicate their arguments before
 * putting them in the slots of the returned object.
 * Thus they cannot be made .Call entry points!
 */

SEXP _new_XVector(const char *classname, SEXP shared, int offset, int length)
{
	SEXP classdef, ans, ans_offset, ans_length;

	PROTECT(classdef = MAKE_CLASS(classname));
	PROTECT(ans = NEW_OBJECT(classdef));
	PROTECT(ans_offset = ScalarInteger(offset));
	PROTECT(ans_length = ScalarInteger(length));
	set_XVector_slots(ans, shared, ans_offset, ans_length);
	UNPROTECT(4);
	return ans;
}

SEXP _new_XRaw_from_tag(const char *classname, SEXP tag)
{
	SEXP shared, ans;

	PROTECT(shared = _new_SharedVector("SharedRaw", tag));
	PROTECT(ans = _new_XVector(classname, shared, 0, LENGTH(tag)));
	UNPROTECT(2);
	return ans;
}

SEXP _new_XInteger_from_tag(const char *classname, SEXP tag)
{
	SEXP shared, ans;

	PROTECT(shared = _new_SharedVector("SharedInteger", tag));
	PROTECT(ans = _new_XVector(classname, shared, 0, LENGTH(tag)));
	UNPROTECT(2);
	return ans;
}

SEXP _new_XDouble_from_tag(const char *classname, SEXP tag)
{
	SEXP shared, ans;

	PROTECT(shared = _new_SharedVector("SharedDouble", tag));
	PROTECT(ans = _new_XVector(classname, shared, 0, LENGTH(tag)));
	UNPROTECT(2);
	return ans;
}

/* Allocation WITHOUT initialization. */

SEXP _alloc_XRaw(const char *classname, int length)
{
	SEXP tag, ans;

	PROTECT(tag = NEW_RAW(length));
	PROTECT(ans = _new_XRaw_from_tag(classname, tag));
	UNPROTECT(2);
	return ans;
}

SEXP _alloc_XInteger(const char *classname, int length)
{
	SEXP tag, ans;

	PROTECT(tag = NEW_INTEGER(length));
	PROTECT(ans = _new_XInteger_from_tag(classname, tag));
	UNPROTECT(2);
	return ans;
}

SEXP _alloc_XDouble(const char *classname, int length)
{
	SEXP tag, ans;

	PROTECT(tag = NEW_NUMERIC(length));
	PROTECT(ans = _new_XDouble_from_tag(classname, tag));
	UNPROTECT(2);
	return ans;
}

