
#ifndef MACROS_H
#define MACROS_H

#define BASE(ty, addr) (* (ty volatile*) addr)
#define REG(ty, base, offset) BASE(ty, (base + offset))

#define BIT(i) (1 << (i))
#define SET_BIT(reg, n) reg |= BIT(n)

#define MASK(pos, width) ((1 << (width-1)) << pos)
#define SET_FIELD(reg, pos, width, val) \
    reg = (reg & ~MASK(pos, width)) | (val << pos)


/* assembly macros */
#define nop() asm volatile ("nop")
#define pause() asm volatile ("wfe")


#endif
