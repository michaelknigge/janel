package testpackage;


public class TestLaunch040
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Test launch checking setting ");
			System.out.println("janel.memory.check.limits=true");
			System.out.println("With both janel.memory.max.total.percent and janel.memory.init.total.percent set to 100");
			
			Runtime rt = Runtime.getRuntime();
			System.out.println("Runtime.maxMemory=" + rt.maxMemory());
			System.out.println("Runtime.totalMemory=" + rt.totalMemory() + " (corresponds to setting initial memory)");
			
			System.out.println("TestLaunch040 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch040 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}