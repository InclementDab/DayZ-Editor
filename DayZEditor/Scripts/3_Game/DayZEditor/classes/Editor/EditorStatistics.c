#ifndef COMPONENT_SYSTEM
[RegisterProfileSetting(EditorStatistics)]
#endif
class EditorStatistics: ProfileSettings
{		
	int EditorPlayTime;
	int EditorPlacedObjects;
	int EditorRemovedObjects;
	float EditorDistanceFlown; // storing in km
	int CharactersControlled;
	int CharactersEdited;
}