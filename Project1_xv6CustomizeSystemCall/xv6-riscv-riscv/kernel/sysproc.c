#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  struct proc *p = myproc();
   p->exit_status = n;
  printf("\n[Process Exit Status]\n");
  printf("PID: %d\n", p->pid);
  printf("Status: %d\n", n);
  printf("Process %d is exiting...\n", p->pid);

  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  for(int i=0;i<10000;i++);
  printf("Process %s (PID %d) is waiting for a child to exit.\n", myproc()->name, myproc()->pid);
  return wait(p);
}
/*
uint64
sys_wait(void) {
  uint64 addr;
  int timeout;
  
  if (argaddr(0, &addr) < 0) // Get address of the status
    return -1;
  if (argint(1, &timeout) < 0) // Get timeout value
    return -1;

  return wait(addr, timeout);
}
*/


uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_diskspace(void)
{
    return diskspace();
}
