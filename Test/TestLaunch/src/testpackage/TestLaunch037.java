package testpackage;

public class TestLaunch037
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Testing launch checking setting ");
			System.out.println("janel.memory.init.total.percent=1");
			System.out.println("janel.memory.init.lower.limit=700000");
			
			Runtime rt = Runtime.getRuntime();
			System.out.println("Runtime.maxMemory=" + rt.maxMemory());
			System.out.println("Runtime.totalMemory=" + rt.totalMemory() + " (corresponds to setting initial memory)");
			
			if(rt.totalMemory() < 650000000L)
			{
				throw new Exception("Failed");
			}
			
			System.out.println("TestLaunch037 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch037 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}