// I think its wise... it was
class CommandNode: TreeNode
{
	protected UAInput m_UAInput;
	protected ShortcutKeyType m_ShortcutKeyType;
	
	void CommandNode(string uuid, string display_name, Symbols icon, ShortcutKeyType key_type = ShortcutKeyType.NONE)
	{
		m_ShortcutKeyType = key_type;
		
#ifndef SERVER
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(PollShortcutExecution);
#endif
	}
	
	Editor GetEditor()
	{
		return Editor.Cast(m_Parent.GetParent());
	}
	
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		switch (m_UUID) {
			case "CursorToggle": {
				GetGame().GetUIManager().ShowCursor(selected);
				GetEditor().GetHud().ClearCursor();
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
				GetEditor().GetHud().CurrentSelectionMode = SelectionMode.BOX;
				if (selected) {
					m_Parent["CircleSelect"].SetSelected(false);
					m_Parent["LassoSelect"].SetSelected(false);
				}
				
				break;
			}
			
			case "CircleSelect": {
				GetEditor().GetHud().CurrentSelectionMode = SelectionMode.ELLIPSE;
				if (selected) {
					m_Parent["BoxSelect"].SetSelected(false);
					m_Parent["LassoSelect"].SetSelected(false);
				}
					
				break;
			}
			
			case "LassoSelect": {
				GetEditor().GetHud().CurrentSelectionMode = SelectionMode.LASSO;
				if (selected) {
					m_Parent["CircleSelect"].SetSelected(false);
					m_Parent["BoxSelect"].SetSelected(false);
				}
				break;
			}
			
			case "Delete": {
				foreach (TreeNode node: TreeNode.SelectedObjects) {					
					GetEditor().InsertHistory("Undo Delete", Symbols.CLOCK_ROTATE_LEFT, node.CreateCopy());					
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
					SetSelected(true);
					break;
				}
				
				break;
			}
			
			case ShortcutKeyType.DOUBLE: {
				if (m_UAInput.LocalDoubleClick()) {
					SetSelected(!IsSelected());
				}
				
				break;
			}
			
			case ShortcutKeyType.HOLD: {
				if (m_UAInput.LocalHoldBegin()) {
					SetSelected(true);
				}
				
				if (m_UAInput.LocalRelease()) {
					SetSelected(false);
				}
				
				break;
			}
			
			case ShortcutKeyType.TOGGLE: {
				if (m_UAInput.LocalPress()) {
					SetSelected(!IsSelected());
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