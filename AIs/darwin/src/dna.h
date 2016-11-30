#ifndef BITMARTYR_DARWIN_DNA_H
#define BITMARTYR_DARWIN_DNA_H

struct Program {
    /* Packing of coefficients is bitwise as:
     * a b c d e
     * f g h i j
     * k l m n o ==> a<<24|b<<23|c<<22|...|w<<2|x<<1|y
     * p q r s t
     * u v w x y
     * with a 1 meaning it is included in the term, 0 it is not.
     *
     * One such polynomial for each direction, + stay and die
     */
    double *coefficients[6];
};

#endif /* BITMARTYR_DARWIN_DNA_H */
