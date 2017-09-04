package testpackage;


public class TestLaunch042
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Testing debugging of a Janel launched application.");
			
			// increase count and sleep values for really testing this
			for(int i=0; i < 1; i++){
				System.out.println("Waiting for debugger to connect.");
				Thread.sleep(3000);
			}
			
			System.out.println("TestLaunch042 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch042 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}