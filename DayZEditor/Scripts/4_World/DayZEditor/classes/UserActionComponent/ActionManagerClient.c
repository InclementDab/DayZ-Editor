modded class ActionManagerClient
{
	protected override void FindContextualUserActions(int pCurrentCommandID)
	{
		// TODO: NEEDS OPTIMIZATION (focus on UpdatePossibleActions > CraftingManager::OnUpdate)
		
		m_ActionsAvaibale = false;
		if (!m_ActionPossible || HasHandInventoryReservation() || GetGame().IsInventoryOpen()) {
			ResetInputsActions();
			return;
		}
		
		if (!GetRunningAction()) {	
			ActionBase action;
			ActionTarget 	target;
			ItemBase 		item;

			// Gathering current inputs
			m_ActionsAvaibale = true;
			
			item = FindActionItem();
			target = FindActionTarget();
			
			int actionConditionMask = ActionBase.ComputeConditionMask(m_Player,target,item);
			for (int i = 0; i < m_OrederedAllActionInput.Count();i++) {
				ActionInput ain = m_OrederedAllActionInput[i];
				if (ain) {
					ain.UpdatePossibleActions(m_Player,target,item, actionConditionMask);
				}
			}
			
			SetActionContext(target,item);
		}
	}
}