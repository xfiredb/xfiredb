/*
 *  CONTAINER
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

#include <xfire/xfire.h>
#include <xfire/types.h>
#include <xfire/rbtree.h>
#include <xfire/list.h>
#include <xfire/string.h>
#include <xfire/container.h>

void container_init(struct container *c, u32 magic)
{
	switch(magic) {
	case S_MAGIC:
		string_init(&c->data.string);
		break;

	case LH_MAGIC:
		list_head_init(&c->data.lh);
		break;

	default:
		break;
	}

	c->magic = magic;
}

