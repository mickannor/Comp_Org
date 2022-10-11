################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Fall 2021
################################################################################
## Data segment
################################################################################
.data
  newline:  .asciiz "\n"
  tab:      .asciiz "\t"

################################################################################
## Text segment
################################################################################
.text
  .align 2          # instructions must be on word boundaries
  .globl main       # main is a global label
  .globl mm_read    # read in NxM matrix
  .globl mm_alloc   # allocate space for NxM matrix
  .globl mm_print   # print out formatted NxM matrix
  .globl mm_mult    # multiple two non-square matrices

################################################################################
# Main segment
################################################################################
main:
  # save return address on the stack
  sub $sp, $sp, 4
  sw  $ra, 0($sp)

  # Read in a matrix 1 of some NxM size
  # Return in $v0 - N
  # Return in $v1 - M
  # Return in 0($sp) - base address of matrix
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 1 values
  move  $s0, $v0
  move  $s1, $v1
  lw    $s2, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 1
  move  $a0, $s0
  move  $a1, $s1
  move  $a2, $s2
  jal mm_print

  # Read in matrix 2 of some NxM size
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 2 values
  move  $s3, $v0
  move  $s4, $v1
  lw    $s5, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 2
  move  $a0, $s3
  move  $a1, $s4
  move  $a2, $s5
  jal mm_print

  # Setup arguments and call mm_mult
  # Note: 5 arguments, so we need to use stack
  # Note: num cols matrix 1 == num rows matrix 2
  move  $a0, $s0    # num rows matrix1
  move  $a1, $s1    # num cols matrix1
  move  $a2, $s2    # address of matrix 1
  move  $a3, $s4    # num cols matrix2
  sub   $sp, $sp, 4
  sw    $s5, 0($sp) # address of matrix 2
  jal mm_mult
  add   $sp, $sp, 4

  # print the result
  move $a0, $s0
  move $a1, $s4
  move $a2, $v0
  jal mm_print

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# mm_read: Read in a NxM matrix from standard input
# Note: You will be calling mm_alloc from this function, so you'll need to save
#       $ra on the stack. Also, you'll be returning 3 values back to main, one
#       value (the matrix base address), will need to be saved on the stack.
#       Matrix format is:
#         First line:  N = num rows
#         Second line: M = num cols
#         Subsequent lines contain one value each, filling each row in turn
#         (same format as hw01, except N,M on separate lines)
# Input:
#   None
# Return:
#   v0 = N
#   v1 = M
#   0($sp) = base address of matrix
################################################################################
mm_read:
  # save return address and any saved registers on the stack, if necessary
  sub $sp $sp 4
  sw $ra, 0($sp)

  # get N
  li    $v0 5
  syscall
  move $a0 $v0

  # get M
  li    $v0 5
  syscall
  move $a1 $v0

  # Setup up arguments and call mm_alloc - v0 is returned as base address

  jal mm_alloc

  # do nested loops to read in values
  mul $t0 $s5 $s6
  move $t1 $v0
  li $t2 0

  loop1: blt $t2 $t0 L1

  L1: sll $t3 $t2 2
      add $t4 $t1 $t3
      li    $v0 5
      syscall
      sw $v0, 0($t4)
      addi $t2 $t2 1
      beq $t2 $t0 exit
      j loop1

  # setup up return values



  # Note: third return value goes on the stack *after* restoration below


  # restore stack, ra, and any saved registers, if necessary
  exit: move $v0 $s5
        move $v1 $s6
        lw $ra, 0($sp)
        add $sp $sp 4
        sw $t1 0($sp)
  # return to main
        jr  $ra

################################################################################
# mm_alloc: Allocate an NxM matrix
# Note: You can just allocate a single array of length N*M.
# Input:
#   a0 = N
#   a1 = M
# Return:
#   v0 = base address of allocated space
################################################################################
mm_alloc:
  # save return address and any saved registers on the stack, if necessary

  # Allocate sufficient space
  mul $t0 $a0 $a1
  move $s5 $a0
  move $s6 $a1
  sll $t0 $t0 2
  move $a0 $t0
  li $v0 9
  syscall

  # set return value

  # restore stack, ra, and any saved registers, if necessary

  # return to main
  jr  $ra

