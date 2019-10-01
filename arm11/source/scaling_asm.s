
.global twlLgyStartFbDMAHook
twlLgyStartFbDMAHook:
    MOVS R5, #0
    MOV  R4, R0
    push {r0-r12,lr}
    bl twlLgyStartFbDMAHook_part2
    pop {r0-r12,pc}

// __int64 __cdecl idivmod(unsigned int a1, unsigned int a2)
.global idivmod
idivmod:
                ORRS            R3, R0, R1
                BMI             loc_111F20
                RSBS            R3, R1, R0,LSR#1
                BCC             loc_111F04
                MOVS            R2, #0
                RSBS            R3, R1, R0,LSR#4
                BCC             loc_111ED0
                RSBS            R3, R1, R0,LSR#8
                BCC             loc_111EA0
                RSBS            R3, R1, R0,LSR#12
                BCC             loc_111E6C
                RSBS            R3, R1, R0,LSR#16
                BCC             loc_111E3C
loc_111E14:
                MOV             R1, R1,LSL#8
                RSBS            R3, R1, R0,LSR#16
                ORR             R2, R2, #0xFF000000
                ORRCS           R2, R2, #0xFF0000
                MOVCS           R1, R1,LSL#8
                RSBS            R3, R1, R0,LSR#12
                BCC             loc_111E6C
                RSBS            R3, R1, #0
                BCS             loc_111FFC
loc_111E38:
                MOVCS           R1, R1,LSR#8
loc_111E3C:
                RSBS            R3, R1, R0,LSR#15
                SUBCS           R0, R0, R1,LSL#15
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#14
                SUBCS           R0, R0, R1,LSL#14
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#13
                SUBCS           R0, R0, R1,LSL#13
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#12
                SUBCS           R0, R0, R1,LSL#12
                ADCS            R2, R2, R2
loc_111E6C:
                RSBS            R3, R1, R0,LSR#11
                SUBCS           R0, R0, R1,LSL#11
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#10
                SUBCS           R0, R0, R1,LSL#10
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#9
                SUBCS           R0, R0, R1,LSL#9
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#8
                SUBCS           R0, R0, R1,LSL#8
                ADCS            R2, R2, R2
                BCS             loc_111E38
loc_111EA0:
                RSBS            R3, R1, R0,LSR#7
                SUBCS           R0, R0, R1,LSL#7
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#6
                SUBCS           R0, R0, R1,LSL#6
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#5
                SUBCS           R0, R0, R1,LSL#5
                ADCS            R2, R2, R2
loc_111EC4:
                RSBS            R3, R1, R0,LSR#4
                SUBCS           R0, R0, R1,LSL#4
                ADCS            R2, R2, R2
loc_111ED0:
                RSBS            R3, R1, R0,LSR#3
                SUBCS           R0, R0, R1,LSL#3
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#2
                SUBCS           R0, R0, R1,LSL#2
                ADCS            R2, R2, R2
loc_111EE8:
                RSBS            R3, R1, R0,LSR#1
                SUBCS           R0, R0, R1,LSL#1
                ADCS            R2, R2, R2
                SUBS            R1, R0, R1
                MOVCC           R1, R0
                ADC             R0, R2, R2
                BX              LR
loc_111F04:
                SUBS            R1, R0, R1
                BCC             loc_111F14
                MOVS            R0, #1
                BX              LR
loc_111F14:
                MOV             R1, R0
                MOVS            R0, #0
                BX              LR
loc_111F20:
                ANDS            R2, R1, #0x80000000
                RSBMI           R1, R1, #0
                EORS            R12, R2, R0,ASR#32
                RSBCS           R0, R0, #0
                RSBS            R3, R1, R0,LSR#4
                BCC             loc_111FB0
                RSBS            R3, R1, R0,LSR#8
                BCC             loc_111F80
                MOV             R1, R1,LSL#6
                RSBS            R3, R1, R0,LSR#8
                ORR             R2, R2, #0xFC000000
                BCC             loc_111F80
                MOV             R1, R1,LSL#6
                RSBS            R3, R1, R0,LSR#8
                ORR             R2, R2, #0x3F00000
                BCC             loc_111F80
                MOV             R1, R1,LSL#6
                RSBS            R3, R1, R0,LSR#8
                ORR             R2, R2, #0xFC000
                MOVCS           R1, R1,LSL#6
                ORRCS           R2, R2, #0x3F00
                RSBS            R3, R1, #0
                BCS             loc_111FF4
loc_111F7C:
                MOVCS           R1, R1,LSR#6
loc_111F80:
                RSBS            R3, R1, R0,LSR#7
                SUBCS           R0, R0, R1,LSL#7
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#6
                SUBCS           R0, R0, R1,LSL#6
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#5
                SUBCS           R0, R0, R1,LSL#5
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#4
                SUBCS           R0, R0, R1,LSL#4
                ADCS            R2, R2, R2
loc_111FB0:
                RSBS            R3, R1, R0,LSR#3
                SUBCS           R0, R0, R1,LSL#3
                ADCS            R2, R2, R2
                RSBS            R3, R1, R0,LSR#2
                SUBCS           R0, R0, R1,LSL#2
                ADCS            R2, R2, R2
                BCS             loc_111F7C
                RSBS            R3, R1, R0,LSR#1
                SUBCS           R0, R0, R1,LSL#1
                ADCS            R2, R2, R2
                SUBS            R1, R0, R1
                MOVCC           R1, R0
                ADC             R0, R2, R2
                MOVS            R12, R12,ASR#31
                RSBMI           R0, R0, #0
                RSBCS           R1, R1, #0
                BX              LR
loc_111FF4:
                MOVS            R12, R12,ASR#31
                RSBMI           R0, R0, #0
loc_111FFC:
                STMFD           SP!, {R0,LR}
                MOVS            R0, #0
                //BL              sub_10A674
                LDMFD           SP!, {R1,PC}