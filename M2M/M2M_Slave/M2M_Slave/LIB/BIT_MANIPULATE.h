#ifndef BIT_MANIPULATE_H_
#define BIT_MANIPULATE_H_

/* THESE MACROS ARE USED TO SET, CLEAR, TOGGLE OR ASSIGN A BIT IN A REGISTER */
/* A macro used to set a specific bit in a specific register */
#define SET_BIT(REG, BIT_NO)                                (REG |= (1 << BIT_NO))
/* A macro used to clear a specific bit in a specific register */
#define CLEAR_BIT(REG, BIT_NO)                              (REG &= ~(1 << BIT_NO))
/* A macro used to toggle a specific bit in a specific register */
#define TOGGLE_BIT(REG, BIT_NO)                             (REG ^= (1 << BIT_NO))
/* A macro used to assign a specific boolean value (0 or 1) into a specific bit of a register */
#define ASSIGN_BIT(REG, BIT_NO, VALUE)                      (REG = ((REG & ~(1 << BIT_NO)) | (VALUE << BIT_NO)))


/* THIS MACRO ARE USED TO SET, CLEAR, TOGGLE OR ASSIGN A SPECIFIC VALUE INTO A SPECIFIC SECTION OF A REGISTER */
/* A macro used to set a specific value in a specific register */
#define SET_BITS(REG, START_BIT_NO, MASK)                   (REG |= (MASK << START_BIT_NO))
/* A macro used to clear a specific value in a specific register */
#define CLEAR_BITS(REG, START_BIT_NO, MASK)                 (REG &= ~(MASK << START_BIT_NO))
/* A macro used to toggle a specific value in a specific register */
#define TOGGLE_BITS(REG, START_BIT_NO, MASK)                (REG ^= (MASK << START_BIT_NO))
/* A macro used to assign a specific value into a specific section of a register */
#define ASSIGN_BITS(REG, START_BIT_NO, VALUE, MASK)         (REG = ((REG & ~(MASK << START_BIT_NO)) | (VALUE << START_BIT_NO)))


/* THIS MACRO IS USED TO GET THE VALUE OF A BIT IN A REGISTER */
#define GET_BIT(REG, BIT_NO)                                ((REG & (1 << BIT_NO)) >> BIT_NO)

/* THIS MACRO IS USED TO GET THE VALUE OF SOME BITS IN A REGISTER */
#define GET_BITS(REG, START_BIT_NO, MASK)                   ((REG & (MASK << START_BIT_NO)) >> START_BIT_NO)


/* THESE MACROS ARE USED TO CHECK IF A BIT IS SET OR CLEARED */
/* A macro used to check if a specific bit is set in a specific register */
#define IS_BIT_SET(REG, BIT_NO)                             (((REG & (1 << BIT_NO)) >> BIT_NO) == 1)
/* A macro used to check if a specific bit is cleared in a specific register */
#define IS_BIT_CLEAR(REG, BIT_NO)                           (((REG & (1 << BIT_NO)) >> BIT_NO) == 0)


/* THESE MACROS ARE USED TO SHIFT LEFT, SHIFT RIGHT, ROTATE LEFT OR ROTATE RIGHT THE VALUE IN A REGISTER */
/* A macro used to shift the value of a specific register to the right */
#define SHIFT_RIGHT(REG, ROT_NO)                            (REG = (REG >> ROT_NO))
/* A macro used to rotate the value of a specific register to the left */
#define SHIFT_LEFT(REG, ROT_NO)                             (REG = (REG << ROT_NO))
/* A macro used to rotate the value of a specific register to the right */
#define ROTATE_RIGHT(REG, ROT_NO)                           (REG = ((REG >> ROT_NO) | (REG << ((sizeof(REG) * 8) - ROT_NO))))
/* A macro used to rotate the value of a specific register to the left */
#define ROTATE_LEFT(REG, ROT_NO)                            (REG = ((REG << ROT_NO) | (REG >> ((sizeof(REG) * 8) - ROT_NO))))

#endif /* BIT_MANIPULATE_H_ */
