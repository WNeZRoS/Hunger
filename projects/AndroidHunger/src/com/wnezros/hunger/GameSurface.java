package com.wnezros.hunger;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

public class GameSurface extends GLSurfaceView {
	
	GameRenderer _Renderer;
	
	public GameSurface(Context context) {
        super(context);
        
        setDebugFlags(DEBUG_CHECK_GL_ERROR);
        
        _Renderer = new GameRenderer();
        setRenderer(_Renderer);
    }
	
	public boolean onTouchEvent(final MotionEvent event) {
        switch(event.getAction()) {
        case MotionEvent.ACTION_DOWN:
            GameContext.mouseDown(1, Float.valueOf(event.getX()).intValue(), Float.valueOf(event.getY()).intValue());
            break;
        case MotionEvent.ACTION_MOVE:
        	GameContext.mouseMove(Float.valueOf(event.getX()).intValue(), Float.valueOf(event.getY()).intValue());
        	break;
        case MotionEvent.ACTION_UP:
        	GameContext.mouseUp(1);
        	break;
        }
        return true;
    }
}

class GameRenderer implements GLSurfaceView.Renderer {
	@Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		GameContext.init();
    }

	@Override
    public void onSurfaceChanged(GL10 gl, int w, int h) {
		Log.i("SurfaceResize", "New size " + w + " x " + h);
        GameContext.resize(w, h);
    }

	@Override
    public void onDrawFrame(GL10 gl) {
        GameContext.render();
    }
}

