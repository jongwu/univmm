#include <linux/kvm.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
#define KVM_REG_ARM_CORE_REG(name)      (offsetof(struct kvm_regs, name) / sizeof(__u32))
#define ARM64_CORE_REG(x)       (KVM_REG_ARM64 | KVM_REG_SIZE_U64 | \
                                 KVM_REG_ARM_CORE | KVM_REG_ARM_CORE_REG(x))
int arch_init_sregs(int vcpufd)
{
	return 0;
}

int arch_init_regs(int vcpufd)
{
	return 0;
}

int reset_vcpu_aarch64(int vcpu_fd, unsigned long guest_start) {
	struct kvm_one_reg reg;
	unsigned long data;
	int ret;

	reg.addr = (unsigned long)&data;

	/* pstate */
	data    = PSR_D_BIT | PSR_A_BIT | PSR_I_BIT | PSR_F_BIT | PSR_MODE_EL1h;
	reg.id  = ARM64_CORE_REG(regs.pstate);
	ret = ioctl(vcpu_fd, KVM_SET_ONE_REG, &reg);
	if (ret < 0)  {
		printf("set pstate fail, errno is %d\n", errno);
		return ret;
	}

	/* x1 ~ x3 */
	data = 0;
	for (int i=0; i<3; i++) {
		reg.id = ARM64_CORE_REG(regs.regs[i]);
		ret = ioctl(vcpu_fd, KVM_SET_ONE_REG, &reg);
		if (ret < 0) {
			printf("set x%d fail, errno is %d\n", i, errno);
			return ret;
		}
	}
	
	data = guest_start;
	reg.id = ARM64_CORE_REG(regs.pc);
	ret = ioctl(vcpu_fd, KVM_SET_ONE_REG, &reg);
	if (ret < 0) {
		printf("set pc fail, errno is %d\n", errno);
		return ret;
	}

	return 0;
}

int arch_init_vcpu(int vm_fd, int vcpu_fd, unsigned long guest_start)
{
	struct kvm_arm_target *target;
	struct kvm_cpu *vcpu;
	struct kvm_vcpu_init preferred_init;
	struct kvm_vcpu_init vcpu_init;
	int ret;

	for(int i=0; i<sizeof(vcpu_init.features)/4; i++) {
		vcpu_init.features[i] = 0;
	}
	//TODO: need check kvm extension for psci first
	vcpu_init.features[0] |= (1UL << KVM_ARM_VCPU_PSCI_0_2);

	//get prefer cpu target
	ret = ioctl(vm_fd, KVM_ARM_PREFERRED_TARGET, &preferred_init);
	if (ret < 0) {
		printf("fail to get preferred target, errno is %d\n", errno);
		return ret;
	}
	vcpu_init.target = preferred_init.target;

	//init vcpu
	ret = ioctl(vcpu_fd, KVM_ARM_VCPU_INIT, &vcpu_init);
	if (ret < 0) {
		printf("fail to init vcpu, errno is %d\n", errno);
		return ret;
	}

	ret = reset_vcpu_aarch64(vcpu_fd, guest_start);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

int arch_handle_mmio(struct kvm_run *run)
{
	int len;

	if (!run->mmio.is_write)
		return 0;

	len = run->mmio.len;
	if (len > 8)
		len = 8;

	for (int i = 0; i < len; i++) {
		if (run->mmio.data[i] == 7)
			return 1;
		printf("%c", run->mmio.data[i]);
	}

//	putchar('\n');

	return 0;
}
