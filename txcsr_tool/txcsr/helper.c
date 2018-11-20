#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

#include "helper.h"

static bool part_probed;
static uint32_t board_variant;
static uint32_t board_part;

static void probe_part(void)
{
#ifdef _X86_HOST_
	board_variant=0;
	board_part=0;
	part_probed=1;
#else
	FILE *cpuinfo;
	char buf[1024];
	char *r;
	char *p;
	bool found_variant = false;
	bool found_part = false;
	long v;

	cpuinfo = fopen("/proc/cpuinfo", "r");
	if (!cpuinfo) {
		perror("ERROR: Failed to open /proc/cpuinfo");
		exit(1);
	}

	while (!(found_variant && found_part)) {
		r = fgets(buf, sizeof(buf), cpuinfo);
		if (!r) {
			perror("ERROR: Failed to read from /proc/cpuinfo");
			exit(1);
		}
		if (strstr(buf, "CPU variant")) {
			p = strchr(buf, ':');
			if (!p) {
				fprintf(stderr, "ERROR: Malformed 'CPU variant': <%s>\n", buf);
				exit(1);
			}
			v = strtol(p + 1, NULL, 0);
			board_variant = (uint32_t)(v << 20);
			found_variant = true;
		} else if (strstr(buf, "CPU part")) {
			p = strchr(buf, ':');
			if (!p) {
				fprintf(stderr, "ERROR: Malformed 'CPU part': <%s>\n", buf);
				exit(1);
			}
			v = strtol(p + 1, NULL, 0);
			board_part = (uint32_t)(v << 4);
			found_part = true;
		}
	}
	part_probed = (found_part && found_variant);
	fclose(cpuinfo);
#endif	
}

uint32_t get_board_variant () {
	if (!part_probed)
		probe_part();
	return board_variant;
}

uint32_t get_board_part () {
	if (!part_probed)
		probe_part();
	return board_part;
}
