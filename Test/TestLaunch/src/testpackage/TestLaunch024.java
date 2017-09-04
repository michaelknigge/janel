package testpackage;

import java.util.*;

public class TestLaunch024
{
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println("Testing launching with user.dir set to FOUND_EXE_FOLDER");
			System.out.println("but the call directory is set to the calldir");
			System.out.println("java.home=" + props.getProperty("java.home"));
			System.out.println("user.dir=" + props.getProperty("user.dir"));
			System.out.println("caller.dir.test=" + props.getProperty("caller.dir.test"));
			
			System.out.println("TestLaunch024 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch024 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}