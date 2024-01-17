class StaticCamera: Object
{
	protected ref EditorNametag m_EditorNametag;
		
	void SetNametag(string nametag)
	{
		m_EditorNametag = new EditorNametag(nametag, this);
	}
}