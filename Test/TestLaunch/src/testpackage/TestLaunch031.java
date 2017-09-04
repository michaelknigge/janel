package testpackage;

public class TestLaunch031
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Testing line continuation with a long class path set.");
			System.out.println("System property java.class.path=" + System.getProperty("java.class.path"));

			System.out.println("TestLaunch031 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch031 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}