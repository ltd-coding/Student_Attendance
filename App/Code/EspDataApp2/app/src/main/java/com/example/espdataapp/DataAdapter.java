package com.example.espdataapp;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.util.List;

public class DataAdapter extends BaseAdapter {
    private Context context;
    private int layout;
    private List<dataDetail> dataDetailList;

    public DataAdapter(Context context, int layout, List<dataDetail> dataDetailList) {
        this.context = context;
        this.layout = layout;
        this.dataDetailList = dataDetailList;
    }

    @Override
    public int getCount() {
        return dataDetailList.size();
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    private class ViewHolder {
        TextView txtName,txtCode,txtDate,txtAttend,txtLate;
        ImageView imgAttend,imgLate;
    }
    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {

        ViewHolder holder;
        if (view==null) {
            holder=new ViewHolder();
            LayoutInflater inflater= (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = inflater.inflate(layout,null);
            holder.txtName=(TextView) view.findViewById(R.id.textViewStudentName);
            holder.txtCode= (TextView) view.findViewById(R.id.textViewStudentCode);
            holder.txtDate= (TextView) view.findViewById(R.id.textViewLastCheckIn);
            holder.txtAttend =(TextView) view.findViewById(R.id.textViewNumAttend);
            holder.txtLate=(TextView) view.findViewById(R.id.textViewNumLate);

            holder.imgAttend = (ImageView) view.findViewById(R.id.imgAttend);
            holder.imgLate = (ImageView) view.findViewById(R.id.imgLate);
            view.setTag(holder);
        }
        else {
            holder=(ViewHolder) view.getTag();
        }

        dataDetail data = dataDetailList.get(i);

        holder.txtName.setText(data.getStudentName());
        holder.txtCode.setText(data.getStudentCode());
        holder.txtDate.setText(String.valueOf(data.getLastCheckIn()));
        holder.txtAttend.setText(String.valueOf(data.getNumAttend()));
        holder.txtLate.setText(String.valueOf(data.getNumLate()));

        return view;
    }
}
