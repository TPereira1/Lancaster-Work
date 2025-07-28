.data 

# This is the maim menu that getsa outputted to the user at the start
menu: .asciiz "Options:\tColours:\n1 - cls \t1 - red\n2 - stave\t2 - orange\n3 - note\t3 - yellow\n4 - exit\t4 - green\n\t\t5 - blue\n"
# Prompt to select an option from the main menu outputted after the main menu displayed
prompt: .asciiz "Select an option\n"
# Second prompt to be displayed after option 1/2/3 is chosen by the user
prompt2:.asciiz "Select colour/row/note\n"
# Invalid input prompt displayed to us4r if they enter an input out of bounds
invalidInput:.asciiz "Invalid input entered.\n"
# Outputted to user if the row entered by the user is out of bounds to start drawing the stave
StaveError:.asciiz "Row entered will cause the stave to be drawn partly/entirely off of the bitmap display, enter a different row.\n"
# Outputted if user tries to draw multiple staves on the same display without clearing
AlreadyDrawn:.asciiz "One stave has already been drawn on this display. Cls to draw another one.\n"

.text
	# This procedure displays the main menu and then takes in the option the user chooses branching to correct procedure for each option.
start:
	lui $s0, 0x1004			# Sets the $s0 to point to the first pixel of heap bitmap display.
	li $v0, 4 
	la $a0, menu
	syscall				# Prints the menu to the console.
	la $a0, prompt
	syscall 			# Prints prompt to select option to the console.
	li $v0, 5
	syscall 			# Takes in an integer as an input from console.
	beq $v0, 1, cls 		# Branches to cls procedure if option 1 select.
	beq $v0, 2, stave		# Branches to stave procedure if option 2 select.
	beq $v0, 3, note 		# Branches to note procedure if option 3 select.
	beq $v0, 4, exit 		# Branches to exit procedure if option 4 select.

	li $v0, 4 
	la $a0, invalidInput
	syscall 			# Displays an error message if an integer out of range is entered.
	j start 			# Jumps back to start of the program, displaying menu.

	# This procedure paints the bitmap display a single colour (default MIPS display use only).
cls:
	li $v0, 4
	la $a0, prompt2
	syscall 			# Prints out the second prompt option to display for user to select colour.
	li $v0, 5
	syscall 			# Takes in an integer input for which colour to paint the bitmap with.

	lui $s1,0x100c 			# An end pointer to the last memory location of the heap default bitmap display 
	beq $v0,1,red 			# Branches to red if option 1 is selected.
	beq $v0,2,orange 		# Branches to orange if option 2 is selected.
	beq $v0,3,yellow 		# Branches to yellow if option 3 is selected.
	beq $v0,4,green 		# Branches to green if option 4 is selected.
	beq $v0,5,blue 			# Branches to blue if option 5 is selected.

	li $v0, 4 
	la $a0, invalidInput
	syscall 			# Displays an error message if an integer out of range is entered.
	j start 			# Jumps back to start of the program, displaying menu.

	# Sets the colour red to $a1 and jumps to background procedure.
red:
	addi $a1, $zero, 0x00ff0000
	j background
	
	# Sets the colour orange to $a1 and jumps to background procedure.
orange:
	addi $a1, $zero, 0x00ffa500
	j background
	
	# Sets the colour yellow to $a1 and jumps to background procedure.
yellow:
	addi $a1, $zero, 0x00ffea00
	j background
	
	# Sets the colour green to $a1 and jumps to background procedure.
green:
	addi $a1, $zero, 0x00ff00
	j background
	
	# Sets the colour blue to $a1 and jumps to background procedure.
blue:
	addi $a1, $zero, 0x00ff
	j background

	# Paints the colour stored in register $a1 to the entire default bitmap display for the heap. Uses $s0 as current pixel pointer in which gets painted to and $s1 as endpointer to stop painting at when $s0 reaches it.
background:
	sw $a1, 0($s0) 			# Sets the colour in $a1 to current memory location in £s0
	addi $s0, $s0, 4		# Jumps to the next pixel 
	bne $s0, $s1, background 	# Keeps looping back to start of background procedure until the memory location of the current pixel is equal to the last pixel of the bitmap display
	addi $t1, $zero, 0 		# Sets $t1 to zero, this register later used when drawing stave to prevent multiple staves being drawn at a time on the same background
	addi $t5, $zero, 0 		# Sets $t5 to zero, used later to store the position of the first line of the stave.
	j start 			# Jump back to the main menu.

	# Procedure draws a stave on the bitmap display starting on the row entered by the user
stave:
	li $v0, 4
	la $a0, prompt2
	syscall 			# Displays the prompt to the user to select the row to draw the stave from
	li $v0, 5
	syscall 			# Takes in integer input for the row to start drawing the stave at.

	bltz $v0, StaveDrawingError 	# Prevents draving the stave being drawn above the bitmap display
	bgt $v0, 211, StaveDrawingError # Prevents the stave being drawn under the bitmap display. Jumps back to the start of stave procedure.
	addi $t0, $zero, 5 		# Holds max number of stave lines to be drawn
	addi $a1, $zero, 0x0000 	# Set colour of the stave to black
	addi $t2, $zero, 2048 		# Holds number of memory spaces a line of display holds
	mul $s2, $v0, $t2 		# Calculations to get the number of memory locations to move start pointer of the bitmap display to appropriate pixel

	add $s0, $s0, $s2 		# Moves start pointer to correct memory location to start drawing the stave
	bge $t1, $t0, Drawn 		# Used to exit back to menu once all 5 lines of stave are drawn 
	add $t5, $zero, $s0 		# Holds memory location of the first pixel of the first line of the stave in $t5 
	add $s1, $s0, $t2 		# Sets an end pointer to the last pixel in line with the start pointer of the stave

	# Draws a full line of stave
