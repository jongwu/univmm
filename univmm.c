#include "stdio.h"
#include "linux/kvm.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "kvm.h"

void err_out(char *s) {
	printf("ERROR: %s, errno is %d\n", s, errno);
}

int main() {
	int ret;

	int kvmfd = open("/dev/kvm", O_RDWR);
	if (kvmfd < 0) {
		err_out("open /dev/kvm fail");
		return 0;
	}
	int vmfd = ioctl(kvmfd, KVM_CREATE_VM, 0);
	if (vmfd < 0) {
		err_out("create vm fail");
		return 0;
	}
	unsigned char *ram = mmap(NULL, 0x1000, PROT_READ |PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	if ((long long) ram < 0) {
		err_out("mmap for kvm ram fail");
		return 0;
	}
	int kfd = open("test.bin", O_RDONLY);
	if (kfd < 0) {
		err_out("open test.bin fail");
		return 0;
	}
	ret = read(kfd, ram, 4096);
	if (ret < 0) {
		err_out("read ram fail");
		return 0;
	}
	struct kvm_userspace_memory_region mem = {
		.slot = 0,
		.guest_phys_addr = 0,
		.memory_size = 0x1000,
		.userspace_addr = (unsigned long)ram,
	};
	ret = ioctl(vmfd, KVM_SET_USER_MEMORY_REGION, &mem);
	if (ret < 0) {
		err_out("set user memory region for vm fail");
		return 0;
	}
	int vcpufd = ioctl(vmfd, KVM_CREATE_VCPU, 0);
	if (vcpufd <0) {
		err_out("create vcpu fail");
		return 0;
	}
	int mmap_size = ioctl(kvmfd, KVM_GET_VCPU_MMAP_SIZE, NULL);
	if (mmap_size <0) {
		err_out("get mmap size for vcpu fail");
		return 0;
	}
	struct kvm_run *run = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vcpufd, 0);
	if (!run) {
		err_out("mmap fro vcpufd fail");
		return 0;
	}

	ret = arch_init_sregs(vcpufd);
	if (ret < 0) {
		err_out("kvm init sregs fail");
		return ret;
	}

	ret = arch_init_regs(vcpufd);
	if (ret < 0) {
		err_out("fail to set kvm regs");
		return 0;
	}

	ret = arch_init_vcpu(vmfd, vcpufd, 0);
	if (ret < 0) {
		err_out("fail to init vcpu");
		return 0;
	}
	while(1) {
		ret = ioctl(vcpufd, KVM_RUN, NULL);
		if (ret == -1) {
			printf("KVM_RUN fail, errno is %d\n", errno);
			return -1;
		}
		switch (run->exit_reason) {
			case KVM_EXIT_HLT:
				printf("kvm hlt\n");
				return 0;
			case KVM_EXIT_IO:
				putchar(*((char *)run + run->io.data_offset));
				break;
			case KVM_EXIT_FAIL_ENTRY:
				printf("entry error\n");
				return -1;
			case KVM_EXIT_MMIO:
				ret = arch_handle_mmio(run);
				if (ret) {
					return 0;
				}
				break;
			default:
				printf("other reason: %d\n", run->exit_reason);
				return -1;
		}
	}
	return 0;
}
