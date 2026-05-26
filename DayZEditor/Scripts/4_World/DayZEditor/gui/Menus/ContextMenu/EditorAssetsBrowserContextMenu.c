class EditorAssetsBrowserContextMenu: EditorContextMenu
{
	void EditorAssetsBrowserContextMenu(float x, float y, EditorAssetsBrowserView assets_browser, EditorAssetsBrowserEntry entry = null, EditorAssetsBrowserSectionData section = null)
	{
		AddMenuItem(new EditorAssetsBrowserMoveToMenuItem(this, null, assets_browser, entry, section, x));

		if (entry && entry.Placeable) {
			AddMenuDivider();
			AddMenuButton(m_Editor.CommandManager[EditorLootEditorCommand]);

			m_Editor.CommandManager[EditorCopyPlaceableToClipboard].SetData(new Param1<EditorPlaceableItem>(entry.Placeable));
			AddMenuButton(m_Editor.CommandManager[EditorCopyPlaceableToClipboard]);
		}
	}
}
