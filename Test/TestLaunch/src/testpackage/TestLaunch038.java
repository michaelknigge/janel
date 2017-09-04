package testpackage;

import java.io.FileOutputStream;
import java.io.IOException;

public class TestLaunch038
{
	public static void main(String[] args)
	{
		try
		{
			Thread currentThread = Thread.currentThread();
			System.out.println("Is the current thread a daemon = " + currentThread.isDaemon());
			
			Thread workerThread = new Thread() {
				public void run() {
					try {
						String fileName = "outputFileTest38_" + System.currentTimeMillis() + ".txt";
						FileOutputStream outputFileTest38 = new FileOutputStream(fileName);
						for(int i=1; i < 10000; i++) {
							String outputString = " Writing " + i + ";";
							outputFileTest38.write(outputString.getBytes());
						}
						outputFileTest38.close();						
					}
					catch(IOException ioe)
					{
						ioe.printStackTrace();
					}
				}
			};
			workerThread.setDaemon(false);
			workerThread.start();

			while(workerThread.isAlive()) {
				// waiting for worker thread to end
			}
			
			System.out.println("TestLaunch038 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch038 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}