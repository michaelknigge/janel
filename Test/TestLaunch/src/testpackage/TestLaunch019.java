package testpackage;

import java.util.*;

public class TestLaunch019
{
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));
			System.out.println("testing setting janel.memory.max.total.percent=25");
			System.out.println("and setting janel.memory.init.total.percent=15");
					
			Runtime rt = Runtime.getRuntime();
			System.out.println("Runtime.maxMemory=" + rt.maxMemory());
			System.out.println("Runtime.totalMemory=" + rt.totalMemory() + " (corresponds to setting initial memory)");
			
			//Thread.sleep(15000); // Could use this to delay exit in order to check process

			System.out.println("TestLaunch019 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch019 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}