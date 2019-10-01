.section ".crt0","ax"
.global _start
.align	4
.arm
push {r1-r12, lr}
bl handleCommand1
pop {r1-r12, pc}
//.space 132
//ldr r1,= 0x127384
//ldrb r0, [r1, r0]
//bx lr

@ cmp r0, #2
@     ldrlt pc, [pc, r0, lsl #2]
@ bx lr
//== begin of command vector table ==
//.word handleCommand0
//.word handleCommand1
//== end of command vector table ==
_start:
.align
.pool
.end