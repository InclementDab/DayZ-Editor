class EditorInventoryData: EditorInventoryAttachmentData
{
	int Version = 1;
		
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(Version);
		super.Write(serializer, Version);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(Version);
		super.Read(serializer, Version);
		return true;
	}
}