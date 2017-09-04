package testpackage;

import java.awt.Toolkit;
import java.util.Properties;


public class TestLaunch050
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Checking executable and os bitness");
						
			Properties props = System.getProperties();
			System.out.println( "executable.bitness=" + props.getProperty("executable.bitness"));	
			System.out.println( "opsys.bitness=" + props.getProperty("opsys.bitness"));	
			
			if(!props.getProperty("executable.bitness").equals("64"))
			{
				throw new Exception("Wrong executable bitness");
			}
			if(!props.getProperty("opsys.bitness").equals("64"))
			{
				throw new Exception("Wrong OS bitness");
			}
			
			System.out.println("TestLaunch050 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch050 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}