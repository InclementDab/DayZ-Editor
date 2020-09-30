
class EditorEditBoxDialog: EditorDialogBase
{
	protected ref EditBoxPrefab m_EditBoxPrefab;
	
	void EditorEditBoxDialog(Widget parent = null, string title = "")
	{
		m_EditBoxPrefab = new EditBoxPrefab();
		AddContent(m_EditBoxPrefab);
		
		AddButton("Create", DialogResult.OK);
		AddButton("Cancel", DialogResult.Cancel);
	}
	
	void ~EditorEditBoxDialog()
	{
		//delete m_EditBoxPrefab;
	}
	
	DialogResult ShowDialog(out string edit_data)
	{
		DialogResult result = ShowDialog();
		Print(m_EditBoxPrefab);
		edit_data = m_EditBoxPrefab.GetEditBoxDialogController().Text;
		return result;
	}	
}