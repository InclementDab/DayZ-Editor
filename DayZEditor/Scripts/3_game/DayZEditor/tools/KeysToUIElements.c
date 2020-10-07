

modded class KeysToUIElements
{
	static ref map<int, ref KeyToUIElement> GetKeysMap() {
		if (!m_KeysToUIElements) {
			Init();
		}
		return m_KeysToUIElements;
	}
}