#ifndef _KVM_H_
#define _KVM_H_

int arch_init_sregs(int);
int arch_init_regs(int);
int arch_init_vcpu(int, int, unsigned long);
int arch_handle_mmio(struct kvm_run *);

#endif
