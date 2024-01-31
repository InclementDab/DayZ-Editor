class RelayCommandExec : RelayCommand
{
	// could just throw the bool inside CanExecute()
	protected bool m_CanExecute = true;
	override bool CanExecute()
	{
		return m_CanExecute;
	}

	void SetCanExecute(bool state)
	{
		m_CanExecute = state;
		thread CanExecuteChanged(m_CanExecute);
	}

	override bool Execute(Class sender, CommandArgs args);
	void CanExecuteChanged(bool state);
}

static int EditorObjectID;

modded class EditorObjectData
{
	[NonSerialized()]
	int m_Id;
	int GetID() { return m_Id; }
	void EditorObjectData()
	{
		EditorObjectID++;
		m_Id = EditorObjectID;
	}
}
