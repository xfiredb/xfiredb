/*
 *  XFireDB ruby database
 *  Copyright (C) 2015   Michel Megens <dev@michelmegens.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <ruby.h>

#include <xfire/xfire.h>
#include <xfire/mem.h>
#include <xfire/database.h>
#include <xfire/container.h>
#include <xfire/list.h>
#include <xfire/string.h>
#include <xfire/hashmap.h>

#include "se.h"

static void rb_db_free(void *p)
{
	struct database *db = p;
	db_free(db);
}

VALUE rb_db_new(VALUE klass)
{
	struct database *db = db_alloc("xfire-database");
	return Data_Wrap_Struct(klass, NULL, rb_db_free, db);
}

VALUE rb_db_set(VALUE self, VALUE key, VALUE val)
{
	return self;
}

VALUE rb_db_ref(VALUE self, VALUE key)
{
	struct database *db;
	struct container *c;
	struct db_entry_container *entry;
	struct string *s;
	char *tmp;
	VALUE rv;
	db_data_t dbdata;

	Data_Get_Struct(self, struct database, db);
	if(db_lookup(db, StringValueCStr(key), &dbdata) != -XFIRE_OK)
		return Qnil;

	c = dbdata.ptr;
	entry = container_of(c, struct db_entry_container, c);

	if(entry->type != rb_cString) {
		return entry->obj;
	} else {
		s = container_get_data(&entry->c);
		string_get(s, &tmp);
		rv = rb_str_new2(tmp);
		xfire_free(tmp);

		return rv;
	}
}

static void raw_rb_db_delete(struct db_entry_container *entry)
{
	struct container *c = &entry->c;

	if(entry->type == rb_cString) {
		/* string type, free it here */
		container_destroy(c);
		xfire_free(entry);
	} else if(entry->type == c_list) {
		rb_list_free(entry->obj);
	} else if(entry->type == c_hashmap) {
		rb_hashmap_free(entry->obj);
	}
}

VALUE rb_db_size(VALUE self)
{
	struct database *db;

	Data_Get_Struct(self, struct database, db);
	return LONG2NUM(db_get_size(db));
}

VALUE rb_db_store(VALUE self, VALUE key, VALUE data)
{
	struct database *db;
	struct container *c;
	struct db_entry_container *rb_c;
	struct string *s;
	const char *tmp = StringValueCStr(key);
	db_data_t dbdata;

	Data_Get_Struct(self, struct database, db);
	if(db_delete(db, tmp, &dbdata) == -XFIRE_OK) {
		c = dbdata.ptr;
		rb_c = container_of(c, struct db_entry_container, c);
		raw_rb_db_delete(rb_c);
	}

	if(rb_obj_class(data) != rb_cString) {
		Data_Get_Struct(data, struct db_entry_container, rb_c);
	} else {
		rb_c = xfire_zalloc(sizeof(*rb_c));
		rb_c->obj = Qnil;
		rb_c->type = rb_cString;
		container_init(&rb_c->c, CONTAINER_STRING);
		s = container_get_data(&rb_c->c);
		string_set(s, StringValueCStr(data));
	}

	if(db_store(db, tmp, &rb_c->c) != -XFIRE_OK)
		return Qnil;

	return data;
}

VALUE rb_db_delete(VALUE self, VALUE key)
{
	struct database *db;
	struct container *c;
	struct db_entry_container *entry;
	db_data_t dbdata;

	Data_Get_Struct(self, struct database, db);
	if(db_delete(db, StringValueCStr(key), &dbdata) != -XFIRE_OK)
		return Qnil;

	c = dbdata.ptr;
	entry = container_of(c, struct db_entry_container, c);
	raw_rb_db_delete(entry);

	return key;
}

VALUE c_database;

void init_database(void)
{
	c_database = rb_define_class_under(c_xfiredb_mod,
			"Database", rb_cObject);
	rb_define_singleton_method(c_database, "new", rb_db_new, 0);
	rb_define_method(c_database, "[]=", rb_db_store, 2);
	rb_define_method(c_database, "[]", rb_db_ref, 1);
	rb_define_method(c_database, "delete", rb_db_delete, 1);
	rb_define_method(c_database, "size", rb_db_size, 0);
}

