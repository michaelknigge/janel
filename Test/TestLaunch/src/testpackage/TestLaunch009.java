package testpackage;

import java.util.*;

public class TestLaunch009
{
	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing setting Java system properties.");
			System.out.println( "setting -verbose:gc");
			System.out.println( "setting -Xincgc");
			TestLaunch009 tl = new TestLaunch009();
			Thread.sleep(1000);
			System.out.println("TestLaunch009 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch009 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}