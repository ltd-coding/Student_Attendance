package com.example.espdataapp;

public class dataDetail {
    private String studentCode;
    private String studentName;
    private int numAttend;
    private int numLate;
    private String lastCheckIn;;

    public dataDetail(String studentCode, String studentName, int numAttend, int numLate, String lastCheckIn) {
        this.studentCode = studentCode;
        this.studentName = studentName;
        this.numAttend = numAttend;
        this.numLate = numLate;
        this.lastCheckIn = lastCheckIn;
    }

    public String getStudentCode() {
        return studentCode;
    }

    public void setStudentCode(String studentCode) {
        this.studentCode = studentCode;
    }

    public String getStudentName() {
        return studentName;
    }

    public void setStudentName(String studentName) {
        this.studentName = studentName;
    }

    public int getNumAttend() {
        return numAttend;
    }

    public void setNumAttend(int numAttend) {
        this.numAttend = numAttend;
    }

    public int getNumLate() {
        return numLate;
    }

    public void setNumLate(int numLate) {
        this.numLate = numLate;
    }

    public String getLastCheckIn() {
        return lastCheckIn;
    }

    public void setLastCheckIn(String lastCheckIn) {
        this.lastCheckIn = lastCheckIn;
    }
}
