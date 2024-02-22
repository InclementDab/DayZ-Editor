enum SandboxNodeInteract
{
	NONE, // darn :(
	
	// The button was pressed, you will have to manually unpress it
	ONCE,
	
	// The button was pressed, it will be cleaned up when you press off of it
	PRESS,
	
	// Button will have to be double clicked
	DOUBLE, // woah
	
	// button MUST be held
	HOLD, // EEEEEK like a poop?
	
	// Button must be in toggle mode lol
	TOGGLE // ying and yang
};