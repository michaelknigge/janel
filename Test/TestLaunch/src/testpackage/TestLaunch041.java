package testpackage;


public class TestLaunch041
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Test launch checking java agent, there should be console output about the premain class executing.");
			System.out.println("TestLaunch041 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch041 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}