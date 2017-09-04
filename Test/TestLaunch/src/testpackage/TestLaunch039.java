package testpackage;


public class TestLaunch039
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Testing displaying a splashscreen");
			Thread.sleep(2000);
			
			System.out.println("TestLaunch039 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch039 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}