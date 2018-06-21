#include <stdlib.h>

#include "my-string.h"

#include "bf_register.h"
#include "functions.h"
#include "list.h"
#include "storage.h"
#include "streams.h"
#include "utils.h"

Var
char_list(const char *s)
{
    Var r;
    int l = strlen(s), i;
    Var ss;
    ss.type = TYPE_STR;
    ss.v.str =  s;

    r = new_list(l);
    for (i = 1; i <= l; ++i) {
        r.v.list[i].type = TYPE_STR;
        r.v.list[i] = substr(ss, 1, 1);
        ss.v.str += strlen(r.v.list[i].v.str);
    }

    return r;
}

static package bf_char_list(Var arglist, Byte next, void *vdata, Objid progr) {
    if (strlen(arglist.v.list[1].v.str) > 150) {
	free_var(arglist);
	return make_error_pack(E_INVARG);
    }
    free_var(arglist);
    return make_var_pack(char_list(arglist.v.list[1].v.str));
}

static package
bf_explode(Var arglist, Byte next, void *vdata, Objid progr)
{
    int nargs = arglist.v.list[0].v.num;
    Stream *brk = new_stream(5);
    stream_add_string(brk, (nargs > 1) ? arglist.v.list[2].v.str : " ");
    Var r;

    if (strcmp(stream_contents(brk), "") == 0) {
	r = char_list(arglist.v.list[1].v.str);
    } else {
	r = new_list(0);
	int i, l = stream_length(brk);
	Stream *tmp = new_stream(5);
	stream_add_string(tmp, arglist.v.list[1].v.str);
	stream_add_string(tmp, stream_contents(brk));

	Var subject;
	subject.type = TYPE_STR;
	subject.v.str = str_dup(reset_stream(tmp));
	free_stream(tmp);

	while (strlen(subject.v.str)) {
	    if ((i = strindex(subject.v.str, stream_contents(brk), 0)) > 1) {
		r = listappend(r, substr(var_dup(subject), 1, i - 1));
	    }
	    subject = substr(subject, i + l, strlen(subject.v.str));
	}
	free_var(subject);
    }
    free_var(arglist);
    free_stream(brk);
    return make_var_pack(r);
}

static package
bf_words(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var r = parse_into_wordlist(arglist.v.list[1].v.str);
    free_var(arglist);
    return make_var_pack(r);
}

static package
bf_from_list(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var r;
    Stream *sep = new_stream(0);
    Stream *result = new_stream(0);

    if ((arglist.v.list[0].v.num > 0) && (arglist.v.list[1].v.list[0].v.num > 0)) {
	Var list = var_ref(arglist.v.list[1]);
	int x;

        stream_add_string(sep, (arglist.v.list[0].v.num > 1) ? arglist.v.list[2].v.str : "");

	stream_add_tostr(result, list.v.list[1]);
	list = listdelete(list, 1);

	for (x = 1; x <= list.v.list[0].v.num; x++) {
		stream_add_string(result, stream_contents(sep));
		stream_add_tostr(result, list.v.list[x]);
	}

	free_var(list);
    } else {
      stream_add_string(result, stream_contents(sep));
    }

    r.type = TYPE_STR;
    r.v.str = str_dup(reset_stream(result));

    free_stream(result);
    free_stream(sep);

    free_var(arglist);
    return make_var_pack(r);
}

static package
bf_intersection(Var arglist, Byte next, void *vdata, Objid progr)
{
    int x;
    for (x = 2; x <= arglist.v.list[0].v.num; x++) {
	if (arglist.v.list[x].type != TYPE_LIST) {
	    free_var(arglist);
	    return make_error_pack(E_TYPE);
	}
    }

    Var r = arglist.v.list[0].v.num ? var_dup(arglist.v.list[1]) : new_list(0);

    if (arglist.v.list[0].v.num > 1) {
	int y;
	for (x = 2; x <= arglist.v.list[0].v.num; x++) {
	    if (r.v.list[0].v.num < arglist.v.list[x].v.list[0].v.num) {
		for (y = 1; y <= r.v.list[0].v.num; y++) {
		    if (!ismember(r.v.list[y], arglist.v.list[x], 0)) {
			r = listdelete(r, y);
			y--;
		    }
        	}
	    } else {
		for (y = 1; y <= arglist.v.list[x].v.list[0].v.num; y++) {
		    if (!ismember(arglist.v.list[x].v.list[y], r, 0)) {
			arglist.v.list[x] = listdelete(arglist.v.list[x], y);
			y--;
		    }
		}
		free_var(r);
		r = var_dup(arglist.v.list[x]);
	    }
	}
    }

    free_var(arglist);
    return make_var_pack(r);
}

