/*
 *  Disk unit test
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
#include <stdio.h>

#include <sys/time.h>

#include <xfire/xfire.h>
#include <xfire/types.h>
#include <xfire/mem.h>
#include <xfire/disk.h>
#include <xfire/string.h>

static void test_hm_insert(struct hashmap *map)
{
	hashmap_add(map, "key1", "test-val-1");
	hashmap_add(map, "key2", "test-val-2");
	hashmap_add(map, "key3", "test-val-3");
	hashmap_add(map, "key4", "test-val-4");
}

static struct string *dbg_get_string(const char *c)
{
	struct string *s = xfire_zalloc(sizeof(*s));

	string_init(s);
	string_set(s, c);
	return s;
}

static void dbg_hm_store(struct disk *d)
{
	struct hashmap map;

	hashmap_init(&map);
	test_hm_insert(&map);
	disk_store_hm(d, "hm-key", &map);
	disk_update_hm(d, "hm-key", "key3", "hm-update-ok");
	disk_delete_hashmapnode(d, "hm-key", "key2");
	hashmap_destroy(&map);

	disk_lookup(d, "hm-key");
}

static void dbg_list_store(struct disk *d)
{
	struct string *s1, *s2, *s3, *s4;
	struct list_head lh;

	list_head_init(&lh);
	s1 = dbg_get_string("entry-1");
	s2 = dbg_get_string("entry-2");
	s3 = dbg_get_string("entry-3");
	s4 = dbg_get_string("entry-3");

	list_rpush(&lh, &s1->entry);
	list_rpush(&lh, &s2->entry);
	list_rpush(&lh, &s3->entry);
	list_rpush(&lh, &s4->entry);

	disk_store_list(d, "list-key", &lh);
	disk_delete_list(d, "list-key", "entry-3");
	disk_update_list(d, "list-key", "entry-3", "entry-4");
	disk_lookup(d, "list-key");

	list_del(&lh, &s1->entry);
	list_del(&lh, &s2->entry);
	list_del(&lh, &s3->entry);
	list_del(&lh, &s4->entry);

	string_destroy(s1);
	string_destroy(s2);
	string_destroy(s3);
	string_destroy(s4);

	xfire_free(s1);
	xfire_free(s2);
	xfire_free(s3);
	xfire_free(s4);
}

int main(int argc, char **argv)
{
	struct disk *d;
	char *lookup;
	struct string *s;

	d = disk_create(SQLITE_DB);
	s = dbg_get_string("test-data");
	if(!disk_store_string(d, "test-key", s))
		fprintf(stdout, "Key store succesfull!\n");

	disk_update_string(d, "test-key", "String update success!");
	lookup = disk_lookup(d, "test-key");
	if(lookup)
		printf("%s = %s\n", "test-key", lookup);

	disk_result_free(lookup);
	string_destroy(s);
	xfire_free(s);

	dbg_list_store(d);
	dbg_hm_store(d);
	disk_destroy(d);
	return -EXIT_SUCCESS;
}

