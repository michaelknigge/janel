package testpackage;

public class TestLaunch036
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Testing launch checking setting ");
			System.out.println("janel.memory.init.total.percent=100");
			System.out.println("janel.memory.init.upper.limit=600000");
			
			Runtime rt = Runtime.getRuntime();
			System.out.println("Runtime.maxMemory=" + rt.maxMemory());
			System.out.println("Runtime.totalMemory=" + rt.totalMemory() + " (corresponds to setting initial memory)");
			
			if(rt.totalMemory() > 650000000L)
			{
				throw new Exception("Failed");
			}
			
			System.out.println("TestLaunch036 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch036 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}