# Assignment 1 - done
- Character able to jump via input action.

- “Explosive Barrel” from class re-implemented in C++.
	- Barrel mesh can still be assigned via Blueprint class. (We always use Blueprints to assign Content/Assets even if all code/logic is C++)
	- All other properties and components must be configured via C++.
	- When hit by a projectile, applies force to nearby physics simulating Actors.

	- Solution: Using ForceComp, and Post Initialize Component



# Lecture 4 - done
- Create an interaction component
- Implement the primary interact within the comp, using line trace
- Adding the interaction comp to the character
- Implement the primary interaction function in character
- Draw Debug line for tracing

- Implement a sweepMultiByObjectType
	- Use FQuat
	- Redefine collision shape
	- Draw the debug sphere
	- Use a for loop to check each

- Play animation in primary attack
- Utilize timer



# Lecture 5 - done
- making a lever to interact
	- Lower the level when interact
	- utilize BP interface

- Remotely open chest
- Remotely explode barrel // leave it to the Explosive barrel assignment?
- Polish the chest with gold and particle in BP

- Polish the magic projectile
	- Add Hit FX emitter
	- On Component Hit and destroy
	- Ignore the player



# Lecture 6 done
- UE_Log for debugging
- Draw Debug String

- Polish Camera
- Adding Crosshair



# Assignment 2
- Fix projectile launch location - done
- Black hole attack - done
- Dash ability
