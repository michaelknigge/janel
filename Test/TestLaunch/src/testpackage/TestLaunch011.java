package testpackage;

import java.util.*;

public class TestLaunch011
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing -Dself.home.test=${SELF_HOME}, \nself.home.test=" + props.getProperty("self.home.test"));						
			System.out.println( "Testing -Dfound.exe.folder.test=${FOUND_EXE_FOLDER}, \nfound.exe.folder.test=" + props.getProperty("found.exe.folder.test"));	
			System.out.println("TestLaunch011 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch011 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}