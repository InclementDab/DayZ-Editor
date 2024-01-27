// I think its wise... it was
class CommandNode: TreeNode
{
	void CommandNode(string uuid, string display_name, Symbols icon)
	{
#ifndef SERVER
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(PollShortcutExecution);
#endif
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
				if (m_UAInput.LocalClick()) {
					SetSelected(true);
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
				if (m_UAInput.LocalClick()) {
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
		return ShortcutKeyType.PRESS;
	}
}