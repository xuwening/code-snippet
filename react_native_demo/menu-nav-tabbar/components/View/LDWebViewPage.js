
import React, {Component} from 'react';
import {
  View,
  WebView,
  StyleSheet,
} from 'react-native';

import NavigationBar from 'react-native-navbar';

const styles = StyleSheet.create({

  webView: {
    backgroundColor: 'red',
    height: 350,
  },
});


class LDWebViewPage extends Component {

  render() {

   const rightButtonConfig = {
      title: 'share',
      handler: () => alert('hello!'),
    };

    const leftButtonConfig = {
      title: 'back',
      handler: () => this.props.nav.pop(),
    };

    const titleConfig = {
      title: 'WebView',
    };

    return (
      <View style={{ flex: 1, }}>
        <NavigationBar
          title={this.props.args}
          leftButton={leftButtonConfig}
          rightButton={rightButtonConfig} />

          <WebView
                ref={'webView'}
                automaticallyAdjustContentInsets={false}
                style={styles.webView}
                source={{uri: this.props.args.url}}
                javaScriptEnabled={true}
                domStorageEnabled={true}
                decelerationRate="normal"
                onNavigationStateChange={this.onNavigationStateChange}
                onShouldStartLoadWithRequest={this.onShouldStartLoadWithRequest}
                startInLoadingState={true}
                scalesPageToFit={true}/>

      </View>
    );

  }
}

module.exports = LDWebViewPage;

