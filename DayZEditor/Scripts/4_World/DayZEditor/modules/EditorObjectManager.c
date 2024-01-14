class EditorObjectManagerModule: Managed
{	
	// Call to select an object
	void SelectObject(notnull EditorObject target)
	{
		m_SelectedObjects.InsertEditorObject(target);
		GetEditor().OnObjectSelected.Invoke(target);
	
		target.OnSelected();
		
		// todo perhaps propagate selections to the children of the object?
	}
	
	// Call to deselect an object
	void DeselectObject(notnull EditorObject target)
	{
		m_SelectedObjects.RemoveEditorObject(target);
		GetEditor().OnObjectDeselected.Invoke(target);
		target.OnDeselected();
	}	
		
	// Call to toggle selection
	void ToggleSelection(notnull EditorObject target)
	{
		if (target.IsSelected()) {
			DeselectObject(target);
		} else {
			SelectObject(target);
		}
	}
		
	// Call to clear selection
	void ClearSelection()
	{	
		foreach (EditorObject editor_object: m_SelectedObjects) {
			DeselectObject(editor_object);
		}
		
		foreach (EditorDeletedObject deleted_object: m_SelectedDeletedObjects) {
			DeselectHiddenObject(deleted_object);
		}
	}
	
	// Hidden object stuff
	void HideMapObject(notnull EditorDeletedObject target)
	{
		
		// strong ref
		m_EditorDeletedObjectRefs[target.GetID()] = target;
	
		m_DeletedObjects.InsertEditorDeletedObject(target);
		
		GetEditor().Statistics.EditorRemovedObjects++;
	}
	
	void UnhideMapObject(int target)
	{
		m_DeletedObjects.Remove(target);
				
		// remove strong ref		
		delete m_EditorDeletedObjectRefs[target];
		m_EditorDeletedObjectRefs.Remove(target);
	}
	
	void UnhideMapObject(notnull EditorDeletedObject target)
	{
		m_DeletedObjects.RemoveEditorDeletedObject(target);

		// remove strong ref
		delete m_EditorDeletedObjectRefs[target.GetID()];
		m_EditorDeletedObjectRefs.Remove(target.GetID());
	}
	
	void SelectHiddenObject(notnull EditorDeletedObject target)
	{
		m_SelectedDeletedObjects.InsertEditorDeletedObject(target);
		GetEditor().OnDeletedObjectSelected.Invoke(target);
		target.OnSelected();
	}
	
	void DeselectHiddenObject(notnull EditorDeletedObject target)
	{
		m_SelectedDeletedObjects.RemoveEditorDeletedObject(target);
		GetEditor().OnDeletedObjectDeselected.Invoke(target);
		target.OnDeselected();
	}
	
	void ToggleHiddenObjectSelection(notnull EditorDeletedObject target)
	{
		if (target.IsSelected())
			DeselectHiddenObject(target);
		else
			SelectHiddenObject(target);
	}
}