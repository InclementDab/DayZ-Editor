
class EditorEditBoxDialog: EditorDialogBase
{
	protected autoptr EditBoxPrefab m_EditBoxPrefab;
	
	void EditorEditBoxDialog(string title)
	{
		m_EditBoxPrefab = new EditBoxPrefab();
		AddContent(m_EditBoxPrefab);
		
		AddButton("Create", DialogResult.OK);
		AddButton("Cancel", DialogResult.Cancel);
	}
	
		
	DialogResult ShowDialog(out string edit_data)
	{
		// Need to store this variable since EVERYTHING is deleted after ShowDialog finishes
		EditBoxWidget edit_box = m_EditBoxPrefab.ContentText;
		
		DialogResult result = ShowDialog();
		edit_data = edit_box.GetText();
		return result;
	}	
}

class EditorMultilineEditBoxDialog: EditorDialogBase
{
	protected autoptr MultilineEditBoxPrefab m_MultilineEditBoxPrefab;
		
	void EditorMultilineEditBoxDialog(string title)
	{
		m_MultilineEditBoxPrefab = new MultilineEditBoxPrefab();
		AddContent(m_MultilineEditBoxPrefab);
		
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	DialogResult ShowDialog(out string text)
	{
		// Need to store this variable since EVERYTHING is deleted after ShowDialog finishes
		MultilineEditBoxWidget edit_box = m_MultilineEditBoxPrefab.ContentText;
		DialogResult result = ShowDialog();
		edit_box.GetText(text);
		return result;
	}	
}