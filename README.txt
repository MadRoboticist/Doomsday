Doomsday v2.0 - Written by [MMM] Heretical Hermit

DISCLAIMER - This program wasn't written with public distribution in mind. Therefore:
			A) It's not terribly user friendly.
			B) The code is not terribly readable.
			C) The code is full of bugs.

compile: gcc Doomsday.c -o Doomsday

usage: ./Doomsday <troop_base.txt> <troop_bonus_base.txt>

FILES:

troop_base.txt 
	contains standard information about troops. 
	This can be edited fairly easily to add mercenaries which haven't been included.
	Currently the file only includes t9 mercenaries. t7 and lower mercenaries must be added to be used.
	The file does not include siege or spies. I generally do not recommend sending siege or spies to DD, but these can be added if you want.
	
troop_bonus_base.txt (I created variations for two different sets of armor - orange and purple)
	This file must be edited for your troop health and strength bonuses for whatever armor you'll use.
	1) Put the armor on a non-bonus captain like Doria, Dustan, Carter, etc.
	2) make sure no other captains are hired, and that your hero is not wearing any equipment
	3) make sure your personal strength bonus and VP bonus are on, and possibly your clan strength bonus.
	4) retrieve the bonuses from your barracks or from your courtyard and edit the text file to reflect those bonuses.
	
USING THE PROGRAM:

0) Run the program in a unix command line, using the files you want to use. 

1) enter 1 to select your captain. Choices are captains with troop bonuses, or other.
	you must enter your captain's available leadership, dominance, and authority
	
2) enter 2 to select your troops:
	I) 		enter the lowest level of troops you plan to send (guards, specs, monsters)
	II)		enter the highest level of troops you have unlocked (guards, specs, monsters)
	III)	enter whether or not you have unlocked all of them (guards or specs)
	IV)		if you haven't unlocked all of your highest level you'll be asked which ones you unlocked. (guards or specs)
	V)		enter the tier of mercs you will be using. Currently only supports sending one tier of mercs 
	VI)		you'll be asked if you have each merc type in the troop_base.txt file for that tier
	
3) enter 3 to set your troop quantities
	I)		enter 1 to display the available troop types and quantities
	II)		enter 2 to calculate the stacks to send 
		A)	enter how many stacks on the doomsday monster to set up for: (1-4) [I generally set mine up for 3 stacks]
		B)  enter a base quantity of your strongest troops to start from
		C)	enter whether or not to sort the troops for strength and health both, and for how many turns
			I generally only sort up to 3 turns by strength. The best VP results seem to come with either just one turn or none.
		D)	the troops are ranked by strength, and then sorted by health so that the stack with the lowest total health 
			belongs to the troop with the highest strength per leadership point against the DD monster for the number of DD stacks you've selected.
			There are two separate troop quantities displayed:
			i) The left column is the initial quantities calculated based on the value you entered in step b)
			ii) The right column is adjusted for your captain's leadership.
				First a ratio is applied to the left column, then the numbers are adjusted if needed so that the strength/health order is still the same.
	III)	You can set the quantities of troops by hand using option 3.
			A) The program will ask which tier, and whether they are guards, monsters, or mercs.
			B) You'll be asked how many of each for the tier/type you've selected.
	IV) 	You can reset all of the quantities to zero.
			There are some bugs that haven't been worked out when changing the types of troops you are sending.
			this option helps clear all "included" flags and troop quantities to zero.
			
4) enter 4 to change default values - These values are set up for my troops by default
	I) VP ratio: this is the ratio of VP you get for how much health you've killed on the monster. You have to calculate this ratio.
	II) Doublestrike bonus: This is for simulation. It only supports a doublestrike value for your whole army
	III) Revival Discount: This is based on the level of your temple.
	
5) enter 5 to show what's selected. Here you can view your captain, troop quantities, default values, and estimated costs.

6) enter 6 to simulate your captain/troop selections
	I) 		Enter the number of monsters stacks (1-4)
	II) 	Enter whether or not to use the "adjusted" quantities. These are the right column from before, or the values you entered manually
	III) 	Enter whether the player gets first strike or not
	IV)		The simulation will run. You can scroll up to see the order of strikes/death 
	V)		You can simulate repeatedly for the same number of stacks, and can select whether or not you get first strike each time. 
			Average costs are calculated for however many runs you've simulated.
	