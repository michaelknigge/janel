package testpackage;

public class TestLaunch034
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Testing launch checking setting ");
			System.out.println("janel.memory.max.total.percent=100");
			System.out.println("janel.memory.max.upper.limit=500000");
			
			Runtime rt = Runtime.getRuntime();
			System.out.println("Runtime.maxMemory=" + rt.maxMemory());
			System.out.println("Runtime.totalMemory=" + rt.totalMemory() + " (corresponds to setting initial memory)");
			
			if(rt.maxMemory() > 550000000L)
			{
				throw new Exception("Failed");
			}
		
			System.out.println("TestLaunch034 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch034 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}