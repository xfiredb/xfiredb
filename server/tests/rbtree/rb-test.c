/*
 *  RBTree test
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
#include <string.h>
#include <math.h>

#include <xfire/rbtree.h>

struct rbtree_node {
	struct rbtree node;
	const char *data;
};

static bool duplicate_compare(struct rbtree *node)
{
	struct rbtree_node *container;

	container = container_of(node, struct rbtree_node, node);
	if(!strcmp(container->data, "Hello World 2!"))
		return true;

	return false;
}

int main(int argc, char **argv)
{
	struct rbtree_root root;
	struct rbtree_node *node;
	struct rbtree *find;
	int i = 1;

	memset(&root, 0, sizeof(root));

	for(; i <= 9; i++) {
		node = malloc(sizeof(*node));
		rbtree_set_key(&node->node, i);
		node->data = "Hello World!";
		rbtree_insert_duplicate(&root, &node->node);

		if(i == 5) {
			node = malloc(sizeof(*node));
			rbtree_set_key(&node->node, i);
			node->data = "Hello World 2!";
			rbtree_insert_duplicate(&root, &node->node);
		}
	}

	find = rbtree_find_duplicate(&root, 5, &duplicate_compare);
	node = container_of(find, struct rbtree_node, node);
	if(find)
		printf("Found node %llu::%s\n", (unsigned long long)find->key,
				node->data);

	printf("Tree height: %u\n", (unsigned int)root.height);

	rbtree_dump(&root, stdout);

	return -EXIT_SUCCESS;
}
