class EditorInventoryAttachmentData: SerializableBase
{
	string Type;
	ref array<ref EditorInventoryAttachmentData> Attachments = {};
	
	void ~EditorInventoryAttachmentData()
	{
		delete Attachments;
	}
	
	void AssignFromEntity(EntityAI entity)
	{
		Type = entity.GetType();
		for (int i = 0; i < entity.GetInventory().AttachmentCount(); i++) {
			EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(i);
			if (!attachment) {
				continue;
			}
			
			EditorInventoryAttachmentData inv_attachment();
			inv_attachment.AssignFromEntity(attachment);
			Attachments.Insert(inv_attachment);
		}
	}
	
	void AssignToEntity(EntityAI entity)
	{
		if (entity.GetType() != Type) {
			EditorLog.Warning("Type was incorrect, expected %1, got %2", Type, entity.GetType());
			//return; // todo doesnt really work very well, need to do more general checks
		}
		
		// todo items in hands are not properly supported, going to need another way of doing this
		foreach (EditorInventoryAttachmentData attachment: Attachments) {
			EntityAI attachment_item = entity.GetInventory().CreateAttachment(attachment.Type);
			if (!attachment_item) {
				continue;
			}
			
			attachment.AssignToEntity(attachment_item);
		}
	}
	
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(Type);
		serializer.Write(Attachments.Count());
		foreach (EditorInventoryAttachmentData attachment: Attachments) {
			attachment.Write(serializer, version);
		}
	}
	
	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(Type);
		int length;
		serializer.Read(length);
		for (int i = 0; i < length; i++) {
			EditorInventoryAttachmentData attachment();
			attachment.Read(serializer, version);
			Attachments.Insert(attachment);	
		}
		
		return true;
	}
}