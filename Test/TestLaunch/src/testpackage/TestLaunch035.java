package testpackage;

public class TestLaunch035
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Testing launch checking setting ");
			System.out.println("janel.memory.max.total.percent=1");
			System.out.println("janel.memory.max.lower.limit=400000");
			
			Runtime rt = Runtime.getRuntime();
			System.out.println("Runtime.maxMemory=" + rt.maxMemory());
			System.out.println("Runtime.totalMemory=" + rt.totalMemory() + " (corresponds to setting initial memory)");
			
			if(rt.maxMemory() < 350000000L)
			{
				throw new Exception("Failed");
			}
			
			System.out.println("TestLaunch035 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch035 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}