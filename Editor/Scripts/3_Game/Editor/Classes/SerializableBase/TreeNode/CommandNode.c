// I think its wise... it was
class CommandNode: TreeNode
{
	void CommandNode(string uuid, string display_name, Symbols icon, TreeNodeInteract key_type = TreeNodeInteract.NONE)
	{
		m_TreeNodeInteract = key_type;
	}
	
	bool Update(float dt, Raycast raycast)
	{		
		// Never null for some reason?
		UAInput input = GetUApi().GetInputByName(m_UUID);
		if (input.ID() == -1) {
			return true;
		}
						
		switch (GetInteractType()) {
			case TreeNodeInteract.PRESS: {
				if (input.LocalPress()) {
					SetState(TreeNodeState.ACTIVE);
					return false;
				}
				
				break;
			}
			
			case TreeNodeInteract.DOUBLE: {
				if (input.LocalDoubleClick()) {
					SetState(TreeNodeState.ACTIVE);
					return false;
				}
				
				break;
			}
			
			case TreeNodeInteract.HOLD: {
				if (input.LocalHoldBegin()) {
					SetState(TreeNodeState.ACTIVE);
					return false;
				}
				
				if (input.LocalRelease()) {
					SetState(TreeNodeState.ACTIVE);
					return false;
				}
				
				break;
			}
			
			case TreeNodeInteract.TOGGLE: {
				if (input.LocalPress()) {
					ToggleState();
					return false;
				}
				
				break;
			}
		}
		
		return true;
	}
				
	override bool SetState(TreeNodeState state)
	{
		if (!super.SetState(state)) {
			return false;
		}
						
		if (state) {
			array<string> xor_selections = GetXorSelections();
			foreach (string xor: xor_selections) {
				TreeNode xor_node = m_Parent[xor];
				if (!xor_node) {
					Error(string.Format("[%1] couldnt find node to xor %2", m_UUID, xor));
					continue;
				}
				
				if (state ^ xor_node.GetState()) {
					xor_node.SetState(TreeNodeState.EMPTY);
				}
			}
		}
		
		return true;
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
		
	array<string> GetXorSelections()
	{
		return {};
	}
}