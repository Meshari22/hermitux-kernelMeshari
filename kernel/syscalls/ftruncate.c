#include <hermit/syscall.h>
#include <hermit/logging.h>
#include <hermit/minifs.h>
#include <asm/page.h>
#include <asm/uhyve.h>

 

typedef struct {
    int fd;
    off_t length;
    int ret;
} _attribute_((packed)) uhyve_ftruncate_t;