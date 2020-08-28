


class EditorListItem
{
	private int m_NestIndex;
	
	protected Widget m_Root;
	Widget GetRoot() { 
		return m_Root;
	}
	

	
	protected ref EditorListItemController m_EditorListItemController;
	EditorListItemController GetController() { 
		return m_EditorListItemController; 
	}

	void EditorListItem() 
	{ 
		EditorLog.Trace("EditorListItem");

		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/items/EditorListItem.layout", null);		
		m_Root.GetScript(m_EditorListItemController);
		m_Root.SetUserData(this);
	}


	void ~EditorListItem() 
	{ 
		EditorLog.Trace("~EditorListItem"); 
		m_Root.Unlink();
	}
	
	
	void SetNestIndex(int index)
	{
		EditorLog.Trace("EditorListItem::SetNestIndex " + index);
		m_NestIndex = index;
		float x, y;
		m_EditorListItemController.EditorListItemFrame.GetSize(x, y);
		m_EditorListItemController.EditorListItemFrame.SetSize(290 - 15 * m_NestIndex, y);
		m_EditorListItemController.EditorListItemFrame.Update();
	}
	
	int GetNestIndex() {
		return m_NestIndex;
	}
	
		

	
	
		

	
}
