#ifndef COMPONENT_SYSTEM
[RegisterProfileSetting(EditorStatistics)]
#endif
class EditorStatistics: ProfileSettings
{		
	int EditorPlayTime; // seconds
	int EditorPlacedObjects;
	int EditorRemovedObjects;
	int EditorPlacedCameraTracks;
	int EditorCameraTracksRidden;
	float DistanceFlown; // meters
	int CharactersControlled;
	int CharactersEdited;
}