package com.wnezros.hunger;

public class GameContext {
	private static final int ACTIVATE = 0;
	private static final int CLOSE = 1;
	private static final int GOT_FOCUS = 2;
	private static final int LOST_FOCUS = 3;
	private static final int KEY_DOWN = 4;
	private static final int KEY_UP = 5;
	private static final int MOUSE_MOVE = 6;
	private static final int MOUSE_DOWN = 7;
	private static final int MOUSE_UP = 8;
	private static final int DRAW = 9;
	private static final int RESIZE = 10;
	
	private static native void main();
	private static native void event(int event, int p1, int p2);
	
    public static void init() {
    	main();
    }
	
	public static void resize(int w, int h) {
    	event(RESIZE, w, h);
    }
    
    public static void render() {
    	event(DRAW, 0, 0);
    }
    
    public static void close() {
    	event(CLOSE, 0, 0);
    }
    
    public static void resume() {
    	event(GOT_FOCUS, 0, 0);
    }
    
    public static void pause() {
    	event(LOST_FOCUS, 0, 0);
    }
    
    public static void keyDown(int key) {
    	event(KEY_DOWN, key, 0);
    }
    
    public static void keyUp(int key) {
    	event(KEY_UP, key, 0);
    }
    
    public static void mouseMove(int x, int y) {
    	event(MOUSE_MOVE, x, y);
    }
    
    public static void mouseDown(int key) {
    	event(MOUSE_DOWN, key, 0);
    }
    
    public static void mouseDown(int key, int x, int y) {
    	mouseMove(x, y);
    	mouseDown(key);
    }
    
    public static void mouseUp(int key) {
    	event(MOUSE_UP, key, 0);
    }
    
    static {
    	System.loadLibrary("hunger");
    }
}
