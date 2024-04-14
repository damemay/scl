#include "../print/spin.h"
#include "../print/color.h"
#include "../utils/helpers.h"
#include <time.h>
#include <assert.h>

int main(int argc, char** argv) {
    SCL_LOG(SCL_COLOR_BOLD "test" SCL_COLOR_CLEAR);
    scl_spin spinner = {};
    int res = scl_spin_start(&spinner, "working...");
    assert(res == 0);
    sleep(1);
    scl_spin_restart(&spinner, "another work...");
    assert(res == 0);
    sleep(1);
    scl_spin_restart(&spinner, NULL);
    assert(res == 0);
    sleep(1);
    scl_spin_cancel(&spinner);
    SCL_LOG(SCL_COLOR_FG_YELLOW "done" SCL_COLOR_CLEAR);
}
