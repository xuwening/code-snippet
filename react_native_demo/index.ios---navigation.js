

import React, {
  Component,
} from 'react';

import {

    AppRegistry,
    Image,
    StyleSheet,
    Text,
    View,
    TouchableOpacity,
    Navigator,
    TabBarIOS,
} from 'react-native';


class SecondView extends Component {

  pressAction() {
    this.props.nav.pop();
  }

  render() {

    return (
        <View style={styles.containerSec}>

          <TouchableOpacity onPress={this.pressAction.bind(this)}>
            <Text> pop back. </Text> 
          </TouchableOpacity>

        </View>
      );
  }
}


class HomePage extends Component {


  pressAction() {

      //每次调用push时，其实就是调用MyNavigator中的renderScene（_renderScene）
      this.props.nav.push({
          component: SecondView,
        });
  }

  render() {

    return (
        <View style={styles.container}>
          <TouchableOpacity onPress={this.pressAction.bind(this)}>
            <Text> this is a text. </Text> 
          </TouchableOpacity>
        </View>
      );
  }
}


class MyNavigator extends Component {

  //跳转场景（全局）
  _renderScene(router, navigator) {

    var SomeComponent = router.component;
    return <SomeComponent nav={navigator} />
  }

  render() {

    return ( 

    <Navigator

      initialRoute={{component: HomePage}}

      configureScene={(route) => { return Navigator.SceneConfigs.VerticalDownSwipeJump; }}

      renderScene={this._renderScene}/>

      );
  }
}


var styles = StyleSheet.create({

  container: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: 'red',
  },

  containerSec: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: 'blue',
  },

  tabText: {
    width: 53,
    height: 81,
  },
});

AppRegistry.registerComponent('reactDemo', () => MyNavigator);

