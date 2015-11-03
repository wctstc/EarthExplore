package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class JavaLog {
	public static void Log(String msg){     
		FileOutputStream out = null;
		try {
		    out = new FileOutputStream(new File("/mnt/sdcard/CocoSS/pay_server.log"), true );
		    out.write((msg+"\n").getBytes());
		    out.close();
		}
		catch( Exception e ){
			e.printStackTrace();
		}
		finally {
			try {
				out.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}