static package
bf_diff(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var result = var_dup(arglist.v.list[1]);
    int x, y;

    for (x = 2; x <= arglist.v.list[0].v.num; x++) {
	if (arglist.v.list[x].type != TYPE_LIST) {
	    free_var(result);
	    free_var(arglist);
	    return make_error_pack(E_TYPE);
	}
	for (y = 1; y <= arglist.v.list[x].v.list[0].v.num; y++) {
	    result = setremove(result, arglist.v.list[x].v.list[y]);
	}
    }

    free_var(arglist);
    return make_var_pack(result);
}

static package
bf_union(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var result = arglist.v.list[0].v.num ? var_dup(arglist.v.list[1]) : new_list(0);
    int x, y;

    for (x = 2; x <= arglist.v.list[0].v.num; x++) {
	if (arglist.v.list[x].type != TYPE_LIST) {
	    free_var(arglist);
	    free_var(result);
	    return make_error_pack(E_TYPE);
	}
	for (y = 1; y <= arglist.v.list[x].v.list[0].v.num; y++) {
	    result = setadd(result, arglist.v.list[x].v.list[y]);
	}
    }

    free_var(arglist);
    return make_var_pack(result);
}

static package
bf_reverse(Var arglist, Byte next, void *vdata, Objid progr)
{
    int y, n = arglist.v.list[1].v.list[0].v.num;
    Var r = new_list(0);
    for (y = n; y > 0; y--) {
	r = listappend(r, var_dup(arglist.v.list[1].v.list[y]));
    }
    free_var(arglist);
    return make_var_pack(r);
}

static package
bf_setremove_all(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var dup = var_dup(arglist.v.list[1]);
    int i;

    while ((i = ismember(arglist.v.list[2], dup, 0)) != 0)
	dup = listdelete(dup, i);

    free_var(arglist);
    return make_var_pack(dup);
}

static package
bf_ancestors(Var arglist, Byte next, void *vdata, Objid progr)
{
    if (arglist.v.list[1].v.obj >= 0) {
	Var tmp, ancestors = new_list(0);
	int i = db_object_parent(arglist.v.list[1].v.obj);
	while (i > 0) {
	    tmp.type = TYPE_OBJ;
	    tmp.v.obj = i;
	    ancestors = listappend(ancestors, tmp);
	    free_var(tmp);
	    i = db_object_parent(i);
	}
	free_var(arglist);
	return make_var_pack(ancestors);
    } else {
	free_var(arglist);
	return make_error_pack(E_INVARG);
    }
}

static package
bf_min(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var r = arglist.v.list[1];
    int i, nargs = arglist.v.list[0].v.num, bad_types = 0;

    switch (r.type) {
    case TYPE_INT:      /* integers */
    case TYPE_OBJ:      /* objects */
    case TYPE_ERR:      /* errors */
	for (i = 2; i <= nargs; i++) {
	    if (arglist.v.list[i].type != r.type) {
		bad_types = 1;
		break;
	    } else if (arglist.v.list[i].v.num < r.v.num)
		r = arglist.v.list[i];
	}
	break;
    case TYPE_FLOAT:    /* floats */
	for (i = 2; i <= nargs; i++) {
	    if (arglist.v.list[i].type != TYPE_FLOAT) {
		bad_types = 1;
		break;
	    } else if (*arglist.v.list[i].v.fnum < *r.v.fnum)
		r = arglist.v.list[i];
	}
	break;
    case TYPE_STR:	/* strings */
	for (i = 2; i <= nargs; i++) {
	    if (arglist.v.list[i].type != TYPE_STR) {
		bad_types = 1;
		break;
	    } else if (mystrcasecmp(arglist.v.list[i].v.str, r.v.str) < 0)
		r = arglist.v.list[i];
	}
	if (!bad_types)
	    r.v.str = str_ref(r.v.str);
	break;
    default:
	bad_types = 1;
    }

    r = var_ref(r);
    free_var(arglist);
    if (bad_types)
        return make_error_pack(E_TYPE);
    else
        return make_var_pack(r);
}

