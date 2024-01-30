// I think its wise... it was
class CommandNode: TreeNode
{	
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
			
	array<string> GetXorSelections()
	{
		return {};
	}
}