class LightningEvent: EventBase
{
	protected vector m_Position;
	
	void LightningEvent()
	{
	}
	
	override void OnStart(Param start_params)
	{
		super.OnStart(start_params);
		
		Param1<vector> position_param = Param1<vector>.Cast(start_params);
		m_Position = position_param.param1;
	}
	
	override SerializableParam1<vector> GetClientSyncData(int event_phase)
	{
		return SerializableParam1<vector>.Create(m_Position);
	}
	
	override void InitPhaseClient(float phase_time, Param data)
	{
		m_Position = Param1<vector>.Cast(data).param1;
	}
	
	override void MidPhaseClient(float phase_time, Param data)
	{
		m_Position = Param1<vector>.Cast(data).param1;
	}
	
	override void EndPhaseClient(float phase_time, Param data)
	{
		m_Position = Param1<vector>.Cast(data).param1;
		
		float distance = vector.Distance(GetGame().GetPlayer().GetPosition(), m_Position);
		if (distance > 4000) {
			return;
		}
		
		string sound = "ThunderHeavy_Near_SoundSet";
		if (distance > 2000) {
			sound = "ThunderHeavy_Far_SoundSet";
		}
		
		SEffectManager.PlaySound(sound, m_Position).SetSoundAutodestroy(true);
	}
	
	override void InitPhaseServer()
	{
	}
	override void MidPhaseServer();
	override void EndPhaseServer();
	
	
	override float GetInitPhaseLength()
	{
		return 0.1;
	}
	
	override float GetMidPhaseLength()
	{
		return 0.1;
	}
	
	override float GetEndPhaseLength()
	{
		return 0.1;
	}
}