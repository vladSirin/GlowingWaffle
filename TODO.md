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



# Assignment 2 - done
- Fix projectile launch location - done
- Black hole attack - done
- Dash ability - done
- Putting explode into the base class of projectile - done
- make projectile_base - done
- use Spawn Projectile class to spawn different projectiles - different approach
- not teleport twice when hit ground. - done



# Lecture 7 - done
- add attributes component - done
    - Health/HealthMax/Stamina/Strength - done
    - Apply health change function
- add attributes to Character - done
- Handle attribute health with projectile - done
- Make Projectile overlap pawn and dynamic - done
- consider it, where should damage handled? - Could be better if handled in damage interface?
- UMG with health bar - done
- Adding a auto fire actor to fire projectiles - done
- Health handle event - done
- Update Healthbar with event - done
- Animate UMG Widgets - done
- Adding damage from explosive barrels - done



# Lecture 8 - done
- Adding Basic material master and instance with SineWave/Time based - done
- Hit Flash Material and Drive with C++ with Dummy Target - done
- Dissolve material simple - done
- Create a Material Function - done
- Using Materials in UMG - done



# Lecture 9 - done
- Creating sound cues
- Animation blueprint and player death, blend by bool
- Projected 3D UI, damage numbers



# Assignment 3
- Magic Projectile [Audio, and Visual Polish] - done
- Player Character [React on Damage Received] - done
- AttributeComponent [HealthMax Attribute] - done
- UI [Animation]
- Goal: ‘Health Potion’ power-up Actor in level to heal ‘Pawn’. - done
- Magic Projectile - done
- Class Design: Consider future power-up/pick-up actors that share similar pickup/respawn logic but apply different reactions. - done



# Lecture 10 - done
- Bot Behavior and Move to Player
- Custom Check Range service for BT
- Check LOS



# Lecture 11 - done

# Lecture 12 - done
- EQS to find spawn points
- Game mode with custom AI spawn point

# Assignment 4 - done
- New AI Behavior: Flee/hide when ‘low health’ and heal itself once arrived. 
  - Service: Check periodically if “low health”. - done
  - EQS: Find hidden position away from player, close to AI itself. - done
  - BT Task: “Heal” back to max hitpoints. - done
  - This behavior can only run once every 60 seconds. -done
  - After healing, the AI will continue to fight as before. - done

# Lecture 13 - done

# Lecture 14 - done

# Lecture 15 - done

# Assignment 5
- “Credits-system” to gain and spend (Design for ease-of-use in C++ & Blueprint) // Credits must persist even if player dies (store in ‘PlayerState’ derived-class)
	- Killing Minions grants credits via SGameModeBase. - done
	- Health Potion: costs credits to interact. - done
	- Coin Pick-up: grants credits. // Must use shared base-class with Health Potion. Shares same behavior of activate, hide, respawn.
	- Credits-widget hooked up to show current credits.
	- Bonus: Randomly Spawn Coin and Health potions throughout a simple level at Game start (EQS)