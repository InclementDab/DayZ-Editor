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
	
	string GetString()
	{
		string shortcut;
		
		for (int i = 0; i < Count(); i++) {
			shortcut += GetKeyString(this[i]);
			if (i != (Count() - 1)) {
				shortcut += "+";
			}
		}
		
		return shortcut;
	}
	
	static string GetKeyString(int key)
	{
		// Special Cases
		switch (key) {
			
			case KeyCode.KC_LCONTROL: return "LControl";
			case KeyCode.KC_LSHIFT: return "LShift";			
			case KeyCode.KC_LMENU: return "LMenu";
			
			case KeyCode.KC_RCONTROL: return "RControl";
			case KeyCode.KC_RSHIFT: return "RShift";			
			case KeyCode.KC_RMENU: return "RMenu";
			
			case KeyCode.KC_RETURN: return "Enter";
			case KeyCode.KC_BACK: return "Backspace";
			
		}
		
		string key_string = typename.EnumToString(KeyCode, key);
		key_string.ToLower();
		key_string.Replace("kc_", "");
		
		string k1 = key_string[0];
		k1.ToUpper();
		key_string[0] = k1;
		return key_string;
	}
}