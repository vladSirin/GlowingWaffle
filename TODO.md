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

- Goal: ‘Health Potion’ power-up Actor in level to heal ‘Pawn’.
React to Interact() // SGameplayInterface.h
Inactivate for 10 seconds upon triggered (Invisible and unable to interact with)
Re-activates on time elapsed to show mesh, and ready to be triggered again.
Ignores interacting pawn if already at maximum health.
Logic in C++ (Assets can always be assigned in Blueprint)

- Magic Projectile
Bonus: Play ‘Attached’ Particle Effect in player hand when ‘casting’ spell. (UGameplayStatics::SpawnEmitterAttached, Asset: P_Gideon_Primary_MuzzleFlash)
Bonus: ‘World’ Camera shake on Projectile Impact. (UGameplayStatics::PlayWorldCameraShake, Assets: ExampleContent/CameraShake/...)

- Class Design: Consider future power-up/pick-up actors that share similar pickup/respawn logic but apply different reactions.
Example Mesh: SM_PotionBottle (Note: has NO ‘primitive’ collision yet)