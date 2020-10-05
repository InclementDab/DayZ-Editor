class ShortcutKeys: array<KeyCode>
{
	int GetMask() 
	{
		int mask, offset;		
		foreach (int key: this) {
			mask |= key << offset;
			offset += 8;
		}
		
		return mask;
	}
}