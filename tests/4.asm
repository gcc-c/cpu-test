//calc first fibonacci numbers
stw a
inc a
stw a

vl0 10
set x v
vl0 10
set y v
vl0 7

cmp z x
spi y 4 // p = y if z < x (10)
dec p

str a
str b

stw b
stw a
add a b
stw m
inc z
set p v
