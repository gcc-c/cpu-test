vl0 12 //length
set a v
//set p a
//vl0 5
//add p v
//set p m //skip data part
//all chars are < 256 so 1st byte is 0 (nop)
#104 //2
#101
#108
#108
#111
#32
#119
#111
#114
#108
#100
#10

cmp b a
vl0 9
add p v
spi m 3 // >=
vl0 2
add v b
mmr m c
inc b
stw c
vl0 11
sub p v
set p m

stw a
inc x
sys x
sys y
