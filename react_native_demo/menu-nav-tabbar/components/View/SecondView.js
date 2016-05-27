
import React, {Component} from 'react';
import {
  View,
  Text,
} from 'react-native';

import NavigationBar from 'react-native-navbar';


class SecondView extends Component {

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
      title: 'Second View',
    };

    return (
      <View style={{ flex: 1, }}>
        <NavigationBar
          title={titleConfig}
          leftButton={leftButtonConfig}
          rightButton={rightButtonConfig} />

          <Text> 这是一个二级页面。
          </Text>
      </View>
    );

  }
}

module.exports = SecondView;