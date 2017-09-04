package testpackage;

public class TestLaunch032
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Testing launch using max Java version of 1.4.99");
			System.out.println("java.home=" + System.getProperty("java.home"));

			System.out.println("TestLaunch032 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch032 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}