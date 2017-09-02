#include <stic.h>

#include "../../src/cfg/config.h"
#include "../../src/ui/color_scheme.h"
#include "../../src/ui/statusbar.h"
#include "../../src/utils/matchers.h"
#include "../../src/cmd_core.h"
#include "../../src/status.h"

SETUP_ONCE()
{
	init_commands();
	cs_reset(&cfg.cs);
	lwin.list_rows = 0;
	rwin.list_rows = 0;
}

SETUP()
{
	cs_reset(&cfg.cs);
	curr_stats.cs = &cfg.cs;
}

TEST(empty_curly_braces)
{
	const char *const COMMANDS = "highlight {} ctermfg=red";

	assert_false(exec_commands(COMMANDS, &lwin, CIT_COMMAND) == 0);
}

TEST(curly_braces_pattern_transform)
{
	const char *const COMMANDS = "highlight {*.sh}<inode/directory> ctermfg=red";

	assert_int_equal(0, exec_commands(COMMANDS, &lwin, CIT_COMMAND));
	assert_string_equal("{*.sh}<inode/directory>",
			matchers_get_expr(cfg.cs.file_hi[0].matchers));
}

TEST(curly_braces_no_flags_allowed)
{
	const char *const COMMANDS = "highlight {*.sh}i ctermfg=red";

	assert_false(exec_commands(COMMANDS, &lwin, CIT_COMMAND) == 0);
}

TEST(empty_re_without_flags)
{
	const char *const COMMANDS = "highlight // ctermfg=red";

	assert_false(exec_commands(COMMANDS, &lwin, CIT_COMMAND) == 0);
}

TEST(empty_re_with_flags)
{
	const char *const COMMANDS = "highlight //i ctermfg=red";

	assert_false(exec_commands(COMMANDS, &lwin, CIT_COMMAND) == 0);
}

TEST(pattern_is_not_unescaped)
{
	const char *const COMMANDS = "highlight /^\\./ ctermfg=red";

	assert_int_equal(0, exec_commands(COMMANDS, &lwin, CIT_COMMAND));
	assert_string_equal("/^\\./", matchers_get_expr(cfg.cs.file_hi[0].matchers));
}

TEST(pattern_length_is_not_limited)
{
	const char *const COMMANDS = "highlight /\\.(7z|Z|a|ace|alz|apkg|arc|arj|bz"
		"|bz2|cab|cpio|deb|gz|jar|lha|lrz|lz|lzma|lzo|rar|rpm|rz|t7z|tZ|tar|tbz"
		"|tbz2|tgz|tlz|txz|tzo|war|xz|zip)$/ ctermfg=red";

	assert_int_equal(0, exec_commands(COMMANDS, &lwin, CIT_COMMAND));
	assert_string_equal("/\\.(7z|Z|a|ace|alz|apkg|arc|arj|bz"
		"|bz2|cab|cpio|deb|gz|jar|lha|lrz|lz|lzma|lzo|rar|rpm|rz|t7z|tZ|tar|tbz"
		"|tbz2|tgz|tlz|txz|tzo|war|xz|zip)$/",
		matchers_get_expr(cfg.cs.file_hi[0].matchers));
}

TEST(i_flag)
{
	const char *const COMMANDS = "highlight /^\\./i ctermfg=red";

	assert_int_equal(0, exec_commands(COMMANDS, &lwin, CIT_COMMAND));
	assert_string_equal("/^\\./i", matchers_get_expr(cfg.cs.file_hi[0].matchers));
}

TEST(I_flag)
{
	const char *const COMMANDS = "highlight /^\\./I ctermfg=red";

	assert_int_equal(0, exec_commands(COMMANDS, &lwin, CIT_COMMAND));
	assert_string_equal("/^\\./I", matchers_get_expr(cfg.cs.file_hi[0].matchers));
}

TEST(wrong_flag)
{
	const char *const COMMANDS = "highlight /^\\./x ctermfg=red";

	assert_int_equal(1, exec_commands(COMMANDS, &lwin, CIT_COMMAND));
}

TEST(negation)
{
	const char *const COMMANDS = "highlight !/^\\./i ctermfg=red";

	assert_success(exec_commands(COMMANDS, &lwin, CIT_COMMAND));
	assert_string_equal("!/^\\./i",
			matchers_get_expr(cfg.cs.file_hi[0].matchers));
}

TEST(highlighting_is_printed_back_correctly)
{
	const char *const COMMANDS = "highlight {*.jpg} ctermfg=red";
	assert_success(exec_commands(COMMANDS, &lwin, CIT_COMMAND));

	status_bar_message("");
	assert_failure(exec_commands("highlight {*.jpg}", &lwin, CIT_COMMAND));
	assert_string_equal("{*.jpg}    cterm=none ctermfg=red     ctermbg=default",
			get_last_message());
}

TEST(existing_records_are_updated)
{
	const char *const COMMANDS1 = "highlight {*.jpg} ctermfg=red";
	const char *const COMMANDS2 = "highlight {*.jpg} ctermfg=blue";

	assert_success(exec_commands(COMMANDS1, &lwin, CIT_COMMAND));
	assert_success(exec_commands(COMMANDS2, &lwin, CIT_COMMAND));
	assert_int_equal(1, cfg.cs.file_hi_count);

	assert_int_equal(COLOR_BLUE, cfg.cs.file_hi[0].hi.fg);
}

TEST(all_records_can_be_removed)
{
	const char *const COMMANDS1 = "highlight {*.jpg} ctermfg=red";
	const char *const COMMANDS2 = "highlight {*.avi} cterm=bold";
	const char *const COMMANDS3 = "highlight clear";

	assert_success(exec_commands(COMMANDS1, &lwin, CIT_COMMAND));
	assert_success(exec_commands(COMMANDS2, &lwin, CIT_COMMAND));
	assert_int_equal(2, cfg.cs.file_hi_count);
	assert_success(exec_commands(COMMANDS3, &lwin, CIT_COMMAND));
	assert_int_equal(0, cfg.cs.file_hi_count);
}

TEST(records_can_be_removed)
{
	const char *const COMMANDS1 = "highlight {*.jpg} ctermfg=red";
	const char *const COMMANDS2 = "highlight clear {*.avi}";
	const char *const COMMANDS3 = "highlight clear {*.jpg}";

	assert_success(exec_commands(COMMANDS1, &lwin, CIT_COMMAND));
	assert_int_equal(1, cfg.cs.file_hi_count);
	assert_failure(exec_commands(COMMANDS2, &lwin, CIT_COMMAND));
	assert_int_equal(1, cfg.cs.file_hi_count);
	assert_success(exec_commands(COMMANDS3, &lwin, CIT_COMMAND));
	assert_int_equal(0, cfg.cs.file_hi_count);
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 filetype=c : */
