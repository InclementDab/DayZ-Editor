[RegisterEditorCommand(EditorAssetsBrowserMoveToCommand)]
class EditorAssetsBrowserMoveToCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		if (!m_Editor) {
			return false;
		}

		Param5<EditorAssetsBrowserView, EditorAssetsBrowserEntry, EditorAssetsBrowserSectionData, string, string> data = Param5<EditorAssetsBrowserView, EditorAssetsBrowserEntry, EditorAssetsBrowserSectionData, string, string>.Cast(GetData());
		if (!data || !data.param1 || data.param4 == string.Empty || data.param5 == string.Empty) {
			return false;
		}

		if (data.param3) {
			return MoveSection(data.param1, data.param3, data.param4, data.param5);
		}

		if (data.param2) {
			return MoveEntry(data.param1, data.param2, data.param4, data.param5);
		}

		return false;
	}

	protected bool MoveEntry(EditorAssetsBrowserView assets_browser, EditorAssetsBrowserEntry entry, string tab_id, string subcategory_id)
	{
		if (!assets_browser || !entry) {
			return false;
		}

		// TODO: Persist asset reclassification through the source-owned AssetsBrowser XML pack.
		EditorLog.Info("TODO EditorAssetsBrowserMoveToCommand::MoveEntry %1 -> %2/%3", entry.StableId, tab_id, subcategory_id);
		m_Editor.GetEditorHud().CreateNotification("Assets Browser asset reclassification is not implemented yet.", 3.0, LinearColor.YELLOW);
		return true;
	}

	protected bool MoveSection(EditorAssetsBrowserView assets_browser, EditorAssetsBrowserSectionData section, string tab_id, string subcategory_id)
	{
		if (!assets_browser || !section) {
			return false;
		}

		// TODO: Persist section reclassification through the source-owned AssetsBrowser XML pack.
		EditorLog.Info("TODO EditorAssetsBrowserMoveToCommand::MoveSection %1 -> %2/%3", section.Id, tab_id, subcategory_id);
		m_Editor.GetEditorHud().CreateNotification("Assets Browser section reclassification is not implemented yet.", 3.0, LinearColor.YELLOW);
		return true;
	}

	override string GetName()
	{
		return "Move To";
	}
}
