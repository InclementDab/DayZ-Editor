class EditorScriptEditorDialog: EditorDialogBase
{
	protected autoptr MultilineEditBoxPrefab m_MultilineEditBoxPrefab;
		
	void EditorScriptEditorDialog(string title)
	{
		m_MultilineEditBoxPrefab = new MultilineEditBoxPrefab("Script Editor", null, string.Empty);
		AddContent(m_MultilineEditBoxPrefab);
		
		AddButton("Execute", DialogResult.OK);
		AddButton("Close", DialogResult.Cancel);
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