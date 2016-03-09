package com.familyroomgames.piratesvsninjasdeluxe;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import com.familyroomgames.piratesvsninjasdeluxe.R;

public class TutorialScreenActivity extends Activity {

    private WebView contentWebView;
    private Handler handler = new Handler();

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.tutorial);

        final TutorialScreenActivity activity = this;

        // content
        contentWebView = (WebView)findViewById(R.id.web_view);
        contentWebView.setWebViewClient(new WebViewClient()
        {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url)
            {
                if (url.startsWith("pirates://"))
                {
                    finish();
                    return true;
                }
                else
                {
                    return false;
                }
            };
        });
    }

    protected void onStart() {
        super.onStart();
        contentWebView.loadUrl("file:///android_asset/Tutorial/index.html");
    }
}