static package
bf_max(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var r = arglist.v.list[1];
    int i, nargs = arglist.v.list[0].v.num, bad_types = 0;

    switch (r.type) {
    case TYPE_INT:      /* integers */
    case TYPE_OBJ:      /* objects */
    case TYPE_ERR:      /* errors */
        for (i = 2; i <= nargs; i++) {
            if (arglist.v.list[i].type != r.type) {
                bad_types = 1;
                break;
            } else if (arglist.v.list[i].v.num > r.v.num)
                r = arglist.v.list[i];
        }
        break;
    case TYPE_FLOAT:    /* floats */
        for (i = 2; i <= nargs; i++) {
            if (arglist.v.list[i].type != TYPE_FLOAT) {
                bad_types = 1;
                break;
            } else if (*arglist.v.list[i].v.fnum > *r.v.fnum)
                r = arglist.v.list[i];
        }
        break;
    case TYPE_STR:	/* strings */
        for (i = 2; i <= nargs; i++) {
            if (arglist.v.list[i].type != TYPE_STR) {
                bad_types = 1;
                break;
            } else if (mystrcasecmp(arglist.v.list[i].v.str, r.v.str) > 0)
                r = arglist.v.list[i];
        }
        if (!bad_types)
            r.v.str = str_ref(r.v.str);
        break;
    default:
        bad_types = 1;
    }

    r = var_ref(r);
    free_var(arglist);
    if (bad_types)
        return make_error_pack(E_TYPE);
    else
        return make_var_pack(r);
}

static package
bf_range(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var r;
    int start, end;

    if (arglist.v.list[0].v.num > 1) {
	start = arglist.v.list[1].v.num;
	end = arglist.v.list[2].v.num;
    } else {
	start = 1;
	end = arglist.v.list[1].v.num;
    }

    r.type = TYPE_LIST;

    if (end < start) {
	r = new_list(0);
    } else {
	int delta = abs(end - start);
	if (delta > MIN_LIST_CONCAT_LIMIT) {
	    free_var(arglist);
	    return make_error_pack(E_QUOTA);
	}
	int y = 1, i;
	r = new_list(delta + 1);
	for (i = delta; i >= 0; i--) {
	    r.v.list[y].type = TYPE_INT;
	    r.v.list[y].v.num = end - i;
	    y++;
	}
    }
    free_var(arglist);
    return make_var_pack(r);
}

static package
bf_count(Var arglist, Byte next, void *vdata, Objid progr)
{
    Var dup;
    dup.type = TYPE_INT;
    int i = 0, x;

    for (x = 1; x <= arglist.v.list[2].v.list[0].v.num; x++) {
        if (equality(arglist.v.list[1], arglist.v.list[2].v.list[x], 0)) {
            i++;
        }
    }
    dup.v.num = i;

    free_var(arglist);
    return make_var_pack(dup);
}

void register_goblin(void) {
  register_function("ancestors", 1, 1, bf_ancestors, TYPE_OBJ);

  /* String utilities */
  register_function("char_list", 1, 1, bf_char_list, TYPE_STR);
  register_function("explode", 1, 2, bf_explode, TYPE_STR, TYPE_STR);
  register_function("words", 1, 1, bf_words, TYPE_STR);
  register_function("from_list", 1, 2, bf_from_list, TYPE_LIST, TYPE_STR);

  /* Set utilities */
  register_function("intersection", 1, -1, bf_intersection, TYPE_LIST);
  register_function("difference", 1, -1, bf_diff, TYPE_LIST);
  register_function("union", 1, -1, bf_union, TYPE_LIST);

  /* List utilities */
  register_function("range", 1, 2, bf_range, TYPE_INT, TYPE_INT);
  register_function("reverse", 1, 1, bf_reverse, TYPE_LIST);
  register_function("setremove_all", 2, 2, bf_setremove_all, TYPE_LIST, TYPE_ANY);
  register_function("count", 2, 2, bf_count, TYPE_ANY, TYPE_LIST);

  /* Other usefull functions */
  register_function("min", 1, -1, bf_min, TYPE_ANY);
  register_function("max", 1, -1, bf_max, TYPE_ANY);
}
