mkdir results

bit_oper 0 eval zero1 results/not_zero
bit_oper 0 eval one1 results/not_one

bit_oper 1 eval zero1 zero2 results/or_zero_zero
bit_oper 1 eval zero1 one1 results/or_zero_one
bit_oper 1 eval one2 zero2 results/or_one_zero
bit_oper 1 eval one1 one2 results/or_one_one

bit_oper 2 eval zero1 zero2 results/and_zero_zero
bit_oper 2 eval zero1 one1 results/and_zero_one
bit_oper 2 eval one2 zero2 results/and_one_zero
bit_oper 2 eval one1 one2 results/and_one_one

bit_oper 3 eval zero1 zero2 results/nor_zero_zero
bit_oper 3 eval zero1 one1 results/nor_zero_one
bit_oper 3 eval one2 zero2 results/nor_one_zero
bit_oper 3 eval one1 one2 results/nor_one_one

bit_oper 4 eval zero1 zero2 results/nand_zero_zero
bit_oper 4 eval zero1 one1 results/nand_zero_one
bit_oper 4 eval one2 zero2 results/nand_one_zero
bit_oper 4 eval one1 one2 results/nand_one_one

bit_oper 5 eval zero1 zero2 results/xor_zero_zero
bit_oper 5 eval zero1 one1 results/xor_zero_one
bit_oper 5 eval one2 zero2 results/xor_one_zero
bit_oper 5 eval one1 one2 results/xor_one_one
