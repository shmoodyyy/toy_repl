# testing setup

define test functions in header files using `TEST_CASE(test_name_fn())` after `#include "test.h"`

`test/genmain.sh` then greps the files for every defined test to be called from `test/main.c`

assertion failures work but give absolutely no helpful hints of real value outside of assuming that everything is an integer (everything_is_a_file_jak.webp), to be refined.
