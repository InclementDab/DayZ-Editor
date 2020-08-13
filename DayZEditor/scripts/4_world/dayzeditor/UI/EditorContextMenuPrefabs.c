class EditorObjectContextMenu: EditorContextMenu
{
	protected ref EditorContextMenuButton m_CutButton;
	protected ref EditorContextMenuButton m_CopyButton;
	protected ref EditorContextMenuButton m_PasteButton;
	
	protected ref EditorContextMenuFolder m_FolderTest;
	
	private EditorObjectManager m_ObjectManager;
	
	void EditorObjectContextMenu()
	{
		m_ObjectManager = GetEditor().GetObjectManager();
		AddButton(new EditorContextMenuButton("Rename", "", null));
		AddButton(new EditorContextMenuDivider(""));
		
		m_CutButton = new EditorContextMenuButton("Cut", "CutSelection", m_ObjectManager);
		m_CutButton.Disable(!m_ObjectManager.CanCut());
		AddButton(m_CutButton);
		m_CopyButton = new EditorContextMenuButton("Copy", "CopySelection", m_ObjectManager);
		m_CopyButton.Disable(!m_ObjectManager.CanCopy());
		AddButton(m_CopyButton);
		m_PasteButton = new EditorContextMenuButton("Paste", "PasteSelection", m_ObjectManager);
		m_PasteButton.Disable(!m_ObjectManager.CanPaste());
		AddButton(m_PasteButton);
		
		AddButton(new EditorContextMenuDivider(""));
		
		m_FolderTest = new EditorContextMenuFolder("Folder1");
		m_FolderTest.AddButton(new EditorContextMenuButton("Option1"));
		AddButton(m_FolderTest);
	}
}