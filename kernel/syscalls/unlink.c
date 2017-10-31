#include <hermit/syscall.h>
#include <hermit/spinlock.h>
#include <asm/uhyve.h>
#include <asm/page.h>

extern spinlock_irqsave_t lwip_lock;
extern volatile int libc_sd;

typedef struct {
	const char* pathname;
	int ret;
} __attribute__((packed)) uhyve_unlink_t;


int sys_unlink(const char *pathname) {
	int s, sysnr, i, len, ret;

	if(is_uhyve()) {
		uhyve_unlink_t uhyve_args = { (const char *) virt_to_phys((size_t) pathname), 0};
		uhyve_send(UHYVE_PORT_UNLINK, (unsigned)virt_to_phys((size_t)&uhyve_args));
		return uhyve_args.ret;
	}

	spinlock_irqsave_lock(&lwip_lock);
	s = libc_sd;

	sysnr = __NR_unlink;
	lwip_write(s, &sysnr, sizeof(sysnr));
	
	len = strlen(pathname);
	lwip_write(s, &len, sizeof(len));
	
	i=0;
	while(i < len)
	{
		ret = lwip_write(s, (char*)pathname+i, len-i);
		if (ret < 0) {
			spinlock_irqsave_unlock(&lwip_lock);
			return ret;
		}

		i += ret;
	}

	ret = lwip_read(s, &i, sizeof(i));
	if (ret < 0)
		i = ret;

	spinlock_irqsave_unlock(&lwip_lock);

	return i;
}

