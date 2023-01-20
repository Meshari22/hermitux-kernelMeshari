#include <hermit/syscall.h>
#include <hermit/logging.h>
#include <asm/page.h>
#include <asm/uhyve.h>
#include <hermit/minifs.h>

 

typedef struct {
    char *path;
    off_t length;
    int ret;
} _attribute_((packed)) uhyve_truncate_t;

 

int sys_truncate(const char *path, int length) {
    uhyve_truncate_t arg;

 

    arg.path = (char *)virt_to_phys((size_t)path);
    arg.length = length;
    arg.ret = -1;

 

    uhyve_send(UHYVE_PORT_TRUNCATE, (unsigned)virt_to_phys((size_t)&arg));

 

    return arg.ret;
}