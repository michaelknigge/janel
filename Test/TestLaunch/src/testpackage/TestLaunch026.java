package testpackage;

import java.util.*;

public class TestLaunch026
{
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println("Testing launch from TestLaunch0025 using Runtime.exec");
			System.out.println("java.home=" + props.getProperty("java.home"));
			System.out.println("user.dir=" + props.getProperty("user.dir"));
			
			System.out.println("TestLaunch026 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch026 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}