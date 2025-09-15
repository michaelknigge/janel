package testpackage;

import java.awt.Toolkit;
import java.util.Properties;


public class TestLaunch051
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Checking setting of environment variables");
						
			Properties props = System.getProperties();
			System.out.println( "self.home.test=" + props.getProperty("self.home.test"));
			System.out.println( "myprop.test1" + props.getProperty("myprop.test1"));	
			System.out.println( "myprop.test2" + props.getProperty("myprop.test2"));
			System.out.println( "myprop.test3" + props.getProperty("myprop.test3"));
			System.out.println( "myprop.test4" + props.getProperty("myprop.test4"));
			System.out.println( "myprop.test5" + props.getProperty("myprop.test5"));
			System.out.println( "myprop.test6" + props.getProperty("myprop.test6"));
			
			if(!props.getProperty("myprop.test1").equals(""))
			{
				throw new Exception("myprop.test1 should be empty");
			}

			if(props.getProperty("myprop.test2") != null)
			{
				throw new Exception("myprop.test2 should not be set");
			}

			// Environment variable "DDD" is not set, assign "DefaultXXX" to property,
			// but do NOT set the envonronment variable
			if(!props.getProperty("myprop.test3").equals("DefaultXXX"))
			{
				throw new Exception("myprop.test3 should be DefaultXXX");
			}

			// Environment variable "DDD" is not set, assign "DefaultZZZ" to property
			// AND set the envonronment variable
			if(!props.getProperty("myprop.test4").equals("DefaultZZZ"))
			{
				throw new Exception("myprop.test4 should be DefaultZZZ");
			}

			if(!props.getProperty("myprop.test5").equals(props.getProperty("self.home.test")))
			{
				throw new Exception("myprop.test5 should be " + props.getProperty("self.home.test"));
			}

			// Should be "DefaultZZZ" due to "${env.DDD:=DefaultZZZ}" for myprop.test4 above...
			if(!props.getProperty("myprop.test6").equals("DefaultZZZ"))
			{
				throw new Exception("myprop.test6 should be DefaultZZZ");
			}
			
			System.out.println("TestLaunch051 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch051 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}