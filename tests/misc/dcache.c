#include <stic.h>

#include <stddef.h> /* NULL */
#include <string.h> /* memset() strcpy() */
#include <time.h> /* time() */

#include "../../src/cfg/config.h"
#include "../../src/ui/ui.h"
#include "../../src/utils/str.h"
#include "../../src/status.h"

#include "utils.h"

SETUP()
{
	update_string(&cfg.shell, "");
	assert_success(stats_init(&cfg));
}

TEARDOWN()
{
	update_string(&cfg.shell, NULL);
}

TEST(size_does_not_clobber_nitems)
{
	uint64_t size;
	uint64_t nitems;

	dcache_set_at(TEST_DATA_PATH, 10, 11);
	dcache_set_at(TEST_DATA_PATH, 13, DCACHE_UNKNOWN);

	dcache_get_at(TEST_DATA_PATH, &size, &nitems);
	assert_ulong_equal(13, size);
	assert_ulong_equal(11, nitems);
}

TEST(nitems_does_not_clobber_size)
{
	uint64_t size;
	uint64_t nitems;

	dcache_set_at(TEST_DATA_PATH, 10, 11);
	dcache_set_at(TEST_DATA_PATH, DCACHE_UNKNOWN, 12);

	dcache_get_at(TEST_DATA_PATH, &size, &nitems);
	assert_ulong_equal(10, size);
	assert_ulong_equal(12, nitems);
}

TEST(outdated_data_is_detected)
{
	dcache_result_t size, nitems;

	dir_entry_t entry = { .name = ".", .origin = TEST_DATA_PATH };

	dcache_set_at(TEST_DATA_PATH, 10, 11);

	entry.mtime = time(NULL) + 1;

	dcache_get_of(&entry, &size, &nitems);
	assert_false(size.is_valid);
	assert_false(nitems.is_valid);
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 filetype=c : */
