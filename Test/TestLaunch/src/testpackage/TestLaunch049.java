package testpackage;

import java.awt.Toolkit;


public class TestLaunch049
{
	public static void main(String[] args)
	{
		try
		{
			System.out.println("Checking screen resolution reported");
			
			int dotsPerInch = Toolkit.getDefaultToolkit().getScreenResolution();
			
			System.out.println("Toolkit.getDefaultToolkit().getScreenResolution()=" + dotsPerInch);
			
			if(dotsPerInch != 96)
			{
				throw new Exception("Wrong DPI!");
			}
			
			System.out.println("TestLaunch049 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch049 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}