
#ifndef MACROS_H
#define MACROS_H


#define _REG(ty, addr) (* (ty volatile*) addr)
#define REG(ty, base, offset) _REG(ty, (base + offset))

#define BIT_MASK(i) (1 << (i))
#define SET_BIT(reg, n) reg |= BIT_MASK(n)
#define CLR_BIT(reg, n) reg &= ~BIT_MASK(n)
#define TOGGLE_BIT(reg, n) reg ^= BIT_MASK(n)
#define GET_BIT(reg, n) (((reg) >> (n)) & 0x1)

#define FIELD_MASK(pos, width) (~(~(0x1) << (width - 1)) << pos)
#define SET_FIELD(reg, pos, width, val) \
    reg = (reg & ~FIELD_MASK(pos, width)) | (val << pos)


#endif

