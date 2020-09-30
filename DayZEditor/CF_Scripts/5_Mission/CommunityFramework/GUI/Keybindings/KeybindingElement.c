modded class KeybindingElement
{
	bool R_WasSet
	string R_DisplayName;

	override void Reload()
	{
		super.Reload();

		if ( R_WasSet )
		{
			m_ElementName.SetText( R_DisplayName );
		}
	}
}