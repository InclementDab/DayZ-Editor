
class EditorEditBoxDialog: EditorDialogBase
{
	protected autoptr EditBoxPrefab m_EditBoxPrefab;
	
	void EditorEditBoxDialog(string title, string caption = "", string default_value = "")
	{
		AddContent(new EditBoxPrefab(caption, m_Controller, default_value));
		
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
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

class EditorFileDialog: EditorDialogBase
{
	protected autoptr EditBoxPrefab m_EditBoxPrefab;
	
	void EditorFileDialog(string title, string caption = "", string default_value = "", string button_name = "OK")
	{
		AddContent(new EditBoxPrefab(caption, m_Controller, default_value));
		
		AddButton(button_name, DialogResult.OK);
		AddButton(DialogResult.Cancel);
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