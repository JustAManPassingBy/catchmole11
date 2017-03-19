package com.example.shc.catchdotheg;

// 화면 설정 : 화면의 x 좌표, y좌표 크기를 가정하고 상대적 위치에 따라 두도록


public class ScreenConfig {
    public int Screen_width;
    public int Screen_height;
    public int Virtual_width;
    public int Virtual_height;

    // 실제 크기 받음
    public ScreenConfig(int screenwidth, int screenheight) {
        Screen_width = screenwidth;
        Screen_height = screenheight;
    }

    // 설정할 xy 크기를 받음.
    public void setSize(int setwidth, int setheight) {
        Virtual_width = setwidth;
        Virtual_height = setheight;
    }

    public int getX(int x) {
        return (int)(x * Screen_width / Virtual_width);
    }

    public int getY(int y) {
        return (int)(y * Screen_height / Virtual_height);
    }

}