DrawLine:
	bge $t1, $t0, start 		# Used to exit back to menu once all 5 lines of stave are drawn
	beq $s0, $s1, NextLine 		# Used to exit once one line of the stave is drawn
	sw $a1, 0($s0) 			# Set current pixel held in memory location $s0 to colour black
	addi $s0, $s0, 4 		# Move to next pixel in bitmap display
	j DrawLine 			# used loop back to DrawLine

	# Changes values $s0, $s1 and $t1 for the next line to be drawn
NextLine:
	addi $s0, $s0,20480 		# Moves start pointer 10 lines down for the next stave line
	add $s1, $s0, $t2 		# Sets end pointer to end of new stave line
	addi $t1, $t1, 1 		# Increment number of lines drawn by stave by one
	j DrawLine 			# jumps back to DrawLine procedure

	# Deals with user entering a row that will cause the stave not to be complelty drawn on the bitmap display 
StaveDrawingError:
	li $v0, 4
	la $a0, StaveError
	syscall 			# Prints out StaveError to user for entering a row out of bounds
	j stave 			# Jumps back to stave so user can re-enter the row to draw the stave at

Drawn:
	li $v0, 4
	la $a0, AlreadyDrawn
	syscall 			# Prints out the stavr has already been drawn to the user
	j start 			# Jumps back to stave so user can re-enter the row to draw the stave at

	# Prints and plays notes to display if stave is drawn.
note:
	li $v0, 4
	la $a0, prompt2
	syscall 			# Prints out prompt to user to select note to play
	li $v0, 12
	syscall 			# Takes in a char input from user

	blez $t5, start 		# Check to see if the stave is drawn, if stave not drawn returns to menu
	addi $a1, $zero, 0x000000 	# Sets colour of the note to black
	add $s0, $zero, $t5 		# Sets the start pointer to first line of the stave
	addi $t4, $zero, 0 		# Used to hold number of lines of the note printed
	addi $t0, $zero, 6 		# Used to hold the maximum nuber lines of the note to print

	beq $v0, 65, A
	beq $v0, 97, A 			# Branches to procedure A, if a or A entered
	beq $v0, 66, B
	beq $v0, 98, B 			# Branches to procedure B, if b or B entered
	beq $v0, 67, C
	beq $v0, 99, C 			# Branches to procedure C, if c or C entered
	beq $v0, 68, D
	beq $v0, 100, D 		# Branches to procedure D if d or D entered
	beq $v0, 69, E
	beq $v0, 101, E 		# Branches to procedure E, if e or E entered
	beq $v0, 70, F
	beq $v0, 102, F 		# Branches to procedure F, if f or F entered
	beq $v0, 71, G
	beq $v0, 103, G 		# Branches to procedure G, if g or G entered

	li $v0, 4 
	la $a0, invalidInput
	syscall 			# Displays an error message if the input entered is an invalid note.
	j note 				# Jumps back to start of procedure to try entering the note again

	# Sets start pointer to correct position on stave and set $a0 to correct pitch of note chosen by the user
A:
	addi $s0, $s0, 52028
	la $a0, 69
	j setEndPoint
B:
	addi $s0, $s0, 40016
	la $a0, 71
	j setEndPoint
C:
	addi $s0, $s0, 30052
	la $a0, 72
	j setEndPoint
D:
	addi $s0, $s0, 18040
	la $a0, 74
	j setEndPoint
E:
	addi $s0, $s0, 6028
	la $a0, 76
	j setEndPoint
F:
	addi $s0, $s0, 74004
	la $a0, 65
	j setEndPoint
G:
	addi $s0, $s0, 61992
	la $a0, 67

	# Sets the end pointer to the right most pixel of note in the current line to be printed 
setEndPoint:
	addi $s1, $s0, 32
	
	# Prints note to display
printNote:
	bge $t4, $t0, End		# Checks to see if note has finished to print
	beq $s0, $s1, nextRecLine	# Checks to see if the pointer has reached the end of the note line
	sw $a1, 0($s0) 			# Store the colour in $a1 to the current pixel location $s0
	addi $s0, $s0, 4 		# Moves to the next pixel
	j printNote 			# Jumps to back to printNote procedure 

	# Shifts start pointer to next line for the note to print
nextRecLine:
	addi $t4, $t4, 1 		# Increments the number of lines printed that makes up a note by 1 
	addi $s0, $s0, 2016 		# Move start pointer to next line of the note
	j setEndPoint 			# jump back to print the note line

	# Plays note once the note has been printed
End:
	li $v0, 31
	li $a1, 1000
	li $a2, 7
	li $a3, 100
	syscall 			# Plays the note at correct pitch
	j start 			# Jump back to menu

	# Used to terminate the program 
exit:
	li $v0, 10
	syscall 			# Terminates the programs