################################################################################
# mm_print: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
# Return:
#   None
################################################################################
mm_print:
  # save return address and any saved registers on the stack, if necessary
  sub $sp $sp 4
  sw $ra, 0($sp)

  # do nested loops to print out values
  li $t0 0 # i = 0
  li $t1 0 # j = 0
  mul $t2 $a0 $a1 #t2 = n * m

  move $s0 $a0
  move $s1 $a1

  loop2: blt $t0 $s1 D1 # i < m


  D1:   sll $t3 $t1 2  # t3 = j * 4
        add $t4 $a2 $t3  #t4 = address of matrix[j]
        lw $t5 0($t4) #t5 = matrix[j]
        li $v0 1
        move $a0 $t5 #maybe try lw $a0, 0($a2)
        syscall
        addi $t1 $t1 1 # j += 1

        li $v0 4
        la $a0 tab
        syscall

        beq $t1 $t2 fin # if j == total
        addi $t0 $t0 1 # i += 1
        beq $t0 $s1 D2 # if i == m

        j loop2

  D2: li $t0 0
      li $v0 4
      la $a0 newline
      syscall
      j loop2

  fin: li $v0 4
       la $a0 newline
       syscall
       lw $ra, 0($sp)
       add $sp $sp 4
       jr  $ra
  # restore stack, ra, and any saved registers, if necessary

  # return to main

################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = N for matrix 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2
#   0($sp) = base address for matrix 2
# Return:
#   v0 = base address for result matrix
################################################################################
mm_mult:
  # save return address and any saved registers on the stack, if necessary
  sub $sp $sp 4
  sw $ra, 0($sp)

  lw $s7 4($sp)
  lw $s8 4($sp)

  # set up and call mm_alloc for result matrix
  move $s0 $a0
  move $s1 $a1
  move $s2 $a2
  move $s3 $a3

  move $a0 $s1
  move $a1 $s3
  jal mm_alloc
  mul $s4 $a1 $s3

  sub $sp $sp 4
  sw $sp 0($v0)

  # three nested loops for matrix-matrix multiplication
  li $s5 0
  li $s6 4
  li $t9 0 # will keep track of the number of values in v0
  li $t0 0 # will keep track of the number  of time a row from matrix1 has been used for multiplication
  li $t1 0 # will keep track of matrix1[x]
  li $t2 0 # will keep track of matrix2[y]

  loop3: blt $t0 $s1 A1 # makes sure t0 is less than matrix1 n

  A1: div $t1 $s0 # x % matrix1 m
      mfhi $t3
      beq $t1 $zero A2
      bgt $t3 $zero A2 # t3 > 0
      beq $t3 $zero A3 # t3 == 0
      j loop3

  A2: sll $t4 $t1 2
      add $t4 $t4 $s2 # address of matrix1[x]
      sll $t5 $t2 2
      add $t5 $t5 $s7 # address of matrix2[y]
      lw $t6 0($t4) #store value in t6
      lw $t7 0($t5) #store value in t7
      mul $t8 $t6 $t7 # multiply them
      add $s5 $s5 $t8

      addi $t1 $t1 1
      add $t2 $t2 $s3
      j loop3

  A3: li $v0 1
      move $a0 $s5
      syscall
      mul $t8 $t9 $s6
      add $t8 $sp $t8
      sw $s5, 0($t8) #store values
      li $s5 0
      li $t1 0
      li $t2 0
      addi $s7 $s7 4 # move to the next column for matrix2
      addi $t0 $t0 1
      addi $t9 $t9 1
      beq $t9 $s4 end # t9 equals the number of values that should be in v0
      beq $t0 $s3 A4 # t0 equals matrix2 m
      j loop3

  A4: mul $a2 $s0 $s6
      add $s2 $s2 $a2 # move to the next row for matrix1
      move $s7 $s8
      li $t0 0
      li $t1 0
      li $t2 0
      j loop3

  # set return value

  # restore stack, ra, and any saved registers, if necessary

  # return to main

  end: lw $v0 0($sp)
       add $sp $sp 4
       lw $ra 0($sp)
       add $sp $sp 4
       move $s0 $a0
       move $s4 $a3
       jr  $ra
