package org.cocos2dx.cpp;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.StringTokenizer;

import android.app.Activity;

import com.bmob.pay.tool.BmobPay;
import com.bmob.pay.tool.OrderQueryListener;
import com.bmob.pay.tool.PayListener;


public class PayThread extends Thread implements PayListener,OrderQueryListener{
	private Activity m_context;
	private BmobPay m_bmob_pay;
	private BufferedReader m_buffered_reader;
	private BufferedWriter m_buffered_writer;
	
	public PayThread( Activity context ){

		JavaLog.Log("Create PayThread");
		m_context = context;
		m_bmob_pay = new BmobPay( m_context );
	}
	public void run(){
		JavaLog.Log("Init Pay");
		BmobPay.init(m_context,"b7124aa5ba568de93d904eb496ce8549");
		CreateServerSocket();
	}
	
	private void CreateServerSocket(){
		ServerSocket ss = null;
		Socket s;
		try {
			ss = new ServerSocket(10812);

			JavaLog.Log("Create ServerSocket Succeed");
			//boolean bRunning = true;
			
			JavaLog.Log("Wait Connect");
			s = ss.accept();
			JavaLog.Log("Connect Succeed");

			m_buffered_reader = new BufferedReader( new InputStreamReader( s.getInputStream() ));
			m_buffered_writer = new BufferedWriter( new OutputStreamWriter( s.getOutputStream() ));

			String function;
			String args = new String("");

			while (true) {
				JavaLog.Log("Wait ReadLine");
				String msg = m_buffered_reader.readLine();
				JavaLog.Log("ReadLine:"+msg);
			
				StringTokenizer stz = new StringTokenizer(msg, " ");
		
				function = stz.nextToken();

				JavaLog.Log("function:"+function);
				
				if( stz.hasMoreTokens() ){
					args = stz.nextToken();
				}
				
				if( function.equals("pay")){
					JavaLog.Log("pay:"+args);
					pay( args );
				}	
				else if( function.equals("query")){
					JavaLog.Log("query:"+args);
					query( args );
				}
			}
		} catch (IOException e) {
			JavaLog.Log("CreateServerSocket IOException");
			StackTraceElement[] st =  e.getStackTrace();
			for( int i = 0; i < st.length; i++ ){
				JavaLog.Log(st[i].toString());
			}
		}
		finally
		{
			try {
				if( ss != null )
				{
					ss.close();
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}
	
	private void pay( String args ){
		StringTokenizer stz = new StringTokenizer( args, ",");
		
		String way = stz.nextToken();
		String price = stz.nextToken();
		String name = stz.nextToken();

		JavaLog.Log("pay way:"+way+",price:"+price+",name:"+name);
		if( way.equals( "zfb")){
			JavaLog.Log("zfb.invoke price:"+Double.parseDouble(price)+",name:"+name);
			m_bmob_pay.pay( Double.parseDouble(price),name,new PayListener()
			{
				//支付回调
				@Override
				public void orderId(String orderID) {
					JavaLog.Log("orderID:"+orderID);
				}
				@Override
				public void succeed() {
					JavaLog.Log("Pay Succeed");
					try { 
						m_buffered_writer.write("succeed succeed");
						m_buffered_writer.flush();
					}
					catch (IOException e) {
						JavaLog.Log("orderId IOException");
					}
				}
				//失败回调
				@Override
				public void fail(int fail_code, String fail_msg ) {
					if( fail_code == 10777 ){
						BmobPay.ForceFree();
						JavaLog.Log("Force Free");
					}
					JavaLog.Log("Pay Fail fail_code:"+fail_code + ",fail_msg:"+fail_msg);
					try { 
						m_buffered_writer.write("PayFail fail_code:"+fail_code + ",fail_msg:"+fail_msg);
						m_buffered_writer.flush();
					}
					catch (IOException e) {
						JavaLog.Log("orderId IOException");
					}
				}
				@Override
				public void unknow() {
					JavaLog.Log("Pay Unknow");
				}
			} );
		}
		else if( way.equals("wx")){
			JavaLog.Log("wx.invoke price:"+Double.parseDouble(price)+",name:"+name);
			m_bmob_pay.payByWX( Double.parseDouble(price),name,this );
		}
	}
	private void query( String args ){
		StringTokenizer stz = new StringTokenizer( args, ",");
		String orderID = stz.nextToken();

		JavaLog.Log("query orderID:"+orderID );
		m_bmob_pay.query(orderID,this );
	}
	
	
	
	//支付回调
	@Override
	public void orderId(String orderID) {

		JavaLog.Log("orderID:"+orderID);
		String rtn = new String("");
		try { 
			rtn += "orderId";
			rtn += " ";
		    rtn += orderID;
			m_buffered_writer.write(rtn);
			m_buffered_writer.flush();
		}
		catch (IOException e) {
			JavaLog.Log("orderId IOException");
			StackTraceElement[] st =  e.getStackTrace();
			for( int i = 0; i < st.length; i++ ){
				JavaLog.Log(st[i].toString());
			}
		}
	}
	@Override
	public void succeed() {
		JavaLog.Log("Pay Succeed");
	}
	@Override
	public void unknow() {
		JavaLog.Log("Pay Unknow");
	}

	
	//查询回调
	@Override
	public void succeed(String status) {
		JavaLog.Log("Query:"+status);
		String rtn = new String("");
		try {
			rtn += "succeed";
			rtn += " ";
			rtn += status;
			m_buffered_writer.write(rtn);
			m_buffered_writer.flush();
		}
		catch (IOException e) {
			JavaLog.Log("orderId IOException");
			StackTraceElement[] st =  e.getStackTrace();
			for( int i = 0; i < st.length; i++ ){
				JavaLog.Log(st[i].toString());
			}
		}
		
	}
	
	//失败回调
	@Override
	public void fail(int fail_code, String fail_msg ) {
		if( fail_code == 10777 ){
			BmobPay.ForceFree();
			JavaLog.Log("Pay Fail fail_code is "+fail_code + ", Force Free");
		}

		JavaLog.Log("Payfail_code:"+fail_code + ",fail_msg:"+fail_msg);
	}
	
}
