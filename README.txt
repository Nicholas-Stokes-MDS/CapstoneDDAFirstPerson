Engine Version: 5.21

Tech demo achieves:
- Implements the Hamlet DDA method for calculating the amount of health packs each crate gives, using Gaussian distribution, erf function, inventory shortfalls, and a list of damage the player has taken per encounter
- The probability of an inventory shortfall is calculated and crates give 0-3 health packs depending on this probability
- This probability is display each time the player ends an encounter
- An encounter starts when the player takes damage and ends when the player hasn't taken damage for 3 seconds

A change made to the proposal submitted research question is that originally I was going to originally have both health and ammo contribute to inventory shortfalls and to use Hamlet. However only health packs are affected by Hamlet.

Ammo uses another algorithm which attempts to give the player the minimum amount of ammo the player needs to kill all enemies while giving some leeway for missed shots. It also achieves this while still giving the player at least a few ammo even while currently having high ammo. 



Controls
- WASD: Move
- Left click: Shoot
- Right click: Reload
- E: Interact with crates (open them)
- H: Use a healing pack