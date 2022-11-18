package com.example.myapplication;
import android.util.Log;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;


public class SocketUtil {
    private Socket socket;

    public SocketUtil(String ip,int port){
        try{
            this.socket=new Socket();

            this.socket.connect(new InetSocketAddress(ip,port),5000);
        }catch (SocketException e)
        {
    //连接超时，在UI界面显示消息
            Log.i("socket",e.toString());
        } catch (IOException e) {
            e.printStackTrace();
            Log.i("socket",e.toString());
        }catch (Exception e){
            Log.i("cdd",e.toString());
        }

    }

    public String sendMessage(String mes){
        String result="";
        try{

            OutputStream outputStream=socket.getOutputStream();
            outputStream.write(mes.getBytes());
            outputStream.flush();
            BufferedReader bfr=new BufferedReader(new InputStreamReader(socket.getInputStream()));
            String line=null;
            StringBuffer buffer=new StringBuffer();
            while ((line=bfr.readLine())!=null)
            {
                buffer.append(line);
            }
            result=buffer.toString();
            bfr.close();
            outputStream.close();

        }catch (SocketException e)
        {
//连接超时，在UI界面显示消息
            Log.i("socket",e.toString());
        }catch (IOException e)
        {
            e.printStackTrace();
        }
        return result;
    }
    public void clean() throws IOException {
        socket.close();
    }
}