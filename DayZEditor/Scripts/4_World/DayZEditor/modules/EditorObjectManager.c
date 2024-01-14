class EditorObjectManagerModule: Managed
{				
	// Hidden object stuff
	void HideMapObject(notnull EditorHiddenObject target)
	{
		// strong ref
		m_EditorHiddenObjectRefs[target.GetID()] = target;
	
		m_DeletedObjects.InsertEditorHiddenObject(target);
		
		GetEditor().Statistics.EditorRemovedObjects++;
	}
	
	void UnhideMapObject(int target)
	{
		m_DeletedObjects.Remove(target);
				
		// remove strong ref		
		delete m_EditorHiddenObjectRefs[target];
		m_EditorHiddenObjectRefs.Remove(target);
	}
	
	void UnhideMapObject(notnull EditorHiddenObject target)
	{
		m_DeletedObjects.RemoveEditorHiddenObject(target);

		// remove strong ref
		delete m_EditorHiddenObjectRefs[target.GetID()];
		m_EditorHiddenObjectRefs.Remove(target.GetID());
	}
	
	void SelectHiddenObject(notnull EditorHiddenObject target)
	{
		m_SelectedDeletedObjects.InsertEditorHiddenObject(target);
		GetEditor().OnDeletedObjectSelected.Invoke(target);
		target.OnSelected();
	}
	
	void DeselectHiddenObject(notnull EditorHiddenObject target)
	{
		m_SelectedDeletedObjects.RemoveEditorHiddenObject(target);
		GetEditor().OnDeletedObjectDeselected.Invoke(target);
		target.OnDeselected();
	}
	
	void ToggleHiddenObjectSelection(notnull EditorHiddenObject target)
	{
		if (target.IsSelected())
			DeselectHiddenObject(target);
		else
			SelectHiddenObject(target);
	}
}