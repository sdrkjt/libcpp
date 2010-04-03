/* 
 * Copyright (C) 2006, Intel Corporation
 * 
 * This file is part of irqbalance
 *
 * This program file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program in a file named COPYING; if not, write to the 
 * Free Software Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301 USA
 */

/* 
 * This file contains the code to communicate a selected distribution / mapping
 * of interrupts to the kernel.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "irqbalance.h"


void activate_mapping(void)
{

        int debug_mode = 0;
	if (getenv("IRQBALANCE_DEBUG")) 
	  debug_mode=1;

	struct interrupt *irq;
	GList *iter;

	iter = g_list_first(interrupts);
	while (iter) {
		irq = iter->data;
		iter = g_list_next(iter);

		if (!cpus_equal(irq->mask, irq->old_mask)) {
			char buf[PATH_MAX];
			FILE *file;
			sprintf(buf, "/proc/irq/%i/smp_affinity", irq->number);
			if(debug_mode){
			  printf(buf);printf("\n");
			}
			file = fopen(buf, "w");
			if (!file)
				continue;
			cpumask_scnprintf(buf, PATH_MAX, irq->mask);
			fprintf(file,"%s", buf);
			if(debug_mode){
			  printf(">> ");printf(buf);printf("\n");
			}
			fclose(file);
			irq->old_mask = irq->mask;
		}
	}
}
