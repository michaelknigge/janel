package testpackage;

import java.util.*;

public class TestLaunch017
{
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));
			System.out.println("testing setting janel.sysprop.process.id=true");
			System.out.println( "process.id=" + props.getProperty("process.id"));

			//Thread.sleep(10000); // Could use this to delay exit in order to check process id

			System.out.println("TestLaunch017 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch017 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}