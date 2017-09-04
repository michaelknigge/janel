package testpackage;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class TestLaunch047
{	
	public static void main(String[] args)
	{
		BufferedWriter bufferedLogFileWriter = null;
		try
		{	
			bufferedLogFileWriter = new BufferedWriter(new FileWriter("C:\\Test\\JanelTestLog047.txt"));
			bufferedLogFileWriter.write("Testing Janel's service by installing the service, starting it, then uninstalling it.\r\n");
			
			for(int i=0; i < args.length; i++)
			{
				bufferedLogFileWriter.write("Argument " + (i+1) + "=" + args[i] + "\r\n");	
			}
			
			bufferedLogFileWriter.write("TestLaunch047 SUCCESSFUL");
			bufferedLogFileWriter.flush();
			
			// sleep for 15 seconds just in case tester wants to see the service.
			long startTime = System.currentTimeMillis();
			long currentTime;
			do {
				Thread.sleep(5000);
				currentTime = System.currentTimeMillis();
			} while (currentTime - startTime < 15000);			
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch047 FAILURE");
			System.exit(1);
		}
		finally 
		{
			if(bufferedLogFileWriter != null) {
				try {
					bufferedLogFileWriter.close();
				}
				catch(IOException ioe) { }
			}
		}
		System.exit(0);
	}
}