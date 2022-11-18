package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import org.w3c.dom.EntityReference;


public class MainActivity extends AppCompatActivity {
    private Button open,shutdown,up,down,play,prev,next,pause,mute,search;
    private TextView textView;
    private String result;
    private SocketUtil server;
    private Handler handler=new Handler(){
        @Override
        public void handleMessage(Message msg){
            super.handleMessage(msg);
            if(msg.what==123)
            {
                textView.setText(result);
            }
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        new Thread(new Runnable() {
            @Override
            public void run() {
                server= new SocketUtil("10.28.253.5",27015);
            }
        }).start();
        initView();

    }
    private void initView()
    {
        textView=(TextView)findViewById(R.id.content);
        final EditText editText=(EditText)findViewById(R.id.message);
        open=(Button)findViewById(R.id.open);
        shutdown=(Button)findViewById(R.id.shutdown);
        up=(Button)findViewById(R.id.up);
        down=(Button)findViewById(R.id.down);
        play=(Button)findViewById(R.id.play);
        prev=(Button)findViewById(R.id.prev);
        next=(Button)findViewById(R.id.next);
        pause=(Button)findViewById(R.id.pause);
        mute=(Button)findViewById(R.id.mute);
        search=(Button)findViewById(R.id.search);
        open.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("1");
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });
        shutdown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("9");
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });
        up.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("5");
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });
        down.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("6");
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });

        play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("2");
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });
        prev.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("3");
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });
        next.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("4");
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });
        pause.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("2");
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });
        mute.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("7");
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });
        search.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        result=server.sendMessage("9"+editText.getText().toString());
                        handler.sendEmptyMessage(123);
                    }
                }).start();
            }
        });



    }
}