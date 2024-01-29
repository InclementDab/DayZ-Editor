// I think its wise... it was
class CommandNode: TreeNode
{
	protected UAInput m_UAInput;
	protected ShortcutKeyType m_ShortcutKeyType;
	
	void CommandNode(string uuid, string display_name, Symbols icon, ShortcutKeyType key_type = ShortcutKeyType.NONE)
	{
		m_ShortcutKeyType = key_type;
	}
	
	bool Update(float dt, Raycast raycast)
	{
		if ((GetFocus() && GetFocus().IsInherited(EditBoxWidget)) || !CanSelect()) {
			return true;
		}
		
		if (!m_UAInput) {
			m_UAInput = GetUApi().GetInputByName(m_UUID);
			if (m_UAInput.ID() == -1) {
				//Error("No input validated for " + Type().ToString());
				return true;
			}
		}
				
		switch (GetShortcutType()) {
			case ShortcutKeyType.PRESS: {
				if (m_UAInput.LocalPress()) {
					GetEditor().Select(this);
					return false;
				}
				
				break;
			}
			
			case ShortcutKeyType.DOUBLE: {
				if (m_UAInput.LocalDoubleClick()) {
					GetEditor().ToggleSelect(this);
					return false;
				}
				
				break;
			}
			
			case ShortcutKeyType.HOLD: {
				if (m_UAInput.LocalHoldBegin()) {
					GetEditor().Select(this);
					return false;
				}
				
				if (m_UAInput.LocalRelease()) {
					GetEditor().Deselect(this);
					return false;
				}
				
				break;
			}
			
			case ShortcutKeyType.TOGGLE: {
				if (m_UAInput.LocalPress()) {
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
		
		Editor editor = GetEditor();
		
		switch (m_UUID) {	
			case "HudToggle": {
				if (GetEditor().GetHud()) {
					GetEditor().GetHud().Show(state);
				}
				
				break;
			}
					
			case "CursorToggle": {
				GetGame().GetUIManager().ShowCursor(state);
				if (editor.GetHud()) {
					editor.GetHud().ClearCursor();
				}
				
				break;
			}
			
			case "Undo": {
				GetEditor().Undo();
				break;
			}
			
			case "Redo": {
				GetEditor().Redo();
				break;
			}
									
			case "Delete": {
				array<TreeNode> selected_nodes = GetEditor().GetSelectedNodes();
				foreach (TreeNode node: selected_nodes) {					
					editor.InsertHistory("Undo Delete", Symbols.CLOCK_ROTATE_LEFT, null, node.CreateCopy());	
					node.GetParent().Children.Remove(node.GetUUID());				
					delete node;
					editor.GetObjects().Synchronize();
					editor.PlaySound(EditorSounds.HIGHLIGHT);
				}
				
				break;
			}
		}
		
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