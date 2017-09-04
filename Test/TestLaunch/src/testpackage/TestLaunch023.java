package testpackage;

import java.util.*;

public class TestLaunch023
{
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println("Testing launching from an exe in directory TestLaunch/bin");
			System.out.println("java.home=" + props.getProperty("java.home"));
			System.out.println("user.dir=" + props.getProperty("user.dir"));
			System.out.println("found.exe.folder.test=" + props.getProperty("found.exe.folder.test"));			
			System.out.println("self.home.test=" + props.getProperty("self.home.test"));
			System.out.println("caller.dir.test=" + props.getProperty("caller.dir.test"));
			
			System.out.println("TestLaunch023 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch023 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}