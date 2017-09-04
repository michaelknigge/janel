package testpackage;

import java.util.*;
import java.awt.*;

public class TestLaunch021
{
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));
			System.out.println("testing a launch of an AWT app");
			Frame frame = new Frame("Testing AWT app");
			Label centerLabel = new Label("If this is XP, this should be XP themed.");
			frame.add("Center", centerLabel);
			frame.setSize(350,100);
			frame.setVisible(true);
			Thread.sleep(2000);

			System.out.println("TestLaunch021 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch021 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}