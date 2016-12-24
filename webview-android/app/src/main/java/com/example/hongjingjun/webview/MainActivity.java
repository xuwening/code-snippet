package com.example.hongjingjun.webview;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.webkit.JavascriptInterface;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;


import java.io.File;


public class MainActivity extends ActionBarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        WebView webView = (WebView)findViewById(R.id.webview);
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webSettings.setLayoutAlgorithm(WebSettings.LayoutAlgorithm.NARROW_COLUMNS);
        webSettings.setUseWideViewPort(true);
        webSettings.setLoadWithOverviewMode(true);
        boolean befirst = true;
        webView.setWebViewClient(new WebViewClient() {

            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {

                view.loadUrl(url);
//                view.loadUrl("(function() { newSdkInterface.getDocmentTitle('aaaa'); })();");
                return true;
            }

//            @Override
//            public void onPageFinished(WebView view, String url) {
//
//
//            }
        });

        webView.addJavascriptInterface(new JSSDKImpl(), "newSdkInterface");
//        webView.loadUrl("http://192.168.5.16:8083/appother/pages/js-sdk-test.html");
//        webView.loadUrl("http://111.20.119.234:9080/leadeon-cmcc-static/v2.0/pages/service/analysis/callDetailsAnalysis.html");
        webView.loadUrl("http://111.20.119.234:9080/appother/pages/js-sdk-test.html");
        webView.requestFocus();

//        webView.loadUrl("(function() { newSdkInterface.getDocmentTitle(document.title); })();");

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
