// I think its wise... it was
class CommandNode: TreeNode
{
	protected UAInput m_UAInput;
	protected ShortcutKeyType m_ShortcutKeyType;
	
	void CommandNode(string uuid, string display_name, Symbols icon, ShortcutKeyType key_type = ShortcutKeyType.NONE)
	{
		m_ShortcutKeyType = key_type;
		
#ifndef SERVER
#ifndef WORKBENCH
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(PollShortcutExecution);
#endif
#endif
	}
		
	override void OnSelectionChanged(bool state)
	{
		super.OnSelectionChanged(state);
		
		switch (m_UUID) {
			case "CursorToggle": {
				GetGame().GetUIManager().ShowCursor(state);
				if (GetEditor().GetHud()) {
					GetEditor().GetHud().ClearCursor();
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
			
			case "BoxSelect": {
				
				if (GetEditor().GetHud()) { // damn
					GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
				}
				
				if (state) {
					GetEditor().Deselect(m_Parent["CircleSelect"]);
					GetEditor().Deselect(m_Parent["LassoSelect"]);
				}
				
				break;
			}
			
			case "CircleSelect": {
				if (GetEditor().GetHud()) {
					GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
				}
				
				if (state) {
					GetEditor().Deselect(m_Parent["BoxSelect"]);
					GetEditor().Deselect(m_Parent["LassoSelect"]);
				}
					
				break;
			}
			
			case "LassoSelect": {
				if (GetEditor().GetHud()) {
					GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
				}
				
				if (state) {
					GetEditor().Deselect(m_Parent["BoxSelect"]);
					GetEditor().Deselect(m_Parent["CircleSelect"]);
				}
				break;
			}
			
			case "Delete": {
				array<TreeNode> selected_nodes = GetEditor().GetSelectedNodes();
				foreach (TreeNode node: selected_nodes) {					
					GetEditor().InsertHistory("Undo Delete", Symbols.CLOCK_ROTATE_LEFT, null, node.CreateCopy());	
					node.GetParent().Children.Remove(node.GetUUID());				
					delete node;
					GetEditor().GetObjects().Synchronize();
					GetEditor().PlaySound(EditorSounds.HIGHLIGHT);
				}
				
				break;
			}
		}
	}
	
	protected void PollShortcutExecution()
	{
		if ((GetFocus() && GetFocus().IsInherited(EditBoxWidget)) || !CanSelect()) {
			return;
		}
		
		if (!m_UAInput) {
			m_UAInput = GetUApi().GetInputByName(m_UUID);
			if (!m_UAInput) {
				//Error("No input validated for " + Type().ToString());
				return; // hoe ass bitch
			}
		}		
		
		switch (GetShortcutType()) {
			case ShortcutKeyType.PRESS: {
				if (m_UAInput.LocalPress()) {
					GetEditor().Select(this);
					break;
				}
				
				break;
			}
			
			case ShortcutKeyType.DOUBLE: {
				if (m_UAInput.LocalDoubleClick()) {
					GetEditor().ToggleSelect(this);
				}
				
				break;
			}
			
			case ShortcutKeyType.HOLD: {
				if (m_UAInput.LocalHoldBegin()) {
					GetEditor().Select(this);
				}
				
				if (m_UAInput.LocalRelease()) {
					GetEditor().Deselect(this);
				}
				
				break;
			}
			
			case ShortcutKeyType.TOGGLE: {
				if (m_UAInput.LocalPress()) {
					GetEditor().ToggleSelect(this);
				}
				
				break;
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
}