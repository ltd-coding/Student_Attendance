package com.example.espdataapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.ListView;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {


    String numCode;
    String urlGetData;
    ListView listViewData;
    ArrayList<dataDetail> arrayData;
    DataAdapter adapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        numCode=getIntent().getStringExtra("userCode");
        urlGetData=String.format("http://datamerge.000webhostapp.com/LogIn-SignUp-master/view-data.php?param1=%1$s",numCode);



        listViewData=(ListView) findViewById(R.id.listView);
        arrayData = new ArrayList<>();

        adapter= new DataAdapter(this,R.layout.dong_data,arrayData);
        listViewData.setAdapter(adapter);

        GetData(urlGetData);
    }
    private void GetData (String url) {
        RequestQueue requestQueue= Volley.newRequestQueue(this);
        JsonArrayRequest jsonArrayRequest= new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        //Toast.makeText(MainActivity.this,response.toString(),Toast.LENGTH_SHORT).show();
                        //Toast.makeText(MainActivity.this,url,Toast.LENGTH_SHORT).show();
                        for (int i=0; i < response.length(); i++) {
                            try {
                                JSONObject object=response.getJSONObject(i);
                                arrayData.add(new dataDetail(
                                   object.getString("StudentCode"),
                                   object.getString("StudentName"),
                                   object.getInt("NumAttend"),
                                   object.getInt("NumLate"),
                                   object.getString("LastCheckIn") ));
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        }
                        adapter.notifyDataSetChanged();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Toast.makeText(MainActivity.this,error.toString(),Toast.LENGTH_LONG).show();
                        //Toast.makeText(MainActivity.this,url,Toast.LENGTH_SHORT).show();
                    }
                }
        );
        requestQueue.add(jsonArrayRequest);


    }
}