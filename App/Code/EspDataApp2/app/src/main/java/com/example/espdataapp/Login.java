package com.example.espdataapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.textfield.TextInputEditText;
import com.vishnusivadas.advanced_httpurlconnection.PutData;

public class Login extends AppCompatActivity {

    TextInputEditText textInputEditTextUserName,textInputEditTextPassword;
    Button buttonLogin;

    ProgressBar progressBar;
    String value;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        textInputEditTextUserName= findViewById(R.id.username);
        textInputEditTextPassword= findViewById(R.id.password);
        buttonLogin= findViewById(R.id.buttonLogin);
        progressBar=findViewById(R.id.progress);

        buttonLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final String username,password;
                username=String.valueOf(textInputEditTextUserName.getText());
                password=String.valueOf(textInputEditTextPassword.getText());

                if (!username.equals("") && !password.equals(""))
                {
                    progressBar.setVisibility(View.INVISIBLE);
                    Handler handler = new Handler();
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            String [] field = new String [2];
                            field[0]="username";
                            field[1]="password";
                            String[] data = new String [2];
                            data[0]=username;
                            data[1]=password;
                            PutData putData = new PutData("http://datamerge.000webhostapp.com/LogIn-SignUp-master/login.php","POST",field,data);
                            if (putData.startPut()) {
                                if (putData.onComplete()){
                                    progressBar.setVisibility(View.GONE);
                                    String result = putData.getResult();
                                    if (result.equals("Login Success")){
                                        Toast.makeText(getApplicationContext(),result,Toast.LENGTH_SHORT).show();
                                        Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                                        value=username;
                                        intent.putExtra("userCode",value);
                                        startActivity(intent);
                                        finish();

                                    }
                                    else {
                                        Toast.makeText(getApplicationContext(),result, Toast.LENGTH_SHORT).show();
                                    }
                                }
                            }
                        }
                    });
                }
                else {
                    Toast.makeText(getApplicationContext(),"All fields required",Toast.LENGTH_SHORT).show();
                }
            }
        });
    }
}