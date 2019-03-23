#ifndef BITVEC_H

#define BITVEC_H 1

typedef unsigned int bitvec_t;

extern bitvec_t *bitvec_new(size_t nbits);
extern bool bitvec_tst(bitvec_t *v, size_t pos);
extern void bitvec_set(bitvec_t *v, size_t pos, bool tf);

#endif /* BITVEC_H */
