// I think its wise... it was
class CommandNode: TreeNode
{
	protected ShortcutKeyType m_ShortcutKeyType;
	
	void CommandNode(string uuid, string display_name, Symbols icon, ShortcutKeyType key_type = ShortcutKeyType.NONE)
	{
		m_ShortcutKeyType = key_type;
	}
	
	bool Update(float dt, Raycast raycast)
	{		
		// Never null for some reason?
		UAInput input = GetUApi().GetInputByName(m_UUID);
		if (input.ID() == -1) {
			return true;
		}
						
		switch (GetShortcutType()) {
			case ShortcutKeyType.PRESS: {
				if (input.LocalPress()) {
					GetEditor().Select(this);
					return false;
				}
				
				break;
			}
			
			case ShortcutKeyType.DOUBLE: {
				if (input.LocalDoubleClick()) {
					GetEditor().ToggleSelect(this);
					return false;
				}
				
				break;
			}
			
			case ShortcutKeyType.HOLD: {
				if (input.LocalHoldBegin()) {
					GetEditor().Select(this);
					return false;
				}
				
				if (input.LocalRelease()) {
					GetEditor().Deselect(this);
					return false;
				}
				
				break;
			}
			
			case ShortcutKeyType.TOGGLE: {
				if (input.LocalPress()) {
					GetEditor().ToggleSelect(this);
					return false;
				}
				
				break;
			}
		}
		
		return true;
	}
				
	override void OnSelectionChanged(bool state)
	{
		super.OnSelectionChanged(state);
						
		if (state) {
			array<string> xor_selections = GetXorSelections();
			foreach (string xor: xor_selections) {
				TreeNode xor_node = m_Parent[xor];
				if (!xor_node) {
					Error(string.Format("[%1] couldnt find node to xor %2", m_UUID, xor));
					continue;
				}
				
				if (state ^ GetEditor().IsSelected(xor_node)) {
					GetEditor().Deselect(xor_node);
				}
			}
		}
	}
	
	string GetShortcutString() 
	{
		string result;
		UAInput inp = GetUApi().GetInputByName(m_UUID);
		for (int i = 0; i < inp.BindKeyCount(); i++) { 
			if (inp.CheckBindDevice(i, EInputDeviceType.MOUSE_AND_KEYBOARD)) {
				string button_name = GetUApi().GetButtonName(inp.GetBindKey(i));
				button_name.Replace("Left ", "");
				button_name.Replace("Right ", "R");
				
				result += button_name;
			}
			
			if (i != inp.BindKeyCount() - 1) {
				result += " + ";
			}
		}
		
		return result;
	}
		
	ShortcutKeyType GetShortcutType()
	{
		return m_ShortcutKeyType;
	}

	bool GetDefaultState()
	{
		return false;
	}
	
	array<string> GetXorSelections()
	{
		return {};
	}
}