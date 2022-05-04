#include "linux/kvm.h"
#include <sys/ioctl.h>
#include <stdio.h>

int arch_init_sregs(int vcpufd)
{
	int ret;
	struct kvm_sregs sregs;

        ret = ioctl(vcpufd, KVM_GET_SREGS, &sregs);
        if (ret < 0) {
                printf("get sregs for vcpu fail");
                return ret;
        }

	sregs.cs.base = 0;
	sregs.cs.selector = 0;
        ret = ioctl(vcpufd, KVM_SET_SREGS, &sregs);
	if (ret < 0) {
		printf("kvm set sregs fail\n");
		return ret;
	}

	return 0;
}

int arch_init_regs(int vcpufd)
{
	int ret;

	struct kvm_regs regs = {
		.rip = 0,
	};
	ret = ioctl(vcpufd, KVM_SET_REGS, &regs);

	return ret;
}

int arch_init_vcpu(int vcpufd, int vmfd, unsigned long guest_start)
{
	return 0;
}

int arch_handle_mmio(struct kvm_run * run)
{
	return 0;
}
