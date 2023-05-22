
#ifndef MACROS
#define MACROS

#define _BASE(addr) ((unsigned volatile*) addr)
#define _ARR(ty, addr) ((ty volatile*) addr)
#define _REG(ty, addr) (* (ty volatile*) addr)
#define REG(ty, base, offset) _REG(ty, (base + offset))

#define BIT(i) (1 << (i))
#define SET_BIT(reg, n) reg |= BIT(n)
#define GET_BIT(reg, n) (((reg) >> (n)) & 0x1)
#define CLR_BIT(reg, n) reg &= ~BIT(n)
#define SET_BYTE(reg, n, v) reg = (reg & ~(0xFF << 8*(n)) | ((v & 0xFF) << 8*n))
#define GET_BYTE(reg, n) (((reg) >> (8*(n))) & 0xFF)

#define MASK(field) _MASK(field)
#define _MASK(pos, wd) (_MASK0(wd) << pos)
#define _MASK0(wd) (~((-2) << (wd-1)))
#define FIELD(field, val) _FIELD(field, val)
#define _FIELD(pos, wd, val) (((val) & _MASK0(wd)) << pos)
#define SET_FIELD(reg, pos, wd, val) \
    reg = (reg & ~_MASK(pos, wd)) | _FIELD(pos, wd, val)
#define GET_FIELD(reg, field) _GET_FIELD(reg, field)
#define _GET_FIELD(reg, pos, wd) ((reg >> pos) & _MASK0(wd))

#define __BIT(pos) pos
#define __FIELD(pos, wd) pos, wd

/* assembly macros */
#define nop() asm volatile ("nop")
#define pause() asm volatile ("wfe")

#endif