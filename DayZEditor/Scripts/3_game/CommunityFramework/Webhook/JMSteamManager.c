class JMSteamManager
{
/*
 * Will support:
 *  - Retrieving steam name
 *  - Retrieving steam photo
 */
};

static ref JMSteamManager g_jmSteamManager;

static ref JMSteamManager GetSteamManager()
{
	return g_jmSteamManager;
}

static void CreateSteamManager()
{
	g_jmSteamManager = new JMSteamManager;
}